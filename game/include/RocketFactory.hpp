#ifndef ROCKETFACTORY_H
#define	ROCKETFACTORY_H

#include <Engine/IGameObjectFactory.hpp>
#include <Engine/GameObject.hpp>

#include "PlayingSurface.hpp"

class RocketFactory : public Engine::IGameObjectFactory
{
public:
	/**
	 * Constructor.
	 *
	 * @param playingSurface Shared pointer to the playing surface.
	 */
	RocketFactory(std::shared_ptr<const PlayingSurface> playingSurface);

	/**
	 * Destructor.
	 */
	~RocketFactory();

	/**
	 * Constructs a Rocket game object.
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
