#ifndef ANCESTORTRANSFORMSCALEDEVENT_H
#define ANCESTORTRANSFORMSCALEDEVENT_H

#include <glm/glm.hpp>

#include <Engine/Event/IEvent.hpp>

namespace Engine
{
	namespace Event
	{
		class AncestorTransformScaledEvent : public IEvent
		{
		public:
			/**
			 * Constructor.
			 *
			 * @param delta The scaling that was applied.
			 */
			AncestorTransformScaledEvent(const glm::vec3& delta);

			/**
			 * Destructor.
			 */
			~AncestorTransformScaledEvent();

			/**
			 * Returns the scaling that was applied to the transform of an
			 * ancestor Game Object.
			 *
			 * @return Scaling that was applied to the transform of an
			 * ancestor Game Object.
			 */
			const glm::vec3& GetDelta() const;

		private:
			/**
			 * The scaling that was applied to the transform of an ancestor
			 * Game Object.
			 */
			glm::vec3 m_delta;
		};
	}
}

#endif
