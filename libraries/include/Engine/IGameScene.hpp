#ifndef IGAMESCENE_H
#define	IGAMESCENE_H

#include <memory>

#include <Engine/IScene.hpp>
#include <Engine/Window.hpp>
#include <Engine/ResourceManager.hpp>
#include <Engine/EventDispatcher.hpp>
#include <Engine/Renderer.hpp>
#include <Engine/Collider.hpp>
#include <Engine/GameObject.hpp>
#include <Engine/IGameObjectFactory.hpp>
#include <Engine/Ray.hpp>

#include <Engine/Event/CreateGameObjectEvent.hpp>

namespace Engine
{
	class IGameScene : public IScene
	{
	public:
		/**
		 * Constructor.
		 *
		 * @param window Shared pointer to the window.
		 * @param resourceManager Shared pointer to the resource manager.
		 * @param sceneStackEventDispatcher Shared pointer to the scene stack's
		 * event dispatcher.
		 */
		IGameScene(std::shared_ptr<Window> window, std::shared_ptr<ResourceManager> resourceManager,
			std::shared_ptr<EventDispatcher> sceneStackEventDispatcher);

		/**
		 * Destructor.
		 */
		virtual ~IGameScene();

		/**
		 * Updates the scene by updating all the Game Objects in the scene.
		 *
		 * @param deltaTime Time since last call (in seconds).
		 */
		virtual void Update(double deltaTime);

		/**
		 * Renders the game scene.
		 */
		virtual void Render();

		/**
		 * Draws the scene's user interface.
		 */
		virtual void OnDrawUI() = 0;

		/**
		 * Creates and broadcasts an event to all Game Objects in the scene.
		 *
		 * @param args... Event constructor arguments.
		 */
		template <typename EventType, typename... Arguments>
		void Broadcast(const Arguments... args)
		{
			for (auto iter = m_gameObjects.begin(); iter != m_gameObjects.end();
				++iter)
			{
				std::shared_ptr<GameObject> gameObject = iter->second;
				if (!gameObject->IsDead())
				{
					gameObject->DispatchEvent<EventType>(args...);
				}
			}
		}

		/**
		 * Returns a shared pointer to the Game Object currently in use as the
		 * scene camera.
		 *
		 * @return Pointer to the scene camera Game Object.
		 */
		std::shared_ptr<GameObject> GetCamera() const;

		/**
		 * Sets the specified Game Object as the current camera for the scene.
		 *
		 * @param gameObject Shared pointer to the Game Object which is to be
		 * used as the current scene camera.
		 */
		void SetCamera(std::shared_ptr<GameObject> gameObject);

		/**
		 * Returns the number of Game Objects in the scene.
		 *
		 * @return Number of Game Objects in the scene.
		 */
		unsigned int GetGameObjectCount() const;

		/**
		 * Returns a shared pointer to the Game Object specified by the provided
		 * index.
		 *
		 * @param index Index for the Game Object to retrieve.
		 * @return Shared pointer to the index-th Game Object in the scene.
		 */
		std::shared_ptr<GameObject> GetGameObject(unsigned int index) const;

		/**
		 * Creates a Game Object in the scene from the specified Game Object
		 * factory instance.
		 *
		 * @param factory Reference to the factory to use.
		 * @return Shared pointer to the created Game Object.
		 */
		std::shared_ptr<GameObject> CreateGameObject(const IGameObjectFactory& factory);

		/**
		 * Creates a new empty Game Object in the scene.
		 *
		 * @return Shared pointer to the created Game Object.
		 */
		std::shared_ptr<GameObject> CreateGameObject();

	protected:
		/**
		 * Returns a ray that passes through the specified screen point in the
		 * direction of the camera.
		 *
		 * @param screenPoint Screen point.
		 * @return Ray that passes through the screen point in the direction of
		 * the camera.
		 */
		Ray ScreenPointToRay(const glm::vec2& screenPoint) const;

	private:
		/**
		 * Game scene renderer.
		 * Renders game objects in the game scene.
		 */
		Renderer m_gameObjectRenderer;

		/**
		 * Game scene collider.
		 * Test for collisions between game objects in the game scene.
		 */
		Collider m_gameObjectCollider;

		/**
		 * Shared pointer to Game Objects that belong to the scene.
		 * These Game Object shared pointers are mapped by their Game Object
		 * identifier.
		 */
		std::map<GameObject::ID, std::shared_ptr<GameObject>> m_gameObjects;

		/**
		 * Shared pointer to the Game Object that represents the active scene
		 * camera.
		 */
		std::shared_ptr<GameObject> m_cameraGameObject;

		/**
		 * Subscription identifier for the CreateGameObjectEvent subscription.
		 */
		EventDispatcher::SubscriptionID m_createGameObjectSubscription;
	};
}

#endif
