#include <Engine/Event/AncestorTransformRotatedEvent.hpp>

namespace Engine
{
	namespace Event
	{
		AncestorTransformRotatedEvent::AncestorTransformRotatedEvent(const glm::quat& delta)
		: IEvent()
		, m_delta(delta)
		{
			// Nothing to do.
		}

		AncestorTransformRotatedEvent::~AncestorTransformRotatedEvent()
		{
			// Nothing to do.
		}

		const glm::quat& AncestorTransformRotatedEvent::GetDelta() const
		{
			return m_delta;
		}
	}
}
