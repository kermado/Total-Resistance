#ifndef MISSILESILOFACTORY_H
#define	MISSILESILOFACTORY_H

#include <memory>

#include <Engine/IGameObjectFactory.hpp>
#include <Engine/GameObject.hpp>

#include "PlayingSurface.hpp"

class MissileSiloFactory : public Engine::IGameObjectFactory
{
public:
	/**
	 * Constructor.
	 *
	 * @param playingSurface Shared pointer to the playing surface.
	 */
	MissileSiloFactory(std::shared_ptr<const PlayingSurface> playingSurface);

	/**
	 * Destructor.
	 */
	~MissileSiloFactory();

	/**
	 * Constructs a Missile Silo game object.
	 *
	 * @param gameObject Shared pointer to the game object to construct.
	 */
	virtual void CreateGameObject(std::shared_ptr<Engine::GameObject> gameObject) const;

private:
	/**
	 * Shared pointer to the playing surface (read only).
	 */
	std::shared_ptr<const PlayingSurface> m_playingSurface;
};

#endif
