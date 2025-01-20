#pragma once
#include "color.h"

#include <cstdint>

namespace nw
{
struct Surface
{
  [[nodiscard]] constexpr size_t Length() const
  {
    return static_cast<size_t>(width) * static_cast<size_t>(height);
  }

  Color *pixels;
  int32_t width;
  int32_t height;
};
} // namespace nw