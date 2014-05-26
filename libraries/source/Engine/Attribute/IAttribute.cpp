#include <Engine/Attribute/IAttribute.hpp>

namespace Engine
{
	namespace Attribute
	{
		IAttribute::IAttribute(std::shared_ptr<Window> window,
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

		IAttribute::~IAttribute()
		{
			// Nothing to do.
		}

		std::shared_ptr<Window> IAttribute::GetWindow() const
		{
			return m_window;
		}

		std::shared_ptr<ResourceManager> IAttribute::GetResourceManager() const
		{
			return m_resourceManager;
		}

		std::shared_ptr<EventDispatcher> IAttribute::GetSceneEventDispatcher() const
		{
			return m_sceneEventDispatcher;
		}

		std::shared_ptr<EventDispatcher> IAttribute::GetGameObjectEventDispatcher() const
		{
			return m_gameObjectEventDispatcher;
		}

		std::weak_ptr<GameObject> IAttribute::GetGameObject() const
		{
			return m_gameObject;
		}
	}
}
