#include <Engine/Event/AncestorTransformTranslatedEvent.hpp>

namespace Engine
{
	namespace Event
	{
		AncestorTransformTranslatedEvent::AncestorTransformTranslatedEvent(const glm::vec3& delta)
		: IEvent()
		, m_delta(delta)
		{
			// Nothing to do.
		}

		AncestorTransformTranslatedEvent::~AncestorTransformTranslatedEvent()
		{
			// Nothing to do.
		}

		const glm::vec3& AncestorTransformTranslatedEvent::GetDelta() const
		{
			return m_delta;
		}
	}
}
