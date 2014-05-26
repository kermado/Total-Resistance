#ifndef COLLISIONEVENT_H
#define	COLLISIONEVENT_H

#include <string>
#include <memory>

#include <Engine/Event/IEvent.hpp>
#include <Engine/GameObject.hpp>

namespace Engine
{
	namespace Event
	{
		class CollisionEvent : public IEvent
		{
		public:
			/**
			 * Constructor.
			 *
			 * @param otherGameObject Shared pointer to the other Game Object
			 * that was involved in the collision.
			 * @param otherBoundingGeometryTag Tag for the other Game
			 * Object's bounding geometry that was involved the collision.
			 * @param thisBoundingGeometryTag Tag for the bounding
			 * geometry that was involved in the collision.
			 */
			CollisionEvent(std::shared_ptr<GameObject> otherGameObject,
				const std::string otherBoundingGeometryTag,
				const std::string thisBoundingGeometryTag);

			/**
			 * Destructor.
			 */
			virtual ~CollisionEvent();

			/**
			 * Returns a shared pointer to the other Game Object
			 * that was involved in the collision.
			 *
			 * @return Pointer to the other Game Object.
			 */
			std::shared_ptr<GameObject> GetOtherGameObject() const;

			/**
			 * Returns the tag for the other Game Object's bounding
			 * geometry that was involved in the collision.
			 *
			 * @return Bounding geometry tag for the other Game Object.
			 */
			std::string GetOtherBoundingGeometryTag() const;

			/**
			 * Returns the tag for the Game Object's bounding
			 * geometry that was involved in the collision.
			 *
			 * @return Bounding geometry tag for this Game Object.
			 */
			std::string GetThisBoundingGeometryTag() const;

		private:
			/**
			 * Shared pointer to the other Game Object that was involved in the
			 * collision.
			 */
			std::shared_ptr<GameObject> m_otherGameObject;

			/**
			 * Tag for the bounding geometry of the other Game Object involved
			 * in the collision.
			 */
			std::string m_otherBoundingGeometryTag;

			/**
			 * Tag for the bounding geometry of the Game Object to which this
			 * event was dispatched.
			 */
			std::string m_thisBoundingGeometryTag;
		};
	}
}

#endif
