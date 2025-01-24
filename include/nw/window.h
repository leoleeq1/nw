#pragma once
#include "surface.h"
#include "window_desc.h"

#include <cstddef>
#include <cstdint>
#include <memory>

namespace nw
{
class EventBus;
class Window
{
 public:
  Window();
  ~Window();
  Window(const Window&) = delete;
  Window& operator=(const Window&) = delete;
  Window(Window&&) = delete;
  Window& operator=(Window&&) = delete;

  void Create(const WindowDesc& desc, EventBus *eventBus);
  bool Update();
  Surface CreateSurface(WindowSize size);
  Surface GetSurface() noexcept;
  void Present();

 private:
  struct Impl;
  std::unique_ptr<Impl> impl_;
};
} // namespace nw