#include "Event/UpdateDestinationEvent.hpp"

namespace Event
{
	UpdateDestinationEvent::UpdateDestinationEvent(const glm::vec3& destination)
	: IEvent()
	, m_destination(destination)
	{
		// Nothing to do.
	}

	UpdateDestinationEvent::~UpdateDestinationEvent()
	{
		// Nothing to do.
	}

	const glm::vec3& UpdateDestinationEvent::GetDestination() const
	{
		return m_destination;
	}
}
