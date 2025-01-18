#pragma once
#include "event.h"

namespace nw
{
class WindowCreatedEvent : public Event
{
 public:
  EventType GetEventType() const override { return EventType::WindowCreated; }

  EventCategory GetEventCategory() const override
  {
    return EventCategory::Application;
  }

  static constexpr EventType Type = EventType::WindowCreated;
};

class WindowClosedEvent : public Event
{
 public:
  EventType GetEventType() const override { return EventType::WindowClosed; }

  EventCategory GetEventCategory() const override
  {
    return EventCategory::Application;
  }

  static constexpr EventType Type = EventType::WindowClosed;
};

class WindowPaintEvent : public Event
{
 public:
  EventType GetEventType() const override { return EventType::WindowPaint; }

  EventCategory GetEventCategory() const override
  {
    return EventCategory::Application;
  }

  static constexpr EventType Type = EventType::WindowPaint;
};

class WindowMinimizedEvent : public Event
{
 public:
  EventType GetEventType() const override { return EventType::WindowMinimized; }

  EventCategory GetEventCategory() const override
  {
    return EventCategory::Application;
  }

  static constexpr EventType Type = EventType::WindowMinimized;
};

class WindowMaximizedEvent : public Event
{
 public:
  EventType GetEventType() const override { return EventType::WindowMaximized; }

  EventCategory GetEventCategory() const override
  {
    return EventCategory::Application;
  }

  static constexpr EventType Type = EventType::WindowMaximized;
};

class WindowResizeEvent : public Event
{
 public:
  [[nodiscard]] int32_t GetWidth() const { return width_; }
  [[nodiscard]] int32_t GetHeight() const { return height_; }

  EventType GetEventType() const override { return EventType::WindowResize; }

  EventCategory GetEventCategory() const override
  {
    return EventCategory::Application;
  }

  static constexpr EventType Type = EventType::WindowResize;

 private:
  int32_t width_;
  int32_t height_;
};

class WindowActivatedEvent : public Event
{
 public:
  EventType GetEventType() const override { return EventType::WindowActivated; }

  EventCategory GetEventCategory() const override
  {
    return EventCategory::Application;
  }

  static constexpr EventType Type = EventType::WindowActivated;
};

class WindowDeactivatedEvent : public Event
{
 public:
  EventType GetEventType() const override
  {
    return EventType::WindowDeactivated;
  }

  EventCategory GetEventCategory() const override
  {
    return EventCategory::Application;
  }

  static constexpr EventType Type = EventType::WindowDeactivated;
};

class WindowSuspendedEvent : public Event
{
 public:
  EventType GetEventType() const override { return EventType::WindowSuspended; }

  EventCategory GetEventCategory() const override
  {
    return EventCategory::Application;
  }

  static constexpr EventType Type = EventType::WindowSuspended;
};

class WindowResumedEvent : public Event
{
 public:
  EventType GetEventType() const override { return EventType::WindowResumed; }

  EventCategory GetEventCategory() const override
  {
    return EventCategory::Application;
  }

  static constexpr EventType Type = EventType::WindowResumed;
};
} // namespace nw