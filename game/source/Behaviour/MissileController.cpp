#include "Behaviour/MissileController.hpp"

#include <Engine/Event/CreateGameObjectEvent.hpp>
#include <Engine/Event/DestroyGameObjectEvent.hpp>

#include "ExhaustFactory.hpp"
#include "Event/UpdateDestinationEvent.hpp"

namespace Behaviour
{
	MissileController::MissileController(std::shared_ptr<Engine::Window> window,
		std::shared_ptr<Engine::ResourceManager> resourceManager,
		std::shared_ptr<Engine::EventDispatcher> sceneEventDispatcher,
		std::shared_ptr<Engine::EventDispatcher> gameObjectEventDispatcher,
		std::weak_ptr<Engine::GameObject> gameObject,
		std::shared_ptr<Engine::Attribute::Transform> transformAttribute,
		float speed,
		float maxAltitude)
	: IBehaviour(window, resourceManager, sceneEventDispatcher, gameObjectEventDispatcher, gameObject)
	, m_transformAttribute(transformAttribute)
	, m_speed(speed)
	, m_maxAltitude(maxAltitude)
	, m_flightPath(nullptr)
	, m_time(0.0)
	, m_exhaustFactory(std::make_shared<ExhaustFactory>())
	, m_updateDestinationSubscription(0)
	{
		// Subscribe for update destination events.
		m_updateDestinationSubscription = GetGameObjectEventDispatcher()->Subscribe<Event::UpdateDestinationEvent>(
			[this](const Event::UpdateDestinationEvent& event)
			{
				// Get the current position.
				const glm::vec3 currentPosition = m_transformAttribute->GetPosition();

				// Get the final position.
				const glm::vec3 finalPosition = event.GetDestination();

				// Get the first (boost phase) mid-point position.
				const glm::vec3 boostPosition = glm::vec3(currentPosition.x, m_maxAltitude, currentPosition.z);

				// Get the second (terminal phase) mid-point position.
				const glm::vec3 terminalPosition = glm::vec3(finalPosition.x, m_maxAltitude, finalPosition.z);

				// Update the flight path.
				m_flightPath = std::unique_ptr<Engine::BezierCurve<3, glm::vec3>>(new Engine::BezierCurve<3, glm::vec3>(
					currentPosition,
					boostPosition,
					finalPosition + glm::vec3(0.0f, -10.0f, 0.0f) * m_transformAttribute->GetScale()
				));

				// Reset the timer.
				m_time = 0.0;
			}
		);
	}

	MissileController::~MissileController()
	{
		// Unsubscribe for update destination events.
		GetGameObjectEventDispatcher()->Unsubscribe<Event::UpdateDestinationEvent>(m_updateDestinationSubscription);
	}

	void MissileController::Update(double deltaTime)
	{
		// Increment the timer.
		m_time += deltaTime;

		// We can only control the missile towards the target destination if
		// the flight path has been computed.
		if (m_flightPath)
		{
			// Approximate the length of the flight path (i.e. the total
			// distance to be travelled) in scene space.
			float length = 0.0f;
			for (unsigned int i = 1; i <= 100; ++i)
			{
				length += glm::distance(
					m_flightPath->GetPointAtTime(static_cast<float>(i) / 100.0f),
					m_flightPath->GetPointAtTime(static_cast<float>(i - 1) / 100.0f)
				);
			}

			// Determine the distance along the flight path travelled.
			const float distance = m_speed * m_time;

			// Determine the fraction of the flight path covered.
			const float fraction = distance / length;

			// Check whether there is still some distance to be travelled.
			if (fraction <= 1)
			{
				// Get the current scene position of the missile.
				const glm::vec3 currentPosition = m_transformAttribute->GetPosition();

				// Get the precice point on the flight path for the Game Object
				// to be placed.
				const glm::vec3 point = m_flightPath->GetPointAtTime(fraction);

				// Rotate the Game Object's transform to "look at" the point.
				m_transformAttribute->LookAt(point);

				// Set the position of the Game Object's transform.
				m_transformAttribute->SetPosition(point);

				// Emit an exhaust cloud if we are in the boost phase.
				if (point.y > currentPosition.y && currentPosition.y <= 0.4f * m_maxAltitude)
				{
					GetSceneEventDispatcher()->Enqueue<Engine::Event::CreateGameObjectEvent>(
						m_exhaustFactory,
						[this](std::shared_ptr<Engine::GameObject> cloud)
						{
							std::shared_ptr<Engine::Attribute::Transform> transform =
								cloud->GetAttribute<Engine::Attribute::Transform>();
							transform->SetPosition(m_transformAttribute->TransformPoint(glm::vec3(0.0f, 0.0f, 1.75f)));
						}
					);
				}
			}
			else
			{
				// Request to destroy the Game Object.
				GetGameObjectEventDispatcher()->Enqueue<Engine::Event::DestroyGameObjectEvent>();
			}
		}
	}
}
