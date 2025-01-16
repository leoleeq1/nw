#pragma once
#include <cstdint>
#include <functional>

namespace nw
{
enum class EventType : uint8_t
{
  None = 0,
  WindowCreated,
  WindowClosed,
  WindowPaint,
  WindowMinimized,
  WindowMaximized,
  WindowResize,
  WindowActivated,
  WindowDeactivated,
  WindowSuspended,
  WindowResumed,
  KeyDown,
  KeyUp,
  KeyRepeat,
  MouseMove,
  MouseButtonDown,
  MouseButtonUp,
  MouseScrolled,
};

enum class EventCategory : uint8_t
{
  None = 0,
  Application = 1 << 0,
  Keyboard = 1 << 1,
  Mouse = 1 << 2,

  Input = Keyboard | Mouse,
};

class Event
{
 public:
  virtual ~Event() = default;
  [[nodiscard]] virtual EventType GetEventType() const = 0;
  [[nodiscard]] virtual EventCategory GetEventCategory() const = 0;

 protected:
  bool handled_ = false;

  friend class EventHandlerBase;
};

class EventHandlerBase
{
 public:
  virtual ~EventHandlerBase() = default;

  void Exec(Event& e) const { Call(e); }

  [[nodiscard]] size_t GetId() const { return GetIdInternal(); }

 private:
  virtual void Call(Event& e) const = 0;
  [[nodiscard]] virtual size_t GetIdInternal() const = 0;
};

template<typename T> class EventHandler : public EventHandlerBase
{
  using HandleFn = std::function<bool(T&)>;

 public:
  explicit EventHandler(const HandleFn& f) : handleFn_(f) {}

 private:
  void Call(Event& e) const override
  {
    if (e.GetEventType() == T::Type)
    {
      handleFn_(static_cast<T&>(e));
    }
  }

  [[nodiscard]] size_t GetIdInternal() const override
  {
    return reinterpret_cast<size_t>(handleFn_.target_type().hash_code());
  }

  HandleFn handleFn_;
};
} // namespace nw