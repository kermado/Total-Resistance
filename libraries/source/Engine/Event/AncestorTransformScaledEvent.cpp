#include <Engine/Event/AncestorTransformScaledEvent.hpp>

namespace Engine
{
	namespace Event
	{
		AncestorTransformScaledEvent::AncestorTransformScaledEvent(const glm::vec3& delta)
		: IEvent()
		, m_delta(delta)
		{
			// Nothing to do.
		}

		AncestorTransformScaledEvent::~AncestorTransformScaledEvent()
		{
			// Nothing to do.
		}

		const glm::vec3& AncestorTransformScaledEvent::GetDelta() const
		{
			return m_delta;
		}
	}
}
