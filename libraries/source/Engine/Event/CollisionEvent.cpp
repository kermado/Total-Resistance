#include <Engine/Event/CollisionEvent.hpp>

namespace Engine
{
	namespace Event
	{
		CollisionEvent::CollisionEvent(std::shared_ptr<GameObject> otherGameObject,
				const std::string otherBoundingGeometryTag,
				const std::string thisBoundingGeometryTag)
		: IEvent()
		, m_otherGameObject(otherGameObject)
		, m_otherBoundingGeometryTag(otherBoundingGeometryTag)
		, m_thisBoundingGeometryTag(thisBoundingGeometryTag)
		{
			// Nothing to do.
		}

		CollisionEvent::~CollisionEvent()
		{
			// Nothing to do.
		}

		std::shared_ptr<GameObject> CollisionEvent::GetOtherGameObject() const
		{
			return m_otherGameObject;
		}

		std::string CollisionEvent::GetOtherBoundingGeometryTag() const
		{
			return m_otherBoundingGeometryTag;
		}

		std::string CollisionEvent::GetThisBoundingGeometryTag() const
		{
			return m_thisBoundingGeometryTag;
		}
	}
}
