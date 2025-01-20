#pragma once

#include <cstddef>
#include <cstdint>

namespace nw
{
struct BitmapDesc
{
  size_t bitCount = 8ULL * 4ULL; // number of bits per pixel
  int32_t width = 32;
  int32_t height = 32;
};

class Bitmap
{
 public:
  explicit Bitmap(BitmapDesc desc, void **pixels) : desc_(desc), pixels_(pixels)
  {
  }

  [[nodiscard]] BitmapDesc GetDescriptor() const noexcept { return desc_; }
  [[nodiscard]] void **GetPixels() const noexcept { return pixels_; }

  constexpr static std::size_t CalcBufferSize(BitmapDesc desc)
  {
    return (((desc.width * desc.bitCount + 15) >> 4U) << 1U) * desc.height;
  }

 private:
  BitmapDesc desc_;
  void **pixels_; // ptr to 1d pixel array
};
} // namespace nw