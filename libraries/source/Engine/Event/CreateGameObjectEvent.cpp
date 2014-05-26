#include <Engine/Event/CreateGameObjectEvent.hpp>

namespace Engine
{
	namespace Event
	{
		CreateGameObjectEvent::CreateGameObjectEvent(std::function<void(std::shared_ptr<GameObject>)> callback)
		: m_factory(nullptr)
		, m_callback(callback)
		{
			// Nothing to do.
		}

		CreateGameObjectEvent::CreateGameObjectEvent(std::shared_ptr<const Engine::IGameObjectFactory> factory)
		: m_factory(factory)
		, m_callback([](std::shared_ptr<GameObject>){})
		{
			// Nothing to do.
		}

		CreateGameObjectEvent::CreateGameObjectEvent(std::shared_ptr<const Engine::IGameObjectFactory> factory,
			std::function<void(std::shared_ptr<GameObject>)> callback)
		: m_factory(factory)
		, m_callback(callback)
		{
			// Nothing to do.
		}

		CreateGameObjectEvent::~CreateGameObjectEvent()
		{
			// Nothing to do.
		}

		std::shared_ptr<const Engine::IGameObjectFactory> CreateGameObjectEvent::GetFactory() const
		{
			return m_factory;
		}

		void CreateGameObjectEvent::ExecuteCallback(std::shared_ptr<GameObject> gameObject) const
		{
			m_callback(gameObject);
		}
	}
}
