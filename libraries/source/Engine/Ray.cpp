#include <Engine/Ray.hpp>

namespace Engine
{
	Ray::Ray(const glm::vec3& origin, const glm::vec3& direction)
	: m_origin(origin)
	, m_direction(direction)
	{
		// Nothing to do.
	}

	Ray::Ray(const Ray& other)
	: m_origin(other.m_origin)
	, m_direction(other.m_direction)
	{
		// Nothing to do.
	}

	Ray::~Ray()
	{
		// Nothing to do.
	}

	const glm::vec3& Ray::GetOrigin() const
	{
		return m_origin;
	}

	const glm::vec3& Ray::GetDirection() const
	{
		return m_direction;
	}

	const glm::vec3 Ray::GetPoint(float distance) const
	{
		return m_origin + distance * m_direction;
	}
}
