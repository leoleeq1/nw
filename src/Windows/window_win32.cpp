#include "nw/Event/ApplicationEvent.h"
#include "nw/Event/event_bus.h"
#include "nw/surface.h"
#include "nw/window.h"
#include "nw/window_desc.h"

#include <stdexcept>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace nw
{
struct WindowState
{
  WindowMode fullscreen = WindowMode::Windowed;
  bool sizemoving = false;
  bool suspended = false;
  bool minimized = false;
};

struct WindowStyleAndSize
{
  DWORD style = WS_OVERLAPPEDWINDOW;
  DWORD exStyle = WS_EX_OVERLAPPEDWINDOW;
  int32_t cmdShow = SW_SHOWDEFAULT;
  int32_t width = 800;
  int32_t height = 600;
};

namespace
{
WindowStyleAndSize GetStyleAndSize(WindowMode mode, WindowSize defaultSize)
{
  return WindowStyleAndSize{
    .style = mode == WindowMode::Windowed
               ? static_cast<DWORD>(WS_OVERLAPPEDWINDOW)
               : static_cast<DWORD>(WS_POPUP),
    .exStyle = mode == WindowMode::Windowed
                 ? static_cast<DWORD>(WS_EX_OVERLAPPEDWINDOW)
                 : static_cast<DWORD>(WS_EX_APPWINDOW),
    .cmdShow = mode == WindowMode::Windowed ? SW_SHOWDEFAULT : SW_SHOWMAXIMIZED,
    .width = mode == WindowMode::Windowed ? defaultSize.width
                                          : GetSystemMetrics(SM_CXSCREEN),
    .height = mode == WindowMode::Windowed ? defaultSize.height
                                           : GetSystemMetrics(SM_CYSCREEN),
  };
}
} // namespace

struct Window::Impl
{
  ~Impl();
  static LRESULT CALLBACK s_WndProc(
    HWND hwnd, uint32_t msg, WPARAM wParam, LPARAM lParam);
  void CreateDIBitmapObject(WindowSize size);

  WindowDesc desc;
  EventBus *eventBus;
  Surface surface;
  HWND hwnd;
  HDC memDC;
  HBITMAP bitmap;
  WindowState state;
};

Window::Impl::~Impl()
{
  if (bitmap)
  {
    DeleteObject(SelectObject(memDC, bitmap));
    bitmap = nullptr;
  }

  if (memDC)
  {
    ReleaseDC(nullptr, memDC);
    memDC = nullptr;
  }
}

void Window::Impl::CreateDIBitmapObject(WindowSize size)
{
  if (bitmap)
  {
    DeleteObject(SelectObject(memDC, bitmap));
    bitmap = nullptr;
  }

  std::byte *pixels = nullptr;
  BITMAPINFO bmi;
  memset(&bmi, 0, sizeof(BITMAPINFO));
  bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  bmi.bmiHeader.biWidth = size.width;
  bmi.bmiHeader.biHeight = size.height;
  bmi.bmiHeader.biPlanes = 1;
  bmi.bmiHeader.biBitCount = 32;
  bmi.bmiHeader.biCompression = BI_RGB;

  HBITMAP hbitmap = CreateDIBSection(memDC, &bmi, DIB_RGB_COLORS,
    reinterpret_cast<void **>(&pixels), nullptr, 0);

  surface = {reinterpret_cast<Color *>(pixels), size.width, size.height};

  if (!hbitmap)
  {
    throw std::runtime_error("Failed to create surface!");
  }

  bitmap = reinterpret_cast<HBITMAP>(SelectObject(memDC, hbitmap));
}

Window::Window() = default;
Window::~Window()
{
  if (impl_)
  {
    UnregisterClassA(impl_->desc.identifier.c_str(), nullptr);
  }
}

void Window::Create(const WindowDesc& desc, EventBus *eventBus)
{
  impl_ = std::make_unique<Impl>(desc, eventBus);
  const WNDCLASSEXA wc{
    .cbSize = sizeof(WNDCLASSEX),
    .style = CS_HREDRAW | CS_VREDRAW,
    .lpfnWndProc = Impl::s_WndProc,
    .cbClsExtra = 0,
    .cbWndExtra = 0,
    .hInstance = nullptr,
    .hIcon = LoadIcon(nullptr, IDI_APPLICATION),
    .hCursor = LoadCursor(nullptr, IDC_ARROW),
    .hbrBackground = CreateSolidBrush(desc.backgroundColor),
    .lpszMenuName = nullptr,
    .lpszClassName = desc.identifier.c_str(),
    .hIconSm = LoadIcon(nullptr, IDI_APPLICATION),
  };

  if (!RegisterClassExA(&wc))
  {
    throw std::runtime_error("RegisterClassEx failed!");
  }

  const WindowStyleAndSize wss = GetStyleAndSize(desc.mode, desc.size);

  impl_->state.fullscreen = desc.mode;

  RECT rc = {0, 0, wss.width, wss.height};

  AdjustWindowRectEx(&rc, wss.style, FALSE, wss.exStyle);

  impl_->hwnd =
    CreateWindowExA(wss.exStyle, desc.identifier.c_str(), desc.title.c_str(),
      wss.style, desc.position.x, desc.position.y, rc.right - rc.left,
      rc.bottom - rc.top, nullptr, nullptr, nullptr, impl_.get());

  if (!impl_->hwnd)
  {
    throw std::runtime_error("CreateWindowEx failed!");
  }

  ShowWindow(impl_->hwnd, wss.cmdShow);

  GetClientRect(impl_->hwnd, &rc);

  impl_->memDC = CreateCompatibleDC(nullptr);
  impl_->CreateDIBitmapObject({rc.right - rc.left, rc.bottom - rc.top});
}

bool Window::Update()
{
  MSG msg;
  while (PeekMessage(&msg, nullptr, NULL, NULL, PM_REMOVE))
  {
    if (msg.message == WM_QUIT)
    {
      return false;
    }

    TranslateMessage(&msg);
    DispatchMessageA(&msg);
  }

  return true;
}

Surface Window::GetSurface() noexcept
{
  return impl_->surface;
}

void Window::Present()
{
  HDC hdc = GetDC(impl_->hwnd);
  BitBlt(hdc, 0, 0, impl_->desc.size.width, impl_->desc.size.height,
    impl_->memDC, 0, 0, SRCCOPY);
}

LRESULT Window::Impl::s_WndProc(
  HWND hwnd, uint32_t msg, WPARAM wParam, LPARAM lParam)
{
  Impl *impl = reinterpret_cast<Impl *>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

  switch (msg)
  {
    case WM_NCCREATE:
    {
      auto *lpcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
      impl = reinterpret_cast<Impl *>(lpcs->lpCreateParams);
      SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(impl));
      impl->eventBus->SendEvent(std::make_unique<WindowCreatedEvent>());
      break;
    }
    case WM_PAINT:
    {
      impl->eventBus->SendEvent(std::make_unique<WindowPaintEvent>());
      break;
    }
    case WM_SIZE:
    {
      if (wParam == SIZE_MINIMIZED)
      {
        if (!impl->state.minimized)
        {
          impl->state.minimized = true;
          if (!impl->state.suspended)
          {
            impl->eventBus->SendEvent(std::make_unique<WindowSuspendedEvent>());
          }
          impl->state.suspended = true;
        }
      }
      else if (impl->state.minimized)
      {
        impl->state.minimized = false;
        if (impl->state.suspended)
        {
          impl->eventBus->SendEvent(std::make_unique<WindowResumedEvent>());
        }
        impl->state.suspended = false;
      }
      else if (!impl->state.sizemoving)
      {
        impl->eventBus->SendEvent(std::make_unique<WindowResizeEvent>(
          WindowSize(LOWORD(lParam), HIWORD(lParam))));
      }
      break;
    }
    case WM_ENTERSIZEMOVE:
    {
      impl->state.sizemoving = true;
      break;
    }
    case WM_EXITSIZEMOVE:
    {
      impl->state.sizemoving = false;
      RECT rc;
      GetClientRect(hwnd, &rc);
      impl->eventBus->SendEvent(std::make_unique<WindowResizeEvent>(
        WindowSize(rc.right - rc.left, rc.bottom - rc.top)));
      break;
    }
    case WM_GETMINMAXINFO:
    {
      if (lParam && impl)
      {
        auto *info = reinterpret_cast<MINMAXINFO *>(lParam);
        info->ptMinTrackSize.x = static_cast<LONG>(impl->desc.minSize.width);
        info->ptMinTrackSize.y = static_cast<LONG>(impl->desc.minSize.height);
        info->ptMaxTrackSize.x = static_cast<LONG>(impl->desc.maxSize.width);
        info->ptMaxTrackSize.y = static_cast<LONG>(impl->desc.maxSize.height);
      }
      break;
    }
    case WM_ACTIVATEAPP:
    {
      if (wParam)
      {
        impl->eventBus->SendEvent(std::make_unique<WindowActivatedEvent>());
      }
      else
      {
        impl->eventBus->SendEvent(std::make_unique<WindowDeactivatedEvent>());
      }
      break;
    }
    case WM_POWERBROADCAST:
    {
      switch (wParam)
      {
        case PBT_APMQUERYSUSPEND:
        {
          if (!impl->state.suspended)
          {
            impl->eventBus->SendEvent(std::make_unique<WindowSuspendedEvent>());
          }
          impl->state.suspended = true;
          return TRUE;
        }
        case PBT_APMRESUMESUSPEND:
        {
          if (!impl->state.minimized)
          {
            if (impl->state.suspended)
            {
              impl->eventBus->SendEvent(std::make_unique<WindowResumedEvent>());
            }
            impl->state.suspended = false;
          }
          return TRUE;
        }
        default:
          break;
      }
      break;
    }
    case WM_DESTROY:
    {
      PostQuitMessage(0);
      break;
    }
    case WM_CLOSE:
    {
      impl->eventBus->SendEvent(std::make_unique<WindowClosedEvent>());
      break;
    }
    case WM_SYSKEYDOWN:
    {
      if (wParam == VK_RETURN
          && (static_cast<uint32_t>(lParam) & 0x60000000U) == 0x20000000U)
      {
        const WindowMode target =
          impl->state.fullscreen == WindowMode::FullScreen
            ? WindowMode::Windowed
            : WindowMode::FullScreen;

        WindowStyleAndSize wss = GetStyleAndSize(target, impl->desc.size);
        wss.cmdShow = SW_SHOWNORMAL;
        UINT flags = SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED;

        if (target == WindowMode::FullScreen)
        {
          flags |= SWP_NOSIZE;
          wss.cmdShow = SW_SHOWMAXIMIZED;
        }

        SetWindowLongPtr(hwnd, GWL_STYLE, wss.style);
        SetWindowLongPtr(hwnd, GWL_EXSTYLE, wss.exStyle);
        SetWindowPos(hwnd, HWND_TOP, 0, 0, wss.width, wss.height, flags);
        ShowWindow(hwnd, wss.cmdShow);

        impl->state.fullscreen = target;
      }
      break;
    }
    default:
      break;
  }
  return DefWindowProc(hwnd, msg, wParam, lParam);
}
} // namespace nw