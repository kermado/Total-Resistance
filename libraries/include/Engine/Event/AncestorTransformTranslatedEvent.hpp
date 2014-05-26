#ifndef ANCESTORTRANSFORMTRANSLATEDEVENT_H
#define ANCESTORTRANSFORMTRANSLATEDEVENT_H

#include <glm/glm.hpp>

#include <Engine/Event/IEvent.hpp>

namespace Engine
{
	namespace Event
	{
		class AncestorTransformTranslatedEvent : public IEvent
		{
		public:
			/**
			 * Constructor.
			 *
			 * @param delta The translation that was applied.
			 */
			AncestorTransformTranslatedEvent(const glm::vec3& delta);

			/**
			 * Destructor.
			 */
			~AncestorTransformTranslatedEvent();

			/**
			 * Returns the translation that was applied to the transform of an
			 * ancestor Game Object.
			 *
			 * @return Translation that was applied to the transform of an
			 * ancestor Game Object.
			 */
			const glm::vec3& GetDelta() const;

		private:
			/**
			 * The translation that was applied to the transform of an ancestor
			 * Game Object.
			 */
			glm::vec3 m_delta;
		};
	}
}

#endif
