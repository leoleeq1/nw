#pragma once
#include "window_desc.h"

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

 private:
  struct Impl;
  std::unique_ptr<Impl> impl_;
};
} // namespace nw