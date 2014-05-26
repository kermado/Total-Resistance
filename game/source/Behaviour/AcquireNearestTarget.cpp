#include "Behaviour/AcquireNearestTarget.hpp"

#include <vector>

#include <glm/gtx/norm.hpp>

#include "Event/TargetAcquiredEvent.hpp"

namespace Behaviour
{
	AcquireNearestTarget::AcquireNearestTarget(std::shared_ptr<Engine::Window> window,
		std::shared_ptr<Engine::ResourceManager> resourceManager,
		std::shared_ptr<Engine::EventDispatcher> sceneEventDispatcher,
		std::shared_ptr<Engine::EventDispatcher> gameObjectEventDispatcher,
		std::weak_ptr<Engine::GameObject> gameObject,
		std::shared_ptr<Engine::Attribute::Transform> transformAttribute,
		std::shared_ptr<Attribute::Tags> tagsAttribute,
		std::string tag)
	: IBehaviour(window, resourceManager, sceneEventDispatcher, gameObjectEventDispatcher, gameObject)
	, m_transformAttribute(transformAttribute)
	, m_tagsAttribute(tagsAttribute)
	, m_tag(tag)
	, m_collisionSubscription(0)
	, m_targetsWithinRange()
	{
		// Subscribe for collision events.
		m_collisionSubscription = GetGameObjectEventDispatcher()->Subscribe<Engine::Event::CollisionEvent>(CALLBACK(AcquireNearestTarget::HandleCollisionEvent));
	}

	AcquireNearestTarget::~AcquireNearestTarget()
	{
		// Unsubscribe to receive collision events.
		GetGameObjectEventDispatcher()->Unsubscribe<Engine::Event::CollisionEvent>(m_collisionSubscription);
	}

	void AcquireNearestTarget::Update(double deltaTime)
	{
		// Get the scene position of this Game Object.
		const glm::vec3 position = m_transformAttribute->GetPosition();

		// We can only proceed if there were any targets within range.
		if (!m_targetsWithinRange.empty())
		{
			// Determine the closest target Game Object within range.
			std::shared_ptr<Engine::GameObject> nearestTarget = nullptr;
			float nearestTargetDistance;
			for (std::shared_ptr<Engine::GameObject> target : m_targetsWithinRange)
			{
				if (target->HasAttribute<Engine::Attribute::Transform>())
				{
					std::shared_ptr<Engine::Attribute::Transform> transform =
						target->GetAttribute<Engine::Attribute::Transform>();
					const glm::vec3 targetPosition = transform->GetPosition();
					const float targetDistance = glm::distance2(position, targetPosition);
					if (!nearestTarget || targetDistance < nearestTargetDistance)
					{
						nearestTargetDistance = targetDistance;
						nearestTarget = target;
					}
				}
			}

			// Raise a TargetAcquiredEvent, so that other behaviours can take
			// suitable action, if the Game Object has an "Enabled" tag.
			if (m_tagsAttribute->HasTag("Enabled"))
			{
				GetGameObjectEventDispatcher()->Enqueue<Event::TargetAcquiredEvent>(nearestTarget);
			}

			// Clear the list of targets within range so that the list can be
			// repopulated immediately before the next update.
			m_targetsWithinRange.clear();
		}
	}

	void AcquireNearestTarget::HandleCollisionEvent(const Engine::Event::CollisionEvent& event)
	{
		std::shared_ptr<Engine::GameObject> target = event.GetOtherGameObject();

		if (!target->IsDead() &&
			target->HasAttribute<Attribute::Tags>() &&
			target->GetAttribute<Attribute::Tags>()->HasTag(m_tag))
		{

			m_targetsWithinRange.push_back(target);
		}
	}
}
