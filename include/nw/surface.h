#pragma once
#include "nw/color.h"
#include "nw/window_desc.h"

#include <cstdint>
#include <vector>

namespace nw
{
struct Surface
{
  explicit Surface(WindowSize size)
    : pixels(static_cast<std::size_t>(size.width)
             * static_cast<std::size_t>(size.height)),
      width{size.width},
      height{size.height}
  {
  }

  constexpr void Resize(WindowSize size)
  {
    pixels.resize(static_cast<std::size_t>(size.width)
                  * static_cast<std::size_t>(size.height));
    width = size.width;
    height = size.height;
  }

  [[nodiscard]] constexpr size_t Length() const
  {
    return static_cast<size_t>(width) * static_cast<std::size_t>(height);
  }

  std::vector<Color> pixels;
  int32_t width;
  int32_t height;
};
} // namespace nw