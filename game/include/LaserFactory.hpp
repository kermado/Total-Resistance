#ifndef LASERFACTORY_H
#define	LASERFACTORY_H

#include <Engine/IGameObjectFactory.hpp>
#include <Engine/GameObject.hpp>

class LaserFactory : public Engine::IGameObjectFactory
{
public:
	/**
	 * Constructor.
	 */
	LaserFactory();

	/**
	 * Destructor.
	 */
	~LaserFactory();

	/**
	 * Constructs a Laser game object.
	 *
	 * @param gameObject Shared pointer to the game object to construct.
	 */
	virtual void CreateGameObject(std::shared_ptr<Engine::GameObject> gameObject) const;
};

#endif
