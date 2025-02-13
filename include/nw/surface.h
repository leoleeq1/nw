#pragma once
#include "nw/color.h"
#include "nw/window_desc.h"

#include <cstdint>
#include <memory>

namespace nw
{
struct Surface
{
  Surface(WindowSize size)
    : pixels{std::make_unique<nw::Color[]>(size.width * size.height)},
      width{size.width},
      height{size.height}
  {
  }

  constexpr void Resize(WindowSize size)
  {
    pixels = std::make_unique<nw::Color[]>(size.width * size.height);
    width = size.width;
    height = size.height;
  }

  [[nodiscard]] constexpr size_t Length() const
  {
    return static_cast<size_t>(width) * static_cast<size_t>(height);
  }

  std::unique_ptr<Color[]> pixels;
  int32_t width;
  int32_t height;
};
} // namespace nw