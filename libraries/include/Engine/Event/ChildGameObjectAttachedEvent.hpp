#ifndef CHILDGAMEOBJECTATTACHEDEVENT_H
#define CHILDGAMEOBJECTATTACHEDEVENT_H

#include <memory>

#include <Engine/Event/IEvent.hpp>
#include <Engine/GameObject.hpp>

namespace Engine
{
	namespace Event
	{
		class ChildGameObjectAttachedEvent : public IEvent
		{
		public:
			/**
			 * Constructor.
			 *
			 * @param child Shared pointer to Game Object that was attached as a
			 * child to this Game Object.
			 */
			ChildGameObjectAttachedEvent(std::shared_ptr<GameObject> child);

			/**
			 * Destructor.
			 */
			~ChildGameObjectAttachedEvent();

			/**
			 * Returns a shared pointer to the Game Object that was attached as
			 * a child to this Game Object.
			 *
			 * @retirn Shared pointer to the child Game Object.
			 */
			std::shared_ptr<GameObject> GetChild() const;

		private:
			/**
			 * Shared pointer to the child Game Object that was attached.
			 */
			std::shared_ptr<GameObject> m_child;
		};
	}
}

#endif
