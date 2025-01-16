#pragma once
#include "event.h"

namespace nw
{
class WindowCreatedEvent : public Event
{
 public:
  virtual EventType GetEventType() const override
  {
    return EventType::WindowCreated;
  }

  virtual EventCategory GetEventCategory() const override
  {
    return EventCategory::Application;
  }

  static constexpr EventType Type = EventType::WindowCreated;
};

class WindowClosedEvent : public Event
{
 public:
  virtual EventType GetEventType() const override
  {
    return EventType::WindowClosed;
  }

  virtual EventCategory GetEventCategory() const override
  {
    return EventCategory::Application;
  }

  static constexpr EventType Type = EventType::WindowClosed;
};

class WindowPaintEvent : public Event
{
 public:
  virtual EventType GetEventType() const override
  {
    return EventType::WindowPaint;
  }

  virtual EventCategory GetEventCategory() const override
  {
    return EventCategory::Application;
  }

  static constexpr EventType Type = EventType::WindowPaint;
};

class WindowMinimizedEvent : public Event
{
 public:
  virtual EventType GetEventType() const override
  {
    return EventType::WindowMinimized;
  }

  virtual EventCategory GetEventCategory() const override
  {
    return EventCategory::Application;
  }

  static constexpr EventType Type = EventType::WindowMinimized;
};

class WindowMaximizedEvent : public Event
{
 public:
  virtual EventType GetEventType() const override
  {
    return EventType::WindowMaximized;
  }

  virtual EventCategory GetEventCategory() const override
  {
    return EventCategory::Application;
  }

  static constexpr EventType Type = EventType::WindowMaximized;
};

class WindowResizeEvent : public Event
{
 public:
  [[nodiscard]] inline int32_t GetWidth() const { return width_; }
  [[nodiscard]] inline int32_t GetHeight() const { return height_; }

  virtual EventType GetEventType() const override
  {
    return EventType::WindowResize;
  }

  virtual EventCategory GetEventCategory() const override
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
  virtual EventType GetEventType() const override
  {
    return EventType::WindowActivated;
  }

  virtual EventCategory GetEventCategory() const override
  {
    return EventCategory::Application;
  }

  static constexpr EventType Type = EventType::WindowActivated;
};

class WindowDeactivatedEvent : public Event
{
 public:
  virtual EventType GetEventType() const override
  {
    return EventType::WindowDeactivated;
  }

  virtual EventCategory GetEventCategory() const override
  {
    return EventCategory::Application;
  }

  static constexpr EventType Type = EventType::WindowDeactivated;
};

class WindowSuspendedEvent : public Event
{
 public:
  virtual EventType GetEventType() const override
  {
    return EventType::WindowSuspended;
  }

  virtual EventCategory GetEventCategory() const override
  {
    return EventCategory::Application;
  }

  static constexpr EventType Type = EventType::WindowSuspended;
};

class WindowResumedEvent : public Event
{
 public:
  virtual EventType GetEventType() const override
  {
    return EventType::WindowResumed;
  }

  virtual EventCategory GetEventCategory() const override
  {
    return EventCategory::Application;
  }

  static constexpr EventType Type = EventType::WindowResumed;
};
} // namespace nw