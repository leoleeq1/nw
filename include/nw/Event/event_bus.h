#pragma once
#include "event.h"

#include <cstdint>
#include <memory>
#include <unordered_map>
#include <vector>

namespace nw
{
class EventBus
{
 public:
  EventBus() = default;
  EventBus(const EventBus&) = delete;
  EventBus& operator=(const EventBus&) = delete;
  ~EventBus() = default;

  void Push(std::unique_ptr<Event>&& e);
  void Dispatch();
  void Subscribe(EventType type, std::unique_ptr<EventHandlerBase>&& handler);
  void Unsubscribe(EventType type, size_t handlerId);

 private:
  std::vector<std::unique_ptr<Event>> events_;
  std::unordered_map<EventType, std::vector<std::unique_ptr<EventHandlerBase>>>
    subscribers_;
};
} // namespace nw