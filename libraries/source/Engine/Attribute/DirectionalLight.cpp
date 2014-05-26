#include <Engine/Attribute/DirectionalLight.hpp>

namespace Engine
{
	namespace Attribute
	{
		DirectionalLight::DirectionalLight(std::shared_ptr<Window> window,
			std::shared_ptr<ResourceManager> resourceManager,
			std::shared_ptr<EventDispatcher> sceneEventDispatcher,
			std::shared_ptr<EventDispatcher> gameObjectEventDispatcher,
			std::weak_ptr<GameObject> gameObject,
			const glm::vec3& color,
			float intensity)
		: IAttribute(window, resourceManager, sceneEventDispatcher, gameObjectEventDispatcher, gameObject)
		, m_color(color)
		, m_intensity(intensity)
		{
			// Nothing to do.
		}

		DirectionalLight::~DirectionalLight()
		{
			// Nothing to do.
		}

		const glm::vec3& DirectionalLight::GetColor() const
		{
			return m_color;
		}

		float DirectionalLight::GetIntensity() const
		{
			return m_intensity;
		}
	}
}
