#include "Behaviour/FireLaserAtAcquiredTarget.hpp"

#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/vector_angle.hpp>

#include <Engine/IGameScene.hpp>

#include <Engine/Event/CreateGameObjectEvent.hpp>
#include <Engine/Event/DestroyGameObjectEvent.hpp>

#include <Engine/Attribute/Transform.hpp>
#include <Engine/Attribute/ShaderProgram.hpp>
#include <Engine/Attribute/Model.hpp>

#include "Event/InflictDamageEvent.hpp"
#include "LaserFactory.hpp"

namespace Behaviour
{
	FireLaserAtAcquiredTarget::FireLaserAtAcquiredTarget(std::shared_ptr<Engine::Window> window,
		std::shared_ptr<Engine::ResourceManager> resourceManager,
		std::shared_ptr<Engine::EventDispatcher> sceneEventDispatcher,
		std::shared_ptr<Engine::EventDispatcher> gameObjectEventDispatcher,
		std::weak_ptr<Engine::GameObject> gameObject,
		std::shared_ptr<Engine::Attribute::Transform> transformAttribute)
	: IBehaviour(window, resourceManager, sceneEventDispatcher, gameObjectEventDispatcher, gameObject)
	, m_transformAttribute(transformAttribute)
	, m_laser(nullptr)
	, m_targetAcquiredSubscription(0)
	, m_target(nullptr)
	{
		// Subscribe for target acquired events.
		m_targetAcquiredSubscription = GetGameObjectEventDispatcher()->Subscribe<Event::TargetAcquiredEvent>(
			[this](const Event::TargetAcquiredEvent& event)
			{
				m_target = event.GetTarget();
			}
		);

		// Create the child Game Object for the laser beam.
		GetGameObjectEventDispatcher()->Enqueue<Engine::Event::CreateGameObjectEvent>(
			std::make_shared<LaserFactory>(),
			[this](std::shared_ptr<Engine::GameObject> gameObject)
			{
				// Store a pointer to the created laser Game Object.
				m_laser = gameObject;

				// Make the laser invisible.
				m_laser->GetAttribute<Engine::Attribute::Model>()->SetVisible(false);
			}
		);
	}

	FireLaserAtAcquiredTarget::~FireLaserAtAcquiredTarget()
	{
		// Unsubscribe for target acquired events.
		GetGameObjectEventDispatcher()->Unsubscribe<Event::TargetAcquiredEvent>(m_targetAcquiredSubscription);
	}

	void FireLaserAtAcquiredTarget::Update(double deltaTime)
	{
		// Ensure that the target is still alive and that it has a Transform
		// attribute.
		if (m_target && !m_target->IsDead() && m_target->HasAttribute<Engine::Attribute::Transform>())
		{
			const glm::vec3 position = m_transformAttribute->GetPosition();
			const glm::quat rotation = m_transformAttribute->GetRotation();
			const glm::vec3 scale = m_transformAttribute->GetScale();

			// Get the target's Transform attribute.
			std::shared_ptr<Engine::Attribute::Transform> targetTransform = m_target->GetAttribute<Engine::Attribute::Transform>();
			const glm::vec3 targetPosition = targetTransform->GetPosition();

			// Ensure that the target is not in the same position so as to
			// avoid division by zero.
			if (glm::distance2(glm::vec2(position.x, position.z), glm::vec2(targetPosition.x, targetPosition.z)) > 0.001f)
			{
				// Calculate the absolute angle between the direction that this
				// Game Object is facing and the direction to the target.
				const glm::vec3 targetDirection = glm::normalize(targetPosition - position);
				float absoluteAngle = glm::angle(m_transformAttribute->GetForward(), targetDirection);

				// If the angle is sufficiently small then start the laser and
				// deal damage to the target.
				if (absoluteAngle < 15.0f)
				{
					StartLaser();
					m_target->BroadcastEnqueue<Event::InflictDamageEvent>(1.0f * deltaTime);
				}
				else
				{
					StopLaser();
				}
			}

			// Scale the laser such that one end touches the origin of
			// the target's transform.
			if (m_laser)
			{
				const float distance = glm::distance(position, targetPosition);
				std::shared_ptr<Engine::Attribute::Transform> laserTransform =
					m_laser->GetAttribute<Engine::Attribute::Transform>();
				glm::vec3 newScale = laserTransform->GetScale();
				newScale.z = distance;
				laserTransform->SetScale(newScale);
			}
		}
		else
		{
			StopLaser();
		}

		// Reset the target.
		m_target = nullptr;
	}

	void FireLaserAtAcquiredTarget::StartLaser()
	{
		if (m_laser)
		{
			m_laser->GetAttribute<Engine::Attribute::Model>()->SetVisible(true);
		}
	}

	void FireLaserAtAcquiredTarget::StopLaser()
	{
		if (m_laser)
		{
			m_laser->GetAttribute<Engine::Attribute::Model>()->SetVisible(false);
		}
	}
}
