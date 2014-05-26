#include "Behaviour/FaceAcquiredTarget.hpp"

#include <glm/gtx/norm.hpp>
#include <iostream>

namespace Behaviour
{
	FaceAcquiredTarget::FaceAcquiredTarget(std::shared_ptr<Engine::Window> window,
		std::shared_ptr<Engine::ResourceManager> resourceManager,
		std::shared_ptr<Engine::EventDispatcher> sceneEventDispatcher,
		std::shared_ptr<Engine::EventDispatcher> gameObjectEventDispatcher,
		std::weak_ptr<Engine::GameObject> gameObject,
		std::shared_ptr<Engine::Attribute::Transform> transformAttribute,
		float rotationSpeed)
	: IBehaviour(window, resourceManager, sceneEventDispatcher, gameObjectEventDispatcher, gameObject)
	, m_transformAttribute(transformAttribute)
	, m_targetAcquiredSubscription(0)
	, m_target(nullptr)
	, m_rotationSpeed(rotationSpeed)
	{
		// Subscribe for target acquired events.
		m_targetAcquiredSubscription = GetGameObjectEventDispatcher()->Subscribe<Event::TargetAcquiredEvent>(CALLBACK(FaceAcquiredTarget::HandleTargetAcquiredEvent));
	}

	FaceAcquiredTarget::~FaceAcquiredTarget()
	{
		// Unsubscribe to receive target acquired events.
		GetGameObjectEventDispatcher()->Unsubscribe<Event::TargetAcquiredEvent>(m_targetAcquiredSubscription);
	}

	void FaceAcquiredTarget::Update(double deltaTime)
	{
		if (m_target && m_target->HasAttribute<Engine::Attribute::Transform>())
		{
			// Get the scene position of this Game Object.
			const glm::vec3 position = m_transformAttribute->GetPosition();

			// Get the target's scene position.
			const std::shared_ptr<Engine::Attribute::Transform> targetTransform =
				m_target->GetAttribute<Engine::Attribute::Transform>();
			const glm::vec3 targetPosition = targetTransform->GetPosition();

			// Calculate the vector from this Game Object to the target.
			const glm::vec3 vectorToTarget = targetPosition - position;

			// Look at the target.
			// Note that we first ensure that the target is not straight above
			// or below us. This ensures that we don't try to divide by zero.
			if (glm::distance2(glm::vec2(position.x, position.z), glm::vec2(targetPosition.x, targetPosition.z)) > 0.01f)
			{
				// Get the current forward direction.
				const glm::vec3 currentForward = m_transformAttribute->GetForward();

				// Get the current scene space orientation.
				const glm::quat currentOrientation = m_transformAttribute->GetRotation();

				// Calculate the rotation that, when applied, would cause the
				// transform to look at the target.
				const glm::quat lookRotation = m_transformAttribute->GetRotationToFaceDirection(vectorToTarget, glm::vec3(0.0f, 1.0f, 0.0f));

				// Calculate the orientation that looks at the target,
				const glm::quat finalOrientation = lookRotation * currentOrientation;

				// Calculate the cosine of the angle between the current
				// orientation and final orientation.
				const float cosTheta = glm::dot(currentOrientation, finalOrientation);

				// If the angle is sufficiently small, set the orientation
				// to the final orientation.
				if (std::abs(cosTheta) > 0.999f)
				{
					m_transformAttribute->SetRotation(finalOrientation);
				}
				else
				{
					// Calculate the mix factor.
					float mixFactor = (m_rotationSpeed * deltaTime) / std::acos(cosTheta);

					// Perform SLERP to get the scene space orientation to be set in
					// this frame and set the orientation.
					const glm::quat orientation = glm::mix(currentOrientation, finalOrientation, mixFactor);
					m_transformAttribute->SetRotation(orientation);
				}

			}
		}

		// Reset the target.
		m_target = nullptr;
	}

	void FaceAcquiredTarget::HandleTargetAcquiredEvent(const Event::TargetAcquiredEvent& event)
	{
		m_target = event.GetTarget();
	}
}
