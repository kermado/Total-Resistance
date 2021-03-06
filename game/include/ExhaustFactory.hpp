#ifndef EXHAUSTFACTORY_H
#define	EXHAUSTFACTORY_H

#include <Engine/IGameObjectFactory.hpp>
#include <Engine/GameObject.hpp>

class ExhaustFactory : public Engine::IGameObjectFactory
{
public:
	/**
	 * Constructor.
	 */
	ExhaustFactory();

	/**
	 * Destructor.
	 */
	~ExhaustFactory();

	/**
	 * Constructs an Explosion game object.
	 *
	 * @param gameObject Shared pointer to the game object to construct.
	 */
	virtual void CreateGameObject(std::shared_ptr<Engine::GameObject> gameObject) const;

private:
	/**
	 * Returns a float randomly between the two specified bounds.
	 *
	 * @param min Minimum value.
	 * @param max Maximum value.
	 * @return Random value within the range.
	 */
	static float RandomInRange(float min, float max);
};

#endif
