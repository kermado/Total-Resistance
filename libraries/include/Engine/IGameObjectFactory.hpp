#ifndef IGAMEOBJECTFACTORY_H
#define	IGAMEOBJECTFACTORY_H

#include <memory>

namespace Engine
{
	class GameObject;

	class IGameObjectFactory
	{
	public:
		/**
		 * Decorates a Game Object.
		 *
		 * @param gameObject Shared pointer to the Game Object to decorate.
		 */
		virtual void CreateGameObject(std::shared_ptr<GameObject> gameObject) const
		{
			// Nothing to do.
		}
	};
}

#endif
