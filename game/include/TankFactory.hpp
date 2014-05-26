#ifndef TANKFACTORY_H
#define	TANKFACTORY_H

#include <memory>

#include <Engine/IGameObjectFactory.hpp>
#include <Engine/GameObject.hpp>

#include "PlayingSurface.hpp"

class TankFactory : public Engine::IGameObjectFactory
{
public:
	/**
	 * Constructor.
	 *
	 * @param playingSurface Shared pointer to the playing surface.
	 * @param initialCell Initial cell for the tank.
	 * @param destinationCell Final target destination cell for the tank.
	 * @param healthMultiple Value to multiply the default health value by.
	 */
	TankFactory(std::shared_ptr<const PlayingSurface> playingSurface, Cell initialCell, Cell destinationCell, float healthMultiple = 1.0f);

	/**
	 * Destructor.
	 */
	~TankFactory();

	/**
	 * Constructs a Tank game object.
	 *
	 * @param gameObject Shared pointer to the game object to construct.
	 */
	virtual void CreateGameObject(std::shared_ptr<Engine::GameObject> gameObject) const;

private:
	/**
	 * Shared pointer to the playing surface (read only).
	 */
	std::shared_ptr<const PlayingSurface> m_playingSurface;

	/**
	 * The initial (starting) cell.
	 */
	Cell m_initialCell;

	/**
	 * The destination (target) cell.
	 */
	Cell m_destinationCell;

	/**
	 * The value to multiply the default health value by.
	 */
	float m_healthMultiple;
};

#endif
