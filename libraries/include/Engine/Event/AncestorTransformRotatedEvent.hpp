#ifndef ANCESTORTRANSFORMROTATEDEVENT_H
#define ANCESTORTRANSFORMROTATEDEVENT_H

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include <Engine/Event/IEvent.hpp>

namespace Engine
{
	namespace Event
	{
		class AncestorTransformRotatedEvent : public IEvent
		{
		public:
			/**
			 * Constructor.
			 *
			 * @param delta The translation that was applied.
			 */
			AncestorTransformRotatedEvent(const glm::quat& delta);

			/**
			 * Destructor.
			 */
			~AncestorTransformRotatedEvent();

			/**
			 * Returns the rotation that was applied to the transform of an
			 * ancestor Game Object.
			 *
			 * @return Rotation that was applied to the transform of an
			 * ancestor Game Object.
			 */
			const glm::quat& GetDelta() const;

		private:
			/**
			 * The rotation that was applied to the transform of an ancestor
			 * Game Object.
			 */
			glm::quat m_delta;
		};
	}
}

#endif
