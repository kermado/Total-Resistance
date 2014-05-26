#ifndef COLLIDER_H
#define	COLLIDER_H

#include <memory>

#include <Engine/NonCopyable.hpp>
#include <Engine/GameObject.hpp>
#include <Engine/Cuboid.hpp>

namespace Engine
{
	class IGameScene;

	class Collider : private NonCopyable
	{
	public:
		/**
		 * Constructor.
		 */
		Collider();

		/**
		 * Destructor.
		 */
		~Collider();

		/**
		 * Checks for collisions between Game Objects in the game scene.
		 *
		 * @param gameScene Reference to the game scene.
		 */
		void Process(const IGameScene& gameScene);

	protected:
		/**
		 * Sets the stage for performing collision detection by recursively
		 * parsing the game scene.
		 *
		 * @param gameObject Shared pointer to a Game Object in the game scene.
		 */
		void ParseGameScene(std::shared_ptr<GameObject> gameObject);

	private:
		/**
		 * Holds shared pointers to all of the Game Objects that need to be
		 * tested for collisions.
		 */
		std::vector<std::shared_ptr<GameObject>> m_collisionList;

		/**
		 * Game scene bounds.
		 */
		FloatCuboid m_bounds;
	};
}

#endif
