#include <Engine/Plane.hpp>
#include <cmath>

namespace Engine
{
	Plane::Plane(const glm::vec3& normal, float distance)
	: m_normal(glm::normalize(normal))
	, m_distance(distance)
	{
		// Nothing to do.
	}

	Plane::Plane(const glm::vec3& normal, const glm::vec3& point)
	: m_normal(glm::normalize(normal))
	{
		m_distance = glm::dot(point, m_normal);
	}

	Plane::Plane(const Plane& other)
	: m_normal(other.m_normal)
	, m_distance(other.m_distance)
	{
		// Nothing to do.
	}

	Plane::Plane(const glm::vec3& firstPoint, const glm::vec3& secondPoint,
		const glm::vec3& thirdPoint)
	{
		const glm::vec3 v1 = secondPoint - firstPoint;
		const glm::vec3 v2 = thirdPoint - firstPoint;
		m_normal = glm::normalize(glm::cross(v1, v2));
		m_distance = glm::dot(firstPoint, m_normal);
	}

	Plane::~Plane()
	{
		// Nothing to do.
	}

	bool Plane::Raycast(const Ray& ray, float& outIntersectionDistance) const
	{
		const float dot = glm::dot(ray.GetDirection(), m_normal);

		if (std::abs(dot) < 0.00001f)
		{
			// Ray is parallel to plane.
			outIntersectionDistance = 0;
			return false;
		}
		else
		{
			// Ray intersects plane.
			outIntersectionDistance = - glm::dot(ray.GetOrigin(), m_normal) / dot;
			return true;
		}
	}
}
