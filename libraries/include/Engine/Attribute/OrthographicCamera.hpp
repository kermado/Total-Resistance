#ifndef ORTHOGRAPHICCAMERA_H
#define	ORTHOGRAPHICCAMERA_H

#include <memory>

#include <glm/glm.hpp>

#include <Engine/Attribute/IAttribute.hpp>
#include <Engine/Window.hpp>
#include <Engine/ResourceManager.hpp>
#include <Engine/EventDispatcher.hpp>

namespace Engine
{
	namespace Attribute
	{
		class OrthographicCamera : public IAttribute
		{
		public:
			/**
			 * Constructor.
			 *
			 * @param window Shared pointer to the window.
			 * @param resourceManager Shared pointer to the resource manager.
			 * @param sceneEventDispatcher Shared pointer to the scene's Event
			 * Dispatcher.
			 * @param gameObjectEventDispatcher Shared pointer to the Game
			 * Object's Event Dispatcher.
			 * @param gameObject Weak pointer to the Game Object that the
			 * attribute is attached to.
			 * @param viewportWidth Width of the viewport (pixels).
			 * @param viewportHeight Height of the viewport (pixels).
			 * @param near Distance to the near clipping plane.
			 * @param far Distance to the far clipping plane.
			 */
			OrthographicCamera(std::shared_ptr<Window> window,
				std::shared_ptr<ResourceManager> resourceManager,
				std::shared_ptr<EventDispatcher> sceneEventDispatcher,
				std::shared_ptr<EventDispatcher> gameObjectEventDispatcher,
				std::weak_ptr<GameObject> gameObject,
				float viewportWidth,
				float viewportHeight,
				float near,
				float far);

			/**
			 * Destructor.
			 */
			virtual ~OrthographicCamera();

			/**
			 * Sets the width of the viewport.
			 *
			 * @param viewportWidth Width of the viewport (in pixels).
			 */
			void SetViewportWidth(float viewportWidth);

			/**
			 * Sets the height of the viewport.
			 *
			 * @param viewportHeight Width of the viewport (in pixels).
			 */
			void SetViewportHeight(float viewportHeight);

			/**
			 * Returns the projection matrix for the camera.
			 *
			 * @return Reference to the projection matrix.
			 */
			const glm::mat4& GetProjectionMatrix();

		private:
			/**
			 * Updates the projection matrix and marks changes dirty flag to
			 * false.
			 */
			void UpdateProjectionMatrix();

		private:
			/**
			 * Does the projection matrix need to be recalculated?
			 */
			bool m_dirty;

			/**
			 * The width of the viewport.
			 */
			float m_viewportWidth;

			/**
			 * The height of the viewport.
			 */
			float m_viewportHeight;

			/**
			 * The distance to the near clipping plane.
			 */
			float m_near;

			/**
			 * The distance to the far clipping plane.
			 */
			float m_far;

			/**
			 * Projection matrix for the camera.
			 */
			glm::mat4 m_projectionMatrix;
		};
	}
};

#endif
