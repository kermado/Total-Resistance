#ifndef WALLFACTORY_H
#define	WALLFACTORY_H

#include <Engine/IGameObjectFactory.hpp>
#include <Engine/GameObject.hpp>

class WallFactory : public Engine::IGameObjectFactory
{
public:
	/**
	 * Constructor.
	 */
	WallFactory();

	/**
	 * Destructor.
	 */
	~WallFactory();

	/**
	 * Constructs a Wall game object.
	 *
	 * @param gameObject Shared pointer to the game object to construct.
	 */
	virtual void CreateGameObject(std::shared_ptr<Engine::GameObject> gameObject) const;
};

#endif
