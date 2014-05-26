#include "Behaviour/FireRocketsAtAcquiredTarget.hpp"

#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/vector_angle.hpp>

#include <Engine/Audio.hpp>
#include <Engine/Event/CreateGameObjectEvent.hpp>

namespace Behaviour
{
	FireRocketsAtAcquiredTarget::FireRocketsAtAcquiredTarget(std::shared_ptr<Engine::Window> window,
		std::shared_ptr<Engine::ResourceManager> resourceManager,
		std::shared_ptr<Engine::EventDispatcher> sceneEventDispatcher,
		std::shared_ptr<Engine::EventDispatcher> gameObjectEventDispatcher,
		std::weak_ptr<Engine::GameObject> gameObject,
		std::shared_ptr<Engine::Attribute::Transform> transformAttribute,
		std::shared_ptr<Engine::IGameObjectFactory> rocketFactory,
		double delayBetweenLaunches)
	: IBehaviour(window, resourceManager, sceneEventDispatcher, gameObjectEventDispatcher, gameObject)
	, m_transformAttribute(transformAttribute)
	, m_rocketFactory(rocketFactory)
	, m_delayBetweenLaunches(delayBetweenLaunches)
	, m_secondsSinceLastLaunch(0.0)
	, m_targetAcquiredSubscription(0)
	, m_target(nullptr)
	{
		// Subscribe for target acquired events.
		m_targetAcquiredSubscription = GetGameObjectEventDispatcher()->Subscribe<Event::TargetAcquiredEvent>(CALLBACK(FireRocketsAtAcquiredTarget::HandleTargetAcquiredEvent));
	}

	FireRocketsAtAcquiredTarget::~FireRocketsAtAcquiredTarget()
	{
		// Unsubscribe for target acquired events.
		GetGameObjectEventDispatcher()->Unsubscribe<Event::TargetAcquiredEvent>(m_targetAcquiredSubscription);
	}

	void FireRocketsAtAcquiredTarget::Update(double deltaTime)
	{
		// Increment the launch timer.
		m_secondsSinceLastLaunch += deltaTime;

		// Check whether enough time has elapsed to launch rockets.
		if (m_secondsSinceLastLaunch > m_delayBetweenLaunches)
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

					// Launch rockets if the absolute angle calculated is
					// sufficiently small.
					if (absoluteAngle < 5.0f)
					{
						// Create the 1st rocket.
						GetSceneEventDispatcher()->Enqueue<Engine::Event::CreateGameObjectEvent>(
							m_rocketFactory,
							[position, rotation, scale](std::shared_ptr<Engine::GameObject> rocket)
							{
								std::shared_ptr<Engine::Attribute::Transform> transform =
									rocket->GetAttribute<Engine::Attribute::Transform>();
								transform->SetPosition(position + rotation * glm::vec3(0.25f, 0.25f, 0.0f) * scale);
								transform->SetRotation(rotation);
							}
						);

						// Create the 2nd rocket.
						GetSceneEventDispatcher()->Enqueue<Engine::Event::CreateGameObjectEvent>(
							m_rocketFactory,
							[position, rotation, scale](std::shared_ptr<Engine::GameObject> rocket)
							{
								std::shared_ptr<Engine::Attribute::Transform> transform =
									rocket->GetAttribute<Engine::Attribute::Transform>();
								transform->SetPosition(position + rotation * glm::vec3(-0.25f, 0.25f, 0.0f) * scale);
								transform->SetRotation(rotation);
							}
						);

						// Play the rocket launch sound.
						Engine::Audio::GetInstance().Play(GetResourceManager()->GetAudio("resources/audio/RocketLaunch.wav"));

						// Reset the launch timer.
						m_secondsSinceLastLaunch = 0.0;
					}
				}
			}
		}

		// Reset the target.
		m_target = nullptr;
	}

	void FireRocketsAtAcquiredTarget::HandleTargetAcquiredEvent(const Event::TargetAcquiredEvent& event)
	{
		m_target = event.GetTarget();
	}
}
