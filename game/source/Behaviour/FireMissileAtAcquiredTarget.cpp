#include "Behaviour/FireMissileAtAcquiredTarget.hpp"

#include <Engine/Audio.hpp>

#include "Event/UpdateDestinationEvent.hpp"

namespace Behaviour
{
	FireMissileAtAcquiredTarget::FireMissileAtAcquiredTarget(std::shared_ptr<Engine::Window> window,
		std::shared_ptr<Engine::ResourceManager> resourceManager,
		std::shared_ptr<Engine::EventDispatcher> sceneEventDispatcher,
		std::shared_ptr<Engine::EventDispatcher> gameObjectEventDispatcher,
		std::weak_ptr<Engine::GameObject> gameObject,
		std::shared_ptr<Engine::Attribute::Transform> transformAttribute,
		std::shared_ptr<Engine::Attribute::Model> modelAttribute,
		std::shared_ptr<Attribute::Tags> tagsAttribute,
		std::shared_ptr<Engine::IGameObjectFactory> missileFactory,
		double delayBetweenLaunches)
	: IBehaviour(window, resourceManager, sceneEventDispatcher, gameObjectEventDispatcher, gameObject)
	, m_transformAttribute(transformAttribute)
	, m_modelAttribute(modelAttribute)
	, m_tagsAttribute(tagsAttribute)
	, m_missileFactory(missileFactory)
	, m_delayBetweenLaunches(delayBetweenLaunches)
	, m_secondsSinceLastLaunch(0.0)
	, m_state(State::ConstructingMissile)
	, m_missile(nullptr)
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
	}

	FireMissileAtAcquiredTarget::~FireMissileAtAcquiredTarget()
	{
		// Unsubscribe for target acquired events.
		GetGameObjectEventDispatcher()->Unsubscribe<Event::TargetAcquiredEvent>(m_targetAcquiredSubscription);
	}

	void FireMissileAtAcquiredTarget::Update(double deltaTime)
	{
		// Increment the launch timer.
		if (m_tagsAttribute->HasTag("Enabled"))
		{
			m_secondsSinceLastLaunch += deltaTime;
		}

		// Determine the starting scene position for the missile.
		const glm::vec3 startPosition = m_transformAttribute->GetPosition();

		// If we are currently constructing the missile, but we are within one
		// second of a possible launch, then create the missile and start
		// opening the silo door.
		if (m_state == State::ConstructingMissile)
		{
			if (m_delayBetweenLaunches - m_secondsSinceLastLaunch <= 1.0f)
			{
				m_state = State::OpeningDoor;
				GetSceneEventDispatcher()->Enqueue<Engine::Event::CreateGameObjectEvent>(
					m_missileFactory,
					[this, startPosition](std::shared_ptr<Engine::GameObject> missile)
					{
						// Store a shared pointer to the constructed missile.
						m_missile = missile;

						// Set the initial position for the missile.
						std::shared_ptr<Engine::Attribute::Transform> transform =
							missile->GetAttribute<Engine::Attribute::Transform>();
						transform->SetPosition(startPosition);
						transform->LookAt(startPosition + glm::vec3(0.0f, 1.0f, 0.0f));
					}
				);
			}
		}
		else if (m_state == State::OpeningDoor)
		{
			// If the opening door animation hasn't finished running, then
			// advance the animation.
			if (m_modelAttribute->GetCurrentAnimationTime() <= 1.0f)
			{
				m_modelAttribute->AdvanceAnimation(deltaTime);
			}
		}
		else if (m_state == State::ClosingDoor)
		{
			// If the closing door animation hasn't finished running, then
			// advance the animation.
			if (m_modelAttribute->GetCurrentAnimationTime() <= 2.0f)
			{
				m_modelAttribute->AdvanceAnimation(deltaTime);
			}
			else
			{
				// The closing door animation is complete. Start constructing
				// the next missile and restart the animation.
				m_state = State::ConstructingMissile;
				m_modelAttribute->RestartAnimation();
			}
		}

		// Check whether enough time has elapsed to launch rockets.
		if (m_secondsSinceLastLaunch > m_delayBetweenLaunches)
		{
			// Ensure that the target is still alive and that it has a Transform
			// attribute.
			if (m_target && !m_target->IsDead() && m_target->HasAttribute<Engine::Attribute::Transform>())
			{
				// Determine the ending scene position for the missile.
				std::shared_ptr<Engine::Attribute::Transform> targetTransform = m_target->GetAttribute<Engine::Attribute::Transform>();
				const glm::vec3 endPosition = targetTransform->GetPosition();

				// Publish an event to the missile so that it can calculate the
				// flight path and begin launching.
				m_missile->EnqueueEvent<Event::UpdateDestinationEvent>(endPosition);

				// Play the missile launch sound.
				Engine::Audio::GetInstance().Play(GetResourceManager()->GetAudio("resources/audio/MissileLaunch.wav"));

				// Remove the pointer to the launched missile.
				m_missile = nullptr;

				// Start the closing the silo door.
				m_state = State::ClosingDoor;

				// Reset launch timer.
				m_secondsSinceLastLaunch = 0.0;
			}
		}

		// Reset the target.
		m_target = nullptr;
	}
}
