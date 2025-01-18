#pragma once
#include "../window_desc.h"
#include "event.h"

namespace nw
{
class WindowCreatedEvent : public Event
{
 public:
  [[nodiscard]] EventType GetEventType() const override
  {
    return EventType::WindowCreated;
  }

  [[nodiscard]] EventCategory GetEventCategory() const override
  {
    return EventCategory::Application;
  }

  static constexpr EventType Type = EventType::WindowCreated;
};

class WindowClosedEvent : public Event
{
 public:
  [[nodiscard]] EventType GetEventType() const override
  {
    return EventType::WindowClosed;
  }

  [[nodiscard]] EventCategory GetEventCategory() const override
  {
    return EventCategory::Application;
  }

  static constexpr EventType Type = EventType::WindowClosed;
};

class WindowPaintEvent : public Event
{
 public:
  [[nodiscard]] EventType GetEventType() const override
  {
    return EventType::WindowPaint;
  }

  [[nodiscard]] EventCategory GetEventCategory() const override
  {
    return EventCategory::Application;
  }

  static constexpr EventType Type = EventType::WindowPaint;
};

class WindowMinimizedEvent : public Event
{
 public:
  [[nodiscard]] EventType GetEventType() const override
  {
    return EventType::WindowMinimized;
  }

  [[nodiscard]] EventCategory GetEventCategory() const override
  {
    return EventCategory::Application;
  }

  static constexpr EventType Type = EventType::WindowMinimized;
};

class WindowMaximizedEvent : public Event
{
 public:
  [[nodiscard]] EventType GetEventType() const override
  {
    return EventType::WindowMaximized;
  }

  [[nodiscard]] EventCategory GetEventCategory() const override
  {
    return EventCategory::Application;
  }

  static constexpr EventType Type = EventType::WindowMaximized;
};

class WindowResizeEvent : public Event
{
 public:
  explicit WindowResizeEvent(WindowSize size) : size_(size) {}

  [[nodiscard]] int32_t GetWidth() const { return size_.width; }
  [[nodiscard]] int32_t GetHeight() const { return size_.height; }

  [[nodiscard]] EventType GetEventType() const override
  {
    return EventType::WindowResize;
  }

  [[nodiscard]] EventCategory GetEventCategory() const override
  {
    return EventCategory::Application;
  }

  static constexpr EventType Type = EventType::WindowResize;

 private:
  WindowSize size_;
};

class WindowActivatedEvent : public Event
{
 public:
  [[nodiscard]] EventType GetEventType() const override
  {
    return EventType::WindowActivated;
  }

  [[nodiscard]] EventCategory GetEventCategory() const override
  {
    return EventCategory::Application;
  }

  static constexpr EventType Type = EventType::WindowActivated;
};

class WindowDeactivatedEvent : public Event
{
 public:
  [[nodiscard]] EventType GetEventType() const override
  {
    return EventType::WindowDeactivated;
  }

  [[nodiscard]] EventCategory GetEventCategory() const override
  {
    return EventCategory::Application;
  }

  static constexpr EventType Type = EventType::WindowDeactivated;
};

class WindowSuspendedEvent : public Event
{
 public:
  [[nodiscard]] EventType GetEventType() const override
  {
    return EventType::WindowSuspended;
  }

  [[nodiscard]] EventCategory GetEventCategory() const override
  {
    return EventCategory::Application;
  }

  static constexpr EventType Type = EventType::WindowSuspended;
};

class WindowResumedEvent : public Event
{
 public:
  [[nodiscard]] EventType GetEventType() const override
  {
    return EventType::WindowResumed;
  }

  [[nodiscard]] EventCategory GetEventCategory() const override
  {
    return EventCategory::Application;
  }

  static constexpr EventType Type = EventType::WindowResumed;
};
} // namespace nw