#pragma once
#include <cstddef>
#include <cstdint>

namespace nw
{
struct Color
{
  union
  {
    struct
    {
      std::byte r;
      std::byte g;
      std::byte b;
      std::byte a;
    };
    uint32_t i;
  };
};
} // namespace nw