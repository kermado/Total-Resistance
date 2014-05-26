#ifndef BOUNDINGSPHERE_H
#define	BOUNDINGSPHERE_H

#include <string>
#include <memory>

#include <Engine/Attribute/IAttribute.hpp>
#include <Engine/Window.hpp>
#include <Engine/ResourceManager.hpp>
#include <Engine/EventDispatcher.hpp>

namespace Engine
{
	namespace Attribute
	{
		class BoundingSphere : public IAttribute
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
			 * @param sphereRadius Radius for the sphere.
			 * @param tag Tag name for the bounding sphere, which can be used
			 * to dentify the bounding object a Game Object has intersected.
			 */
			BoundingSphere(std::shared_ptr<Window> window,
				std::shared_ptr<ResourceManager> resourceManager,
				std::shared_ptr<EventDispatcher> sceneEventDispatcher,
				std::shared_ptr<EventDispatcher> gameObjectEventDispatcher,
				std::weak_ptr<GameObject> gameObject,
				const float sphereRadius,
				const std::string tag = "");

			/**
			 * Destructor.
			 */
			~BoundingSphere();

			/**
			 * Returns the radius of the bounding sphere.
			 *
			 * @return Sphere radius.
			 */
			float GetRadius() const;

			/**
			 * Returns the tag for the bounding sphere.
			 *
			 * @return Bounding sphere tag.
			 */
			std::string GetTag() const;

		private:
			/**
			 * Radius for the sphere.
			 */
			float m_radius;

			/**
			 * Tag identifier.
			 */
			std::string m_tag;
		};
	}
}

#endif
