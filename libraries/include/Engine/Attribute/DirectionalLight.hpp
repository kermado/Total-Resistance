#ifndef LIGHT_H
#define	LIGHT_H

#include <memory>

#include <glm/glm.hpp>

#include <Engine/Attribute/IAttribute.hpp>
#include <Engine/Window.hpp>
#include <Engine/ResourceManager.hpp>
#include <Engine/EventDispatcher.hpp>

namespace Engine
{
	namespace Attribute
	{
		class DirectionalLight : public IAttribute
		{
		public:
			/**
			 * Constructor.
			 *
			 * @param window Shared pointer to the window.
			 * @param resourceManager Shared pointer to the resource manager.
			 * @param sceneEventDispatcher Shared pointer to the scene's Event
			 * Dispatcher.
			 * @param gameObjectEventDispatcher Shared pointer to the Game
			 * Object's Event Dispatcher.
			 * @param gameObject Weak pointer to the Game Object that the
			 * attribute is attached to.
			 * @param color Color of the light.
			 * @param intensity Intensity of the light.
			 */
			DirectionalLight(std::shared_ptr<Window> window,
				std::shared_ptr<ResourceManager> resourceManager,
				std::shared_ptr<EventDispatcher> sceneEventDispatcher,
				std::shared_ptr<EventDispatcher> gameObjectEventDispatcher,
				std::weak_ptr<GameObject> gameObject,
				const glm::vec3& color = glm::vec3(1.0f, 1.0f, 1.0f),
				float intensity = 1.0f);

			/**
			 * Destructor.
			 */
			virtual ~DirectionalLight();

			/**
			 * Returns the light color.
			 *
			 * @return Light color.
			 */
			const glm::vec3& GetColor() const;

			/**
			 * Returns the light intensity.
			 *
			 * @return Light intensity.
			 */
			float GetIntensity() const;

		private:
			/**
			 * Light color (RGB).
			 */
			glm::vec3 m_color;

			/**
			 * Light intensity.
			 */
			float m_intensity;
		};
	}
};

#endif
