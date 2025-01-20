#pragma once
#include <cstddef>
#include <cstdint>
#include <span>

namespace nw
{
struct Color
{
  explicit Color(uint32_t color) : u32(color) {}
  explicit Color(std::span<std::byte, 4> rgba)
    : r(rgba[0]),
      g(rgba[1]),
      b(rgba[2]),
      a(rgba[3])
  {
  }

  union
  {
    uint32_t u32;
    struct
    {
      std::byte r;
      std::byte g;
      std::byte b;
      std::byte a;
    };
  };
};
} // namespace nw