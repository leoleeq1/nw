#pragma once
#include <concepts>
#include <cstdint>
#include <span>

namespace nw
{
template<typename T>
concept ByteConvertible = std::convertible_to<T, uint8_t>;

struct Color
{
  Color() noexcept {}
  explicit Color(uint32_t color) noexcept : u32(color) {}
  explicit Color(std::span<uint8_t, 4> argb) noexcept
    : u32(*reinterpret_cast<uint32_t *>(&argb))
  {
  }
  template<ByteConvertible... Args> explicit Color(Args... args)
  {
    static_assert(sizeof...(Args) == 4, "Four arguments required!");
    int index = 0;
    const std::span<uint8_t> color{reinterpret_cast<uint8_t *>(&u32), 4};
    ((color[index++] = static_cast<uint8_t>(args)), ...);
  }

  static Color white;
  static Color black;
  static Color red;
  static Color blue;
  static Color green;

  union
  {
    uint32_t u32{};
    struct
    {
      uint8_t b;
      uint8_t g;
      uint8_t r;
      uint8_t a;
    };
  };
};
} // namespace nw