#ifndef NAVIGATETOCELL_H
#define NAVIGATETOCELL_H

#include <Engine/Behaviour/IBehaviour.hpp>
#include <Engine/Attribute/Transform.hpp>
#include <Engine/EventDispatcher.hpp>

#include "PlayingSurface.hpp"
#include "Event/PlayingSurfaceModifiedEvent.hpp"

namespace Behaviour
{
	class NavigateToCell : public Engine::Behaviour::IBehaviour
	{
	public:
		/**
		 * Constructor.
		 *
		 * @param window Shared pointer to the window.
		 * @param resourceManager Shared pointer to the resource manager.
		 * @param sceneEventDispatcher Shared pointer to the scene's event
		 * dispatcher.
		 * @param gameObjectEventDispatcher Shared pointer to the game
		 * object's event dispatcher.
		 * @param gameObject Weak pointer to the Game Object that the
		 * attribute is attached to.
		 * @param transformAttribute Shared pointer to the game object's
		 * transform attribute.
		 * @param playingSurface The playing surface over which to navigate.
		 * @param target Target cell to navigate towards.
		 * @param speed Movement speed.
		 */
		NavigateToCell(std::shared_ptr<Engine::Window> window,
			std::shared_ptr<Engine::ResourceManager> resourceManager,
			std::shared_ptr<Engine::EventDispatcher> sceneEventDispatcher,
			std::shared_ptr<Engine::EventDispatcher> gameObjectEventDispatcher,
			std::weak_ptr<Engine::GameObject> gameObject,
			std::shared_ptr<Engine::Attribute::Transform> transformAttribute,
			const PlayingSurface& playingSurface,
			const Cell& target,
			float speed);

		/**
		 * Destructor.
		 */
		~NavigateToCell();

		/**
		 * Moves the game object towards the target cell by following a path.
		 */
		virtual void Update(double deltaTime);

		/**
		 * Handles PlayingSurfaceModifiedEvents.
		 *
		 * @param event Reference to the event.
		 */
		void HandlePlayingSurfaceModifiedEvent(const Event::PlayingSurfaceModifiedEvent& event);

	private:
		/**
		 * Shared pointer to the game object's transform attribute.
		 */
		std::shared_ptr<Engine::Attribute::Transform> m_transformAttribute;

		/**
		 * Copy of the playing surface over which to navigate.
		 */
		PlayingSurface m_playingSurface;

		/**
		 * The target cell.
		 */
		Cell m_target;

		/**
		 * The movement speed.
		 */
		float m_speed;

		/**
		 * The shortest path starting from the game object's current cell to the
		 * target cell.
		 */
		Path m_shortestPath;

		/**
		 * Subscription identifier for the PlayingSurfaceModifiedEvent
		 * subscription.
		 */
		Engine::EventDispatcher::SubscriptionID m_playingSurfaceModifiedSubscription;
	};
}

#endif
