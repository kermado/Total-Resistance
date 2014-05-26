#ifndef LASERTOWERFACTORY_H
#define	LASERTOWERFACTORY_H

#include <memory>

#include <Engine/IGameObjectFactory.hpp>
#include <Engine/GameObject.hpp>

#include "PlayingSurface.hpp"

class LaserTowerFactory : public Engine::IGameObjectFactory
{
public:
	/**
	 * Constructor.
	 *
	 * @param playingSurface Shared pointer to the playing surface.
	 */
	LaserTowerFactory(std::shared_ptr<const PlayingSurface> playingSurface);

	/**
	 * Destructor.
	 */
	~LaserTowerFactory();

	/**
	 * Constructs a Laser Tower game object.
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
