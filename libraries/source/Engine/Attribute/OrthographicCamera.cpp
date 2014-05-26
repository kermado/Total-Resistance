#include <Engine/Attribute/OrthographicCamera.hpp>

#include <glm/gtc/matrix_transform.hpp>

namespace Engine
{
	namespace Attribute
	{
		OrthographicCamera::OrthographicCamera(std::shared_ptr<Window> window,
				std::shared_ptr<ResourceManager> resourceManager,
				std::shared_ptr<EventDispatcher> sceneEventDispatcher,
				std::shared_ptr<EventDispatcher> gameObjectEventDispatcher,
				std::weak_ptr<GameObject> gameObject,
				float viewportWidth,
				float viewportHeight,
				float near,
				float far)
		: IAttribute(window, resourceManager, sceneEventDispatcher, gameObjectEventDispatcher, gameObject)
		, m_dirty(true)
		, m_viewportWidth(viewportWidth)
		, m_viewportHeight(viewportHeight)
		, m_near(near)
		, m_far(far)
		, m_projectionMatrix()
		{
			// Nothing to do.
		}

		OrthographicCamera::~OrthographicCamera()
		{
			// Nothing to do.
		}

		void OrthographicCamera::SetViewportWidth(float viewportWidth)
		{
			m_viewportWidth = viewportWidth;
			m_dirty = true;
		}

		void OrthographicCamera::SetViewportHeight(float viewportHeight)
		{
			m_viewportHeight = viewportHeight;
			m_dirty = true;
		}

		const glm::mat4& OrthographicCamera::GetProjectionMatrix()
		{
			if (m_dirty)
			{
				UpdateProjectionMatrix();
			}

			return m_projectionMatrix;
		}

		void OrthographicCamera::UpdateProjectionMatrix()
		{
			const float halfWidth = 0.5f * m_viewportWidth;
			const float halfHeight = 0.5f * m_viewportHeight;

			m_projectionMatrix = glm::ortho<float>(
				-halfWidth,
				halfWidth,
				-halfHeight,
				halfHeight,
				m_near,
				m_far
			);

			m_dirty = false;
		}
	}
}
