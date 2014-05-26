#include "Behaviour/Animate.hpp"

namespace Behaviour
{
	Animate::Animate(std::shared_ptr<Engine::Window> window,
		std::shared_ptr<Engine::ResourceManager> resourceManager,
		std::shared_ptr<Engine::EventDispatcher> sceneEventDispatcher,
		std::shared_ptr<Engine::EventDispatcher> gameObjectEventDispatcher,
		std::weak_ptr<Engine::GameObject> gameObject,
		std::shared_ptr<Engine::Attribute::Model> modelAttribute)
	: IBehaviour(window, resourceManager, sceneEventDispatcher, gameObjectEventDispatcher, gameObject)
	, m_modelAttribute(modelAttribute)
	{
		// Nothing to do.
	}

	Animate::~Animate()
	{
		// Nothing to do.
	}

	void Animate::Update(double deltaTime)
	{
		m_modelAttribute->AdvanceAnimation(deltaTime);
	}
}
