#ifndef PLANE_H
#define	PLANE_H

#include <glm/glm.hpp>

#include <Engine/Ray.hpp>

namespace Engine
{
	class Plane
	{
	public:
		/**
		 * Constructor.
		 *
		 * @param normal Normal vector for the plane.
		 * @param distance Distance of the plane from the origin.
		 */
		Plane(const glm::vec3& normal, float distance);

		/**
		 * Constructor.
		 *
		 * @param normal Normal vector for the plane.
		 * @param point A point on the plane.
		 */
		Plane(const glm::vec3& normal, const glm::vec3& point);

		/**
		 * Constructor.
		 *
		 * @param firstPoint First point on the plane.
		 * @param secondPoint Second point on the plane.
		 * @param thirdPoint Third point on the plane.
		 */
		Plane(const glm::vec3& firstPoint, const glm::vec3& secondPoint,
			const glm::vec3& thirdPoint);

		/**
		 * Copy constructor.
		 *
		 * @param other The other plane.
		 */
		Plane(const Plane& other);

		/**
		 * Destructor.
		 */
		~Plane();

		/**
		 * Calculates the distance along the ray at which the ray intersect
		 * the plane. Returns false if the ray and plane are parallel and
		 * sets the intersection distance to 0.
		 *
		 * @param ray Ray to intersect with plane.
		 * @param[out] outIntersectionDistance Distance along the ray at
		 * which an intersection with the plane occurs.
		 * @return True if the ray intersects the plane. False if the ray
		 * and plane are parallel.
		 */
		bool Raycast(const Ray& ray, float& outIntersectionDistance) const;

	private:
		/**
		 * Unit normal to the plane.
		 */
		glm::vec3 m_normal;

		/**
		 * Distance from the origin to the plane.
		 */
		float m_distance;
	};
}

#endif
