#include "nw/window.h"
#include "nw/window_desc.h"

#include <stdexcept>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace nw
{
    struct WindowState
    {
        bool sizemoving = false;
        bool suspended = false;
        bool minimized = false;
        WindowMode fullscreen = WindowMode::Windowed;
    };

    struct WindowStyleAndSize
    {
        DWORD style = WS_OVERLAPPEDWINDOW;
        DWORD exStyle = WS_EX_OVERLAPPEDWINDOW;
        int32_t cmdShow = SW_SHOWDEFAULT;
        int32_t width = 800;
        int32_t height = 600;
    };

    static WindowStyleAndSize GetStyleAndSize(WindowMode mode, WindowSize defaultSize)
    {
        return WindowStyleAndSize{
            .style = mode == WindowMode::Windowed
                         ? static_cast<DWORD>(WS_OVERLAPPEDWINDOW)
                         : static_cast<DWORD>(WS_POPUP),
            .exStyle = mode == WindowMode::Windowed
                           ? static_cast<DWORD>(WS_EX_OVERLAPPEDWINDOW)
                           : static_cast<DWORD>(WS_EX_APPWINDOW),
            .cmdShow = mode == WindowMode::Windowed
                           ? SW_SHOWDEFAULT
                           : SW_SHOWMAXIMIZED,
            .width = mode == WindowMode::Windowed
                         ? static_cast<int32_t>(defaultSize.width)
                         : GetSystemMetrics(SM_CXSCREEN),
            .height = mode == WindowMode::Windowed
                          ? static_cast<int32_t>(defaultSize.height)
                          : GetSystemMetrics(SM_CYSCREEN),
        };
    }

    struct Window::Impl
    {
        static LRESULT CALLBACK s_WndProc(HWND hwnd, uint32_t msg, WPARAM wParam, LPARAM lParam);

        WindowDesc desc;
        HWND hwnd;
        WindowState state;
    };

    Window::Window() = default;
    Window::~Window()
    {
        if (impl_)
        {
            UnregisterClassA(impl_->desc.identifier.c_str(), nullptr);
        }
    }

    void Window::Create(const WindowDesc &desc)
    {
        impl_ = std::make_unique<Impl>(desc);
        WNDCLASSEXA wc{
            .cbSize = sizeof(WNDCLASSEX),
            .style = CS_HREDRAW | CS_VREDRAW,
            .lpfnWndProc = Impl::s_WndProc,
            .cbClsExtra = 0,
            .cbWndExtra = 0,
            .hInstance = nullptr,
            .hIcon = LoadIcon(nullptr, IDC_ARROW),
            .hbrBackground = CreateSolidBrush(desc.backgroundColor),
            .lpszMenuName = nullptr,
            .lpszClassName = desc.identifier.c_str(),
            .hIconSm = LoadIcon(nullptr, IDI_APPLICATION),
        };

        if (!RegisterClassExA(&wc))
        {
            throw std::runtime_error("RegisterClassEx failed!");
        }

        WindowStyleAndSize wss = GetStyleAndSize(desc.mode, desc.size);

        impl_->state.fullscreen = desc.mode;

        RECT rc = {0, 0, wss.width, wss.height};

        AdjustWindowRectEx(&rc, wss.style, FALSE, wss.exStyle);

        impl_->hwnd = CreateWindowExA(wss.exStyle, desc.identifier.c_str(), desc.title.c_str(), wss.style,
                                      desc.position.x, desc.position.y, rc.right - rc.left, rc.bottom - rc.top,
                                      nullptr, nullptr, nullptr, impl_.get());

        if (!impl_->hwnd)
        {
            throw std::runtime_error("CreateWindowEx failed!");
        }

        ShowWindow(impl_->hwnd, wss.cmdShow);

        GetClientRect(impl_->hwnd, &rc);
        // Use rc to create GraphicsAPI Viewport
    }

    bool Window::Update()
    {
        MSG msg;
        while (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                return false;
            }

            TranslateMessage(&msg);
            DispatchMessageA(&msg);
        }

        InvalidateRect(impl_->hwnd, NULL, true);
        UpdateWindow(impl_->hwnd);

        return true;
    }

    LRESULT Window::Impl::s_WndProc(HWND hwnd, uint32_t msg, WPARAM wParam, LPARAM lParam)
    {
        Impl *impl = reinterpret_cast<Impl *>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

        switch (msg)
        {
        case WM_CREATE:
        {
            if (lParam)
            {
                LPCREATESTRUCT lpcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
                SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(lpcs->lpCreateParams));
            }
            break;
        }
        case WM_NCCREATE:
        {
            LPCREATESTRUCT lpcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
            SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(lpcs->lpCreateParams));
            break;
        }
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            std::ignore = BeginPaint(hwnd, &ps);
            EndPaint(hwnd, &ps);
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
                        ; // OnSuspend
                    impl->state.suspended = true;
                }
            }
            else if (impl->state.minimized)
            {
                impl->state.minimized = false;
                if (impl->state.suspended)
                    ; // OnResume
                impl->state.suspended = false;
            }
            else if (!impl->state.sizemoving)
            {
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
            break;
        }
        case WM_GETMINMAXINFO:
        {
            if (lParam && impl)
            {
                MINMAXINFO *info = reinterpret_cast<MINMAXINFO *>(lParam);
                info->ptMinTrackSize.x = impl->desc.minSize.width;
                info->ptMinTrackSize.y = impl->desc.minSize.height;
                info->ptMaxTrackSize.x = impl->desc.maxSize.width;
                info->ptMaxTrackSize.y = impl->desc.maxSize.height;
            }
            break;
        }
        case WM_ACTIVATEAPP:
        {
            if (wParam)
            {
                // OnActivated
            }
            else
            {
                // OnDeactivated
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
                    ; // OnSuspend
                impl->state.suspended = true;
                return TRUE;
            }
            case PBT_APMRESUMESUSPEND:
            {
                if (!impl->state.minimized)
                {
                    if (impl->state.suspended)
                        ; // OnResume
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
            break;
        }
        case WM_SYSKEYDOWN:
        {
            if (wParam == VK_RETURN && (lParam & 0x60000000) == 0x20000000)
            {
                WindowMode target = impl->state.fullscreen == WindowMode::FullScreen
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
}