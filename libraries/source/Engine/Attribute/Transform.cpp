#include <Engine/Attribute/Transform.hpp>

#include <glm/gtx/norm.hpp>

#include <Engine/GameObject.hpp>

#include <Engine/Event/AncestorTransformTranslatedEvent.hpp>
#include <Engine/Event/AncestorTransformRotatedEvent.hpp>
#include <Engine/Event/AncestorTransformScaledEvent.hpp>
#include <Engine/Event/ChildGameObjectAttachedEvent.hpp>

namespace Engine
{
	namespace Attribute
	{
		Transform::Transform(std::shared_ptr<Window> window,
			std::shared_ptr<ResourceManager> resourceManager,
			std::shared_ptr<EventDispatcher> sceneEventDispatcher,
			std::shared_ptr<EventDispatcher> gameObjectEventDispatcher,
			std::weak_ptr<GameObject> gameObject)
		: IAttribute(window, resourceManager, sceneEventDispatcher, gameObjectEventDispatcher, gameObject)
		, m_localPosition(0.0f, 0.0f, 0.0f)
		, m_localRotation()
		, m_localScale(1.0f, 1.0f, 1.0f)
		, m_position(m_localPosition)
		, m_rotation(m_localRotation)
		, m_scale(m_localScale)
		, m_localTransformationMatrix()
		, m_transformationMatrix()
		, m_localTransformationMatrixDirty(true)
		, m_transformationMatrixDirty(true)
		{
			// Subscribe for ancestor transform translated events.
			m_ancestorTranslatedSubscription = GetGameObjectEventDispatcher()->Subscribe<Event::AncestorTransformTranslatedEvent>(
				[this](const Event::AncestorTransformTranslatedEvent& event) {
					m_position += event.GetDelta();
					m_transformationMatrixDirty = true;
				}
			);

			// Subscribe for ancestor transform rotated events.
			m_ancestorRotatedSubscription = GetGameObjectEventDispatcher()->Subscribe<Event::AncestorTransformRotatedEvent>(
				[this](const Event::AncestorTransformRotatedEvent& event) {
					m_rotation = event.GetDelta() * m_rotation;
					m_transformationMatrixDirty = true;
				}
			);

			// Subscribe for ancestor transform scaled events.
			m_ancestorScaledSubscription = GetGameObjectEventDispatcher()->Subscribe<Event::AncestorTransformScaledEvent>(
				[this](const Event::AncestorTransformScaledEvent& event) {
					m_scale *= event.GetDelta();
					m_transformationMatrixDirty = true;
				}
			);

			// Subscribe for child game object attached events.
			m_childAttachedSubscription = GetGameObjectEventDispatcher()->Subscribe<Event::ChildGameObjectAttachedEvent>(
				[this](const Event::ChildGameObjectAttachedEvent& event)
				{
					std::shared_ptr<GameObject> child = event.GetChild();
					if (child->HasAttribute<Transform>())
					{
						child->BroadcastDispatch<Event::AncestorTransformTranslatedEvent>(m_position);
						child->BroadcastDispatch<Event::AncestorTransformRotatedEvent>(m_rotation);
						child->BroadcastDispatch<Event::AncestorTransformScaledEvent>(m_scale);
					}
					else
					{
						child->BroadcastEnqueue<Event::AncestorTransformTranslatedEvent>(m_position);
						child->BroadcastEnqueue<Event::AncestorTransformRotatedEvent>(m_rotation);
						child->BroadcastEnqueue<Event::AncestorTransformScaledEvent>(m_scale);
					}
				}
			);
		}

		Transform::~Transform()
		{
			// Unsubscribe for ancestor transform translated events.
			GetGameObjectEventDispatcher()->Unsubscribe<Event::AncestorTransformTranslatedEvent>(m_ancestorTranslatedSubscription);

			// Unsubscribe for ancestor transform rotated events.
			GetGameObjectEventDispatcher()->Unsubscribe<Event::AncestorTransformRotatedEvent>(m_ancestorRotatedSubscription);

			// Unsubscribe for ancestor transform scaled events.
			GetGameObjectEventDispatcher()->Unsubscribe<Event::AncestorTransformScaledEvent>(m_ancestorScaledSubscription);

			// Unsubscribe for child game object attached events.
			GetGameObjectEventDispatcher()->Unsubscribe<Event::ChildGameObjectAttachedEvent>(m_childAttachedSubscription);
		}

		const glm::vec3& Transform::GetLocalPosition() const
		{
			return m_localPosition;
		}

		const glm::quat& Transform::GetLocalRotation() const
		{
			return m_localRotation;
		}

		const glm::vec3& Transform::GetLocalScale() const
		{
			return m_localScale;
		}

		const glm::vec3& Transform::GetPosition() const
		{
			return m_position;
		}

		const glm::quat& Transform::GetRotation() const
		{
			return m_rotation;
		}

		const glm::vec3& Transform::GetScale() const
		{
			return m_scale;
		}

		const glm::mat4& Transform::GetLocalTransformationMatrix() const
		{
			if (m_localTransformationMatrixDirty)
			{
				const_cast<Transform*>(this)->CleanLocalTransformationMatrix();
			}

			return m_localTransformationMatrix;
		}

		const glm::mat4& Transform::GetTransformationMatrix() const
		{
			if (m_transformationMatrixDirty)
			{
				const_cast<Transform*>(this)->CleanTransformationMatrix();
			}

			return m_transformationMatrix;
		}

		glm::vec3 Transform::GetForward() const
		{
			const glm::vec3 localForwardDirection(0.0f, 0.0f, -1.0f);
			return glm::normalize(m_rotation * localForwardDirection);
		}

		void Transform::SetLocalPosition(const glm::vec3& position)
		{
			const glm::vec3 delta = position - m_localPosition;
			m_localPosition = position;
			m_position += delta;
			m_localTransformationMatrixDirty = true;
			m_transformationMatrixDirty = true;

			// Recursively broadcast translation delta to all descended Game
			// Objects.
			if (std::shared_ptr<GameObject> gameObject = GetGameObject().lock())
			{
				gameObject->DescendantBroadcastDispatch<Event::AncestorTransformTranslatedEvent>(delta);
			}
		}

		void Transform::SetLocalRotation(const glm::quat& rotation)
		{
			const glm::quat delta = glm::inverse(m_localRotation) * rotation;
			m_localRotation = rotation;
			m_rotation = delta * m_rotation;
			m_localTransformationMatrixDirty = true;
			m_transformationMatrixDirty = true;

			// Recursively broadcast rotation delta to all descended Game
			// Objects.
			if (std::shared_ptr<GameObject> gameObject = GetGameObject().lock())
			{
				gameObject->DescendantBroadcastDispatch<Event::AncestorTransformRotatedEvent>(delta);
			}
		}

		void Transform::SetLocalScale(const glm::vec3& scale)
		{
			const glm::vec3 delta = scale / m_localScale;
			m_localScale = scale;
			m_scale *= delta;
			m_localTransformationMatrixDirty = true;
			m_transformationMatrixDirty = true;

			// Recursively broadcast scaling delta to all descended Game
			// Objects.
			if (std::shared_ptr<GameObject> gameObject = GetGameObject().lock())
			{
				gameObject->DescendantBroadcastDispatch<Event::AncestorTransformScaledEvent>(delta);
			}
		}

		void Transform::SetLocalScale(float scale)
		{
			SetLocalScale(glm::vec3(scale, scale, scale));
		}

		void Transform::SetPosition(const glm::vec3& position)
		{
			const glm::vec3 delta = position - m_position;
			m_position = position;
			m_localPosition += delta;
			m_localTransformationMatrixDirty = true;
			m_transformationMatrixDirty = true;

			// Recursively broadcast translation delta to all descended Game
			// Objects.
			if (std::shared_ptr<GameObject> gameObject = GetGameObject().lock())
			{
				gameObject->DescendantBroadcastDispatch<Event::AncestorTransformTranslatedEvent>(delta);
			}
		}

		void Transform::SetRotation(const glm::quat& rotation)
		{
			const glm::quat delta = rotation * glm::inverse(m_rotation);
			m_rotation = rotation;
			m_localRotation = delta * m_localRotation;
			m_localTransformationMatrixDirty = true;
			m_transformationMatrixDirty = true;

			// Recursively broadcast rotation delta to all descended Game
			// Objects.
			if (std::shared_ptr<GameObject> gameObject = GetGameObject().lock())
			{
				gameObject->DescendantBroadcastDispatch<Event::AncestorTransformRotatedEvent>(delta);
			}
		}

		void Transform::SetScale(const glm::vec3& scale)
		{
			const glm::vec3 delta = scale / m_scale;
			m_scale = scale;
			m_localScale *= delta;
			m_localTransformationMatrixDirty = true;
			m_transformationMatrixDirty = true;

			// Recursively broadcast scaling delta to all descended Game
			// Objects.
			if (std::shared_ptr<GameObject> gameObject = GetGameObject().lock())
			{
				gameObject->DescendantBroadcastDispatch<Event::AncestorTransformScaledEvent>(delta);
			}
		}

		void Transform::SetScale(float scale)
		{
			SetScale(glm::vec3(scale, scale, scale));
		}

		void Transform::Translate(const glm::vec3& translation)
		{
			m_localPosition += translation;
			m_position += translation;
			m_localTransformationMatrixDirty = true;
			m_transformationMatrixDirty = true;

			// Recursively broadcast translation delta to all descended Game
			// Objects.
			if (std::shared_ptr<GameObject> gameObject = GetGameObject().lock())
			{
				gameObject->DescendantBroadcastDispatch<Event::AncestorTransformTranslatedEvent>(translation);
			}
		}

		void Transform::Rotate(const glm::quat& rotation)
		{
			m_localRotation = rotation * m_localRotation;
			m_rotation = rotation * m_rotation;
			m_localTransformationMatrixDirty = true;
			m_transformationMatrixDirty = true;

			// Recursively broadcast rotation delta to all descended Game
			// Objects.
			if (std::shared_ptr<GameObject> gameObject = GetGameObject().lock())
			{
				gameObject->DescendantBroadcastDispatch<Event::AncestorTransformRotatedEvent>(rotation);
			}
		}

		void Transform::Scale(const glm::vec3& scale)
		{
			m_localScale *= scale;
			m_scale *= scale;
			m_localTransformationMatrixDirty = true;
			m_transformationMatrixDirty = true;

			// Recursively broadcast scaling delta to all descended Game
			// Objects.
			if (std::shared_ptr<GameObject> gameObject = GetGameObject().lock())
			{
				gameObject->DescendantBroadcastDispatch<Event::AncestorTransformScaledEvent>(scale);
			}
		}

		void Transform::Scale(float scale)
		{
			Scale(glm::vec3(scale, scale, scale));
		}

		void Transform::LookAt(const glm::vec3& point, const glm::vec3& up)
		{
			const glm::vec3 desiredForward = glm::normalize(point - m_position);
			Rotate(GetRotationToFaceDirection(desiredForward, up));
		}

		glm::vec3 Transform::TransformPoint(const glm::vec3& point) const
		{
			return glm::vec3(GetLocalTransformationMatrix() * glm::vec4(point, 1.0f));
		}

		glm::quat Transform::GetRotationToFaceDirection(const glm::vec3& direction) const
		{
			// Normalise the desired forward direction.
			const glm::vec3 desiredForward = glm::normalize(direction);

			// Get the current forward direction.
			const glm::vec3 currentForward = GetForward();

			// Return the quaternion that rotates the transform such that its
			// forward direction points in the target direction.
			const glm::vec3 w1 = glm::cross(currentForward, desiredForward);
		    return glm::normalize(glm::quat(1.0f + glm::dot(currentForward, desiredForward), w1.x, w1.y, w1.z));
		}

		glm::quat Transform::GetRotationToFaceDirection(const glm::vec3& direction, const glm::vec3& up) const
		{
			// Normalise the desired forward direction.
			const glm::vec3 desiredForward = glm::normalize(direction);

			// Get the current forward direction.
			const glm::vec3 currentForward = GetForward();

			// Calculate the quaternion that rotates the transform such that its
			// forward direction points in the target direction.
			const glm::vec3 w1 = glm::cross(currentForward, desiredForward);
			const glm::quat q1 = glm::normalize(glm::quat(1.0f + glm::dot(currentForward, desiredForward), w1.x, w1.y, w1.z));

			// We can't preserve the upward direction if the transform should
			// point in the upward or downward direction.
			if (glm::distance2(desiredForward, up) < 0.01f ||
				glm::distance2(desiredForward, -up) < 0.01f)
			{
				return q1;
			}

			// Calculate the transform's up direction after applying the
			// rotation calculated above.
			const glm::vec3 currentUp = glm::normalize((q1 * m_rotation) * glm::vec3(0.0f, 1.0f, 0.0f));

			// Calculate the vector that points to the right.
			const glm::vec3 right = glm::normalize(glm::cross(desiredForward, up));

			// Recompute the desired up direction.
			const glm::vec3 desiredUp = glm::normalize(glm::cross(right, desiredForward));

			// Calculate the quaternion that rotates the transform such that
			// its up direction points in the specified up direction.
			const glm::vec3 w2 = glm::cross(currentUp, desiredUp);
			const glm::quat q2 = glm::normalize(glm::quat(1.0f + glm::dot(currentUp, desiredUp), w2.x, w2.y, w2.z));

			// Return the combined rotation.
			// Note: There is no need to normalize the result since both q1 and
			// q2 are unit quaternions.
			return q2 * q1;
		}

		void Transform::CleanLocalTransformationMatrix()
		{
			m_localTransformationMatrix = glm::translate(m_localPosition) * glm::toMat4(m_localRotation) * glm::scale(m_localScale);
			m_localTransformationMatrixDirty = false;
		}

		void Transform::CleanTransformationMatrix()
		{
			m_transformationMatrix = glm::translate(m_position) * glm::toMat4(m_rotation) * glm::scale(m_scale);
			m_transformationMatrixDirty = false;
		}
	}
}
