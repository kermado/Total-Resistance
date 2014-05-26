#ifndef UPDATEDESTINATIONEVENT_H
#define UPDATEDESTINATIONEVENT_H

#include <glm/glm.hpp>

#include <Engine/Event/IEvent.hpp>

namespace Event
{
	class UpdateDestinationEvent : public Engine::Event::IEvent
	{
	public:
		/**
		 * Constructor.
		 *
		 * @param destination Destination point in scene space.
		 */
		UpdateDestinationEvent(const glm::vec3& destination);

		/**
		 * Destructor.
		 */
		~UpdateDestinationEvent();

		/**
		 * Returns the scene destination point.
		 *
		 * @return Scene destination point.
		 */
		const glm::vec3& GetDestination() const;

	private:
		/**
		 * The destination point in scene space.
		 */
		glm::vec3 m_destination;
	};
}

#endif
