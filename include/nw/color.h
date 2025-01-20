#pragma once
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <span>

namespace nw
{
struct Color
{
  explicit Color(uint32_t color) : u32(color) {}
  template<typename T>
    requires std::is_integral_v<T>
  explicit Color(std::span<T, 4> rgba)
    : r(static_cast<std::byte>(rgba[0])),
      g(static_cast<std::byte>(rgba[1])),
      b(static_cast<std::byte>(rgba[2])),
      a(static_cast<std::byte>(rgba[3]))
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