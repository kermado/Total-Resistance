#include "Attribute/Resources.hpp"

namespace Attribute
{
	Resources::Resources(std::shared_ptr<Engine::Window> window,
		std::shared_ptr<Engine::ResourceManager> resourceManager,
		std::shared_ptr<Engine::EventDispatcher> sceneEventDispatcher,
		std::shared_ptr<Engine::EventDispatcher> gameObjectEventDispatcher,
		std::weak_ptr<Engine::GameObject> gameObject,
		unsigned int metal)
	: IAttribute(window, resourceManager, sceneEventDispatcher, gameObjectEventDispatcher, gameObject)
	, m_metal(metal)
	{
		// Nothing to do.
	}

	Resources::~Resources()
	{
		// Nothing to do.
	}

	unsigned int Resources::GetMetal() const
	{
		return m_metal;
	}
}
