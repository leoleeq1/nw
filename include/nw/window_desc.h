#pragma once
#include <cstdint>
#include <string>

namespace nw
{
    enum class WindowMode
    {
        Windowed = 0,
        WindowedFullScreen,
        FullScreen,
    };

    enum class WindowOption : uint8_t
    {
        Centered = 1u << 0,
        Movable = 1u << 1,
        Resizable = 1u << 2,
        Minimizable = 1u << 3,
        Maximizable = 1u << 4,
    };

    struct WindowPosition
    {
        uint32_t x = 0;
        uint32_t y = 0;
    };

    struct WindowSize
    {
        uint32_t width = 800;
        uint32_t height = 600;
    };

    struct WindowDesc
    {
        std::string title = "nwWindow";
        std::string identifier = "nw";

        WindowPosition position = {.x = 0u, .y = 0u};
        WindowSize size = {.width = 800u, .height = 600u};
        WindowSize minSize = {.width = 100u, .height = 100u};
        WindowSize maxSize = {.width = 1u << 14, .height = 1u << 14};

        uint32_t backgroundColor = 0xFFFFFFFF;

        WindowMode mode = WindowMode::Windowed;
        WindowOption option = static_cast<WindowOption>(~1u);
    };
}