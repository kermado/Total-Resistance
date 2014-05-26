#ifndef RESOURCES_H
#define RESOURCES_H

#include <Engine/Attribute/IAttribute.hpp>
#include <Engine/Window.hpp>
#include <Engine/ResourceManager.hpp>

namespace Attribute
{
	class Resources : public Engine::Attribute::IAttribute
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
		 * @param metal Amount of metal carried by the Game Object.
		 */
		Resources(std::shared_ptr<Engine::Window> window,
			std::shared_ptr<Engine::ResourceManager> resourceManager,
			std::shared_ptr<Engine::EventDispatcher> sceneEventDispatcher,
			std::shared_ptr<Engine::EventDispatcher> gameObjectEventDispatcher,
			std::weak_ptr<Engine::GameObject> gameObject,
			unsigned int metal);

		/**
		 * Destructor.
		 */
		~Resources();

		/**
		 * Returns the amount of metal carried by the Game Object.
		 *
		 * @return Amount of metal.
		 */
		unsigned int GetMetal() const;

	private:
		/**
		 * The amount of metal carried by the Game Object.
		 */
		unsigned int m_metal;
	};
}

#endif
