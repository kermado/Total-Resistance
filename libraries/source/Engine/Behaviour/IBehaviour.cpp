#include <Engine/Behaviour/IBehaviour.hpp>

namespace Engine
{
	namespace Behaviour
	{
		IBehaviour::IBehaviour(std::shared_ptr<Window> window,
				std::shared_ptr<ResourceManager> resourceManager,
				std::shared_ptr<EventDispatcher> sceneEventDispatcher,
				std::shared_ptr<EventDispatcher> gameObjectEventDispatcher,
				std::weak_ptr<GameObject> gameObject)
		: m_window(window)
		, m_resourceManager(resourceManager)
		, m_sceneEventDispatcher(sceneEventDispatcher)
		, m_gameObjectEventDispatcher(gameObjectEventDispatcher)
		, m_gameObject(gameObject)
		{
			// Nothing to do.
		}

		IBehaviour::~IBehaviour()
		{
			// Nothing to do.
		}

		std::shared_ptr<Window> IBehaviour::GetWindow() const
		{
			return m_window;
		}

		std::shared_ptr<ResourceManager> IBehaviour::GetResourceManager() const
		{
			return m_resourceManager;
		}

		std::shared_ptr<EventDispatcher> IBehaviour::GetSceneEventDispatcher() const
		{
			return m_sceneEventDispatcher;
		}

		std::shared_ptr<EventDispatcher> IBehaviour::GetGameObjectEventDispatcher() const
		{
			return m_gameObjectEventDispatcher;
		}

		std::weak_ptr<GameObject> IBehaviour::GetGameObject() const
		{
			return m_gameObject;
		}
	}
}
