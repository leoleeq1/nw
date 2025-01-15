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
        bool fullscreen = false;
    };

    struct Window::Impl
    {
        static LRESULT CALLBACK s_WndProc(HWND hwnd, uint32_t msg, WPARAM wParam, LPARAM lParam);

        WindowDesc desc;
        HWND hwnd;
        WindowState state;
    };

    Window::Window() = default;
    Window::~Window() = default;

    void Window::Create(WindowDesc desc)
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

        RECT rc = {0, 0, desc.size.width, desc.size.height};

        AdjustWindowRectEx(&rc, WS_OVERLAPPEDWINDOW, FALSE, WS_EX_OVERLAPPEDWINDOW);

        impl_->hwnd = CreateWindowExA(WS_EX_OVERLAPPEDWINDOW, desc.identifier.c_str(), desc.title.c_str(), WS_OVERLAPPEDWINDOW, desc.position.x, desc.position.y, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, nullptr, impl_.get());

        if (!impl_->hwnd)
        {
            throw std::runtime_error("CreateWindowEx failed!");
        }

        ShowWindow(impl_->hwnd, SW_SHOWDEFAULT);

        GetClientRect(impl_->hwnd, &rc);
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
                if (impl->state.fullscreen)
                {
                    SetWindowLongPtr(hwnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
                    SetWindowLongPtr(hwnd, GWL_EXSTYLE, 0);

                    ShowWindow(hwnd, SW_SHOWNORMAL);
                    SetWindowPos(hwnd, HWND_TOP, 0, 0, impl->desc.size.width, impl->desc.size.height, SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);
                }
                else
                {
                    SetWindowLongPtr(hwnd, GWL_STYLE, WS_POPUP);
                    SetWindowLongPtr(hwnd, GWL_EXSTYLE, WS_EX_TOPMOST);

                    SetWindowPos(hwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
                    ShowWindow(hwnd, SW_SHOWMAXIMIZED);
                }

                impl->state.fullscreen = !impl->state.fullscreen;
            }
            break;
        }
        default:
            break;
        }
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
}