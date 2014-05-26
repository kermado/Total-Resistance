#ifndef RANGEFACTORY_H
#define	RANGEFACTORY_H

#include <Engine/IGameObjectFactory.hpp>
#include <Engine/GameObject.hpp>

class RangeFactory : public Engine::IGameObjectFactory
{
public:
	/**
	 * Constructor.
	 *
	 * @param range Range radius in local coordinates.
	 */
	RangeFactory(float range);

	/**
	 * Destructor.
	 */
	~RangeFactory();

	/**
	 * Constructs a game object that displays a targeting range.
	 *
	 * @param gameObject Shared pointer to the game object to construct.
	 */
	virtual void CreateGameObject(std::shared_ptr<Engine::GameObject> gameObject) const;

private:
	/**
	 * The range raduis in local coordinates.
	 */
	float m_range;
};

#endif
