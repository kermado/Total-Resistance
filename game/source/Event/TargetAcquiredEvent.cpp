#include "Event/TargetAcquiredEvent.hpp"

namespace Event
{
	TargetAcquiredEvent::TargetAcquiredEvent(std::shared_ptr<Engine::GameObject> target)
	: IEvent()
	, m_target(target)
	{
		// Nothing to do.
	}

	TargetAcquiredEvent::~TargetAcquiredEvent()
	{
		// Nothing to do.
	}

	std::shared_ptr<Engine::GameObject> TargetAcquiredEvent::GetTarget() const
	{
		return m_target;
	}
}
