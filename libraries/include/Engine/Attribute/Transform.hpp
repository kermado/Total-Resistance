#ifndef TRANSFORM_H
#define	TRANSFORM_H

#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>

#include <Engine/Attribute/IAttribute.hpp>
#include <Engine/Window.hpp>
#include <Engine/ResourceManager.hpp>
#include <Engine/EventDispatcher.hpp>

namespace Engine
{
	namespace Attribute
	{
		class Transform : public IAttribute
		{
		public:
			/**
			 * Constructor.
			 */
			Transform(std::shared_ptr<Window> window,
				std::shared_ptr<ResourceManager> resourceManager,
				std::shared_ptr<EventDispatcher> sceneEventDispatcher,
				std::shared_ptr<EventDispatcher> gameObjectEventDispatcher,
				std::weak_ptr<GameObject> gameObject);

			/**
			 * Destructor.
			 */
			~Transform();

			/**
			 * Returns the position of the transform relative to the parent Game
			 * Object's transform.
			 *
			 * @return Local position.
			 */
			const glm::vec3& GetLocalPosition() const;

			/**
			 * Returns the rotation of the transform relative to the parent Game
			 * Object's transform.
			 *
			 * @return Local rotation quaternion.
			 */
			const glm::quat& GetLocalRotation() const;

			/**
			 * Returns the scale of the transform relative to the parent Game
			 * Object's transform.
			 *
			 * @return Local component scale vector.
			 */
			const glm::vec3& GetLocalScale() const;

			/**
			 * Returns the position of the transform in scene space.
			 *
			 * @return Point in scene space corresponding to the transform's
			 * local origin.
			 */
			const glm::vec3& GetPosition() const;

			/**
			 * Returns the rotation of the transform in scene space.
			 *
			 * @return Rotation quaternion in scene space.
			 */
			const glm::quat& GetRotation() const;

			/**
			 * Returns the scale of the transform in scene space.
			 *
			 * @return Component scale in scene space.
			 */
			const glm::vec3& GetScale() const;

			/**
			 * Returns the transform's transformation matrix relative to the
			 * parent Game Object's transform.
			 *
			 * @return Transformation matrix in local space.
			 */
			const glm::mat4& GetLocalTransformationMatrix() const;

			/**
			 * Returns the transform's scene space transformation matrix.
			 *
			 * @return Transformation matrix in scene space.
			 */
			const glm::mat4& GetTransformationMatrix() const;

			/**
			 * Returns the forward direction of the transform in scene space.
			 *
			 * @return Forward vector in scene space.
			 */
			glm::vec3 GetForward() const;

			/**
			 * Sets the position of the transform relative to the parent Game
			 * Object's transform.
			 *
			 * @param position Point.
			 */
			void SetLocalPosition(const glm::vec3& position);

			/**
			 * Sets the rotation of the transform relative to the parent Game
			 * Object's transform.
			 *
			 * @param rotation Rotation quaternion.
			 */
			void SetLocalRotation(const glm::quat& rotation);

			/**
			 * Sets the scale of the transform relative to the parent Game
			 * Object's transform.
			 *
			 * @param scale Component scale vector.
			 */
			void SetLocalScale(const glm::vec3& scale);

			/**
			 * Sets the scale of the transform relative to the parent Game
			 * Object's transform.
			 *
			 * @param scale Uniform scale.
			 */
			void SetLocalScale(float scale);

			/**
			 * Sets the position of the transform in scene space.
			 *
			 * @param position Point.
			 */
			void SetPosition(const glm::vec3& position);

			/**
			 * Sets the rotation of the transform in scene space.
			 *
			 * @param rotation Rotation quaternion.
			 */
			void SetRotation(const glm::quat& rotation);

			/**
			 * Sets the scale of the transform in scene space.
			 *
			 * @param scale Component scale vector.
			 */
			void SetScale(const glm::vec3& scale);

			/**
			 * Sets the scale of the transform in scene space.
			 *
			 * @param scale Uniform scale.
			 */
			void SetScale(float scale);

			/**
			 * Moves the transform in the direction and distance specified by
			 * the provided translation vector.
			 *
			 * @param translation Translation vector.
			 */
			void Translate(const glm::vec3& translation);

			/**
			 * Rotates the transform by the specified rotation quaternion.
			 *
			 * @param rotation Rotation quaternion.
			 */
			void Rotate(const glm::quat& rotation);

			/**
			 * Scales the transform by the specified scaling vector.
			 *
			 * @param scale Scaling vector.
			 */
			void Scale(const glm::vec3& scale);

			/**
			 * Scales the transform uniformly by the specified multiplier.
			 *
			 * @param scale Unisform scaling multiplier.
			 */
			void Scale(float scale);

			/**
			 * Rotates the transform so that the transform's forward vector
			 * points at the target point. Then rotates the transform to point
			 * its upward vector in the up direction hinted.
			 *
			 * @param point Scene space point to look at.
			 * @param up Scene space vector specifying the upward direction.
			 * This is taken to be the direction of the positive y-axis by
			 * default.
			 */
			void LookAt(const glm::vec3& point, const glm::vec3& up = glm::vec3(0.0f, 1.0f, 0.0f));

			/**
			 * Transforms the specified point provided in local coordinates
			 * into scene coordinates.
			 *
			 * @param point Point in local space.
			 * @return Point in scene space.
			 */
			glm::vec3 TransformPoint(const glm::vec3& point) const;

		public:
			/**
			 * Returns the unit quaternion that would rotate the transform's
			 * forward direction to face the specified direction.
			 *
			 * @note The quaternion returned represents the shortest possible
			 * rotation and will probably not preserve the upward direction.
			 *
			 * @param direction The direction in which the quaternion should
			 * rotate the transform's forward direction to face.
			 * @return Unit quaternion that rotates the transform's forward
			 * direction to point in the specified direction.
			 */
			glm::quat GetRotationToFaceDirection(const glm::vec3& direction) const;

			/**
			 * Returns the unit quaternion that would rotate the transform's
			 * forward direction to face the specified direction and the
			 * transform's upward direction to point in the direction of the
			 * specified scene up vector.
			 *
			 * @param direction The direction in which the quaternion should
			 * rotate the transform's forward direction to face.
			 * @param up Scene space vector specifying the upward direction.
			 * @return Unit quaternion that rotates the transform's forward
			 * direction to point in the specified direction and the transform's
			 * upward direction to point in direction of the specified scene up
			 * vector.
			 */
			glm::quat GetRotationToFaceDirection(const glm::vec3& direction, const glm::vec3& up) const;

		private:
			/**
			 * Recalculates the local transformation matrix and marks the
			 * relevant boolean flag as clean.
			 */
			void CleanLocalTransformationMatrix();

			/**
			 * Recalculates the scene space transformation matrix and marks the
			 * relevant boolean flag as clean.
			 */
			void CleanTransformationMatrix();

		private:
			/**
			 * The position of the transform relative to the parent Game
			 * Object's transform.
			 */
			glm::vec3 m_localPosition;

			/**
			 * The rotation of the transform relative to the parent Game
			 * Object's transform.
			 */
			glm::quat m_localRotation;

			/**
			 * The scale of the transform relative to the parent Game Object's
			 * transform.
			 */
			glm::vec3 m_localScale;

			/**
			 * The position of the transform in scene space.
			 */
			glm::vec3 m_position;

			/**
			 * The rotation of the transform in scene space.
			 */
			glm::quat m_rotation;

			/**
			 * The scale of the transform in scene space.
			 */
			glm::vec3 m_scale;

			/**
			 * The transformation matrix of the transform relative to the parent
			 * Game Object's transform.
			 */
			glm::mat4 m_localTransformationMatrix;

			/**
			 * The transformation matrix of the transform in scene space.
			 */
			glm::mat4 m_transformationMatrix;

		private:
			/**
			 * Whether or not the local transformation matrix needs to be
			 * recalculated.
			 */
			bool m_localTransformationMatrixDirty;

			/**
			 * Whether or not the scene space transformation matrix needs to be
			 * recalculated.
			 */
			bool m_transformationMatrixDirty;

		private:
			/**
			 * The subscription for AncestorTransformTranslatedEvents.
			 */
			EventDispatcher::SubscriptionID m_ancestorTranslatedSubscription;

			/**
			 * The subscription for AncestorTransformRotatedEvents.
			 */
			EventDispatcher::SubscriptionID m_ancestorRotatedSubscription;

			/**
			 * The subscription for AncestorTransformScaledEvents.
			 */
			EventDispatcher::SubscriptionID m_ancestorScaledSubscription;

			/**
			 * The subscription for ChildGameObjectAttachedEvents.
			 */
			EventDispatcher::SubscriptionID m_childAttachedSubscription;
		};
	}
}

#endif
