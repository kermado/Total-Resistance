#ifndef CREATEGAMEOBJECTEVENT_H
#define CREATEGAMEOBJECTEVENT_H

#include <memory>
#include <functional>

#include <Engine/Event/IEvent.hpp>
#include <Engine/IGameObjectFactory.hpp>

namespace Engine
{
	namespace Event
	{
		class CreateGameObjectEvent : public Engine::Event::IEvent
		{
		public:
			/**
			 * Constructor.
			 *
			 * @param callback The callback function that should be executed
			 * once the Game Object has been created.
			 */
			CreateGameObjectEvent(std::function<void(std::shared_ptr<GameObject>)> callback);

			/**
			 * Constructor.
			 *
			 * @param factory Shared pointer to the Game Object factory that
			 * should be used to create the Game Object.
			 */
			CreateGameObjectEvent(std::shared_ptr<const Engine::IGameObjectFactory> factory);

			/**
			 * Constructor.
			 *
			 * @param factory Shared pointer to the Game Object factory that
			 * should be used to create the Game Object.
			 * @param callback The callback function that should be executed
			 * once the Game Object has been created.
			 */
			CreateGameObjectEvent(std::shared_ptr<const Engine::IGameObjectFactory> factory,
				std::function<void(std::shared_ptr<GameObject>)> callback);

			/**
			 * Destructor.
			 */
			~CreateGameObjectEvent();

			/**
			 * Returns a shared pointer to the Game Object Factory that should
			 * be used to create the Game Object.
			 *
			 * @return The Game Object Factory (read only).
			 */
			std::shared_ptr<const Engine::IGameObjectFactory> GetFactory() const;

			/**
			 * Executes the callback function, passing to it the specified
			 * shared pointer to the created Game Object.
			 *
			 * @param gameObject Shared pointer to the created Game Object.
			 */
			void ExecuteCallback(std::shared_ptr<GameObject> gameObject) const;

		private:
			/**
			 * The Game Object Factory that is to be used to create the Game
			 * Object.
			 */
			std::shared_ptr<const Engine::IGameObjectFactory> m_factory;

			/**
			 * The callback function that is to be executed once the Game Object
			 * has been created.
			 */
			std::function<void(std::shared_ptr<GameObject>)> m_callback;
		};
	}
}

#endif
