#ifndef ROCKETLAUNCHERFACTORY_H
#define	ROCKETLAUNCHERFACTORY_H

#include <memory>

#include <Engine/IGameObjectFactory.hpp>
#include <Engine/GameObject.hpp>

#include "PlayingSurface.hpp"

class RocketLauncherFactory : public Engine::IGameObjectFactory
{
public:
	/**
	 * Constructor.
	 *
	 * @param playingSurface Shared pointer to the playing surface.
	 */
	RocketLauncherFactory(std::shared_ptr<const PlayingSurface> playingSurface);

	/**
	 * Destructor.
	 */
	~RocketLauncherFactory();

	/**
	 * Constructs a Rocket Launcher game object.
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
