#include <Engine/Attribute/BoundingSphere.hpp>

namespace Engine
{
	namespace Attribute
	{
		BoundingSphere::BoundingSphere(std::shared_ptr<Window> window,
				std::shared_ptr<ResourceManager> resourceManager,
				std::shared_ptr<EventDispatcher> sceneEventDispatcher,
				std::shared_ptr<EventDispatcher> gameObjectEventDispatcher,
				std::weak_ptr<GameObject> gameObject,
				const float sphereRadius,
				const std::string tag)
		: IAttribute(window, resourceManager, sceneEventDispatcher, gameObjectEventDispatcher, gameObject)
		, m_radius(sphereRadius)
		, m_tag(tag)
		{
			// Nothing to do.
		}

		BoundingSphere::~BoundingSphere()
		{
			// Nothing to do.
		}

		float BoundingSphere::GetRadius() const
		{
			return m_radius;
		}

		std::string BoundingSphere::GetTag() const
		{
			return m_tag;
		}
	}
}
