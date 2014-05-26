#ifndef RAY_H
#define	RAY_H

#include <glm/glm.hpp>

namespace Engine
{
	class Ray
	{
	public:
		/**
		 * Constructor.
		 *
		 * @param origin Origin for the ray.
		 * @param direction Direction for the ray.
		 */
		Ray(const glm::vec3& origin, const glm::vec3& direction);

		/**
		 * Copy constructor.
		 *
		 * @param other The other ray.
		 */
		Ray(const Ray& other);

		/**
		 * Destructor.
		 */
		~Ray();

		/**
		 * Returns the origin of the ray.
		 *
		 * @return Ray origin.
		 */
		const glm::vec3& GetOrigin() const;

		/**
		 * Returns the direction of the ray.
		 *
		 * @return Ray direction.
		 */
		const glm::vec3& GetDirection() const;

		/**
		 * Returns the point at the specified distane from the ray
		 * origin in the direction of the ray.
		 *
		 * @param distance Distance along the ray for the point.
		 * @return Point at the specified distance along the ray.
		 */
		const glm::vec3 GetPoint(float distance) const;

	private:
		/**
		 * Ray origin.
		 */
		glm::vec3 m_origin;

		/**
		 * Ray direction.
		 */
		glm::vec3 m_direction;
	};
}

#endif
