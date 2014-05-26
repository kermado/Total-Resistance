#ifndef TARGETACQUIREDEVENT_H
#define TARGETACQUIREDEVENT_H

#include <memory>

#include <Engine/Event/IEvent.hpp>
#include <Engine/GameObject.hpp>

namespace Event
{
	class TargetAcquiredEvent : public Engine::Event::IEvent
	{
	public:
		/**
		 * Constructor.
		 *
		 * @param target Shared pointer to the target Game Object.
		 */
		TargetAcquiredEvent(std::shared_ptr<Engine::GameObject> target);

		/**
		 * Destructor.
		 */
		~TargetAcquiredEvent();

		/**
		 * Returns a shared pointer to the target Game Object.
		 *
		 * @return Shared pointer to the Game Object that was targeted.
		 */
		std::shared_ptr<Engine::GameObject> GetTarget() const;

	private:
		/**
		 * Shared pointer to the target Game Object.
		 */
		std::shared_ptr<Engine::GameObject> m_target;
	};
}

#endif
