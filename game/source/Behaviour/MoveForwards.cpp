#include "Behaviour/MoveForwards.hpp"

namespace Behaviour
{
	MoveForwards::MoveForwards(std::shared_ptr<Engine::Window> window,
		std::shared_ptr<Engine::ResourceManager> resourceManager,
		std::shared_ptr<Engine::EventDispatcher> sceneEventDispatcher,
		std::shared_ptr<Engine::EventDispatcher> gameObjectEventDispatcher,
		std::weak_ptr<Engine::GameObject> gameObject,
		std::shared_ptr<Engine::Attribute::Transform> transformAttribute,
		float speed)
	: IBehaviour(window, resourceManager, sceneEventDispatcher, gameObjectEventDispatcher, gameObject)
	, m_transformAttribute(transformAttribute)
	, m_speed(speed)
	{
		// Nothing to do.
	}

	MoveForwards::~MoveForwards()
	{
		// Nothing to do.
	}

	void MoveForwards::Update(double deltaTime)
	{
		// The behaviour should not have been passed a nullptr.
		assert(m_transformAttribute);

		// Translate the game object's transform in its forward
		// direction.
		const glm::vec3 forward = m_transformAttribute->GetForward();
		const glm::vec3 translation = forward * m_speed * static_cast<float>(deltaTime);
		m_transformAttribute->Translate(translation);
	}
}
