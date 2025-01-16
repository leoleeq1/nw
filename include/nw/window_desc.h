#pragma once
#include <cstdint>
#include <string>

namespace nw
{
enum class WindowMode : uint8_t
{
  Windowed = 0,
  FullScreen,
};

enum class WindowOption : uint8_t
{
  Centered = 1U << 0U,
  Movable = 1U << 1U,
  Resizable = 1U << 2U,
  Minimizable = 1U << 3U,
  Maximizable = 1U << 4U,
};

struct WindowPosition
{
  int32_t x = 0;
  int32_t y = 0;
};

struct WindowSize
{
  int32_t width = 800;
  int32_t height = 600;
};

struct WindowDesc
{
  std::string title = "nwWindow";
  std::string identifier = "nw";

  WindowPosition position = {.x = 0, .y = 0};
  WindowSize size = {.width = 800, .height = 600};
  WindowSize minSize = {.width = 100, .height = 100};
  WindowSize maxSize = {.width = 1U << 14U, .height = 1U << 14U};

  uint32_t backgroundColor = 0xFFFFFFFF;

  WindowMode mode = WindowMode::Windowed;
  WindowOption option = static_cast<WindowOption>(~1U);
};
} // namespace nw