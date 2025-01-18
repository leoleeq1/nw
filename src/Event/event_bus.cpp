#include "nw/Event/event_bus.h"
#include "nw/Event/ApplicationEvent.h"
#include "nw/Event/event.h"

#include <cassert>
#include <vector>

namespace nw
{
void EventBus::Push(std::unique_ptr<Event> e)
{
  events_.push_back(std::move(e));
}

void EventBus::Dispatch()
{
  for (auto& e : events_)
  {
    auto it = subscribers_.find(e->GetEventType());
    if (it == subscribers_.end())
    {
      continue;
    }

    auto& [_, handlers] = *it;
    for (auto& handler : handlers)
    {
      handler->Exec(*e);
    }
  }
}

void EventBus::SendEvent(std::unique_ptr<Event> e)
{
  auto it = subscribers_.find(e->GetEventType());
  if (it == subscribers_.end())
  {
    return;
  }

  auto& [_, handlers] = *it;
  for (auto& handler : handlers)
  {
    handler->Exec(*e);
  }
}

void EventBus::Subscribe(
  EventType type, std::unique_ptr<EventHandlerBase> handler)
{
  auto it = subscribers_.find(type);
  if (it == subscribers_.end())
  {
    subscribers_[type].push_back(std::move(handler));
    return;
  }

  auto& [_, handlers] = *it;
  for (auto& i : handlers)
  {
    assert(i->GetId() != handler->GetId());
  }
  handlers.push_back(std::move(handler));
}

void EventBus::Unsubscribe(EventType type, size_t handlerId)
{
  auto it = subscribers_.find(type);
  if (it == subscribers_.end())
  {
    return;
  }

  auto& [_, handlers] = *it;
  for (auto handler = handlers.begin(); handler != handlers.end(); ++handler)
  {
    if ((handler->get())->GetId() == handlerId)
    {
      handlers.erase(handler);
      return;
    }
  }
}
} // namespace nw