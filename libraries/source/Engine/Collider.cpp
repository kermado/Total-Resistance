#include <Engine/Collider.hpp>

#include <glm/gtx/component_wise.hpp>
#include <glm/gtx/norm.hpp>

#include <Engine/IGameScene.hpp>
#include <Engine/Octree.hpp>

#include <Engine/Attribute/Transform.hpp>
#include <Engine/Attribute/BoundingSphere.hpp>
#include <Engine/Event/CollisionEvent.hpp>

namespace Engine
{
	Collider::Collider()
	: m_collisionList()
	, m_bounds()
	{
		// Nothing to do.
	}

	Collider::~Collider()
	{
		// Nothing to do.
	}

	void Collider::Process(const IGameScene& gameScene)
	{
		// Clear the collision detection list from the previous execution.
		m_collisionList.clear();

		// Reset the scene bounds calculated from the previous execution.
		m_bounds = FloatCuboid();

		// Get the number of Game Objects in the scene.
		const unsigned int gameObjectCount = gameScene.GetGameObjectCount();

		// Populate the collision detection list with all the collidable Game
		// Objects in the scene.
		for (unsigned int i = 0; i < gameObjectCount; ++i)
		{
			ParseGameScene(gameScene.GetGameObject(i));
		}

		/*
		// Create an octree space partition that spans the scene bounds.
		Octree octree(m_bounds);

		// Populate the octree.
		for (auto iter = m_collisionList.begin(); iter != m_collisionList.end(); ++iter)
		{
			octree.Insert(*iter);
		}

		// Process collisions.
		for (auto iter = m_collisionList.begin(); iter != m_collisionList.end(); ++iter)
		{
			std::shared_ptr<GameObject> gameObject = *iter;
			octree.ProcessObject(gameObject, [gameObject](std::shared_ptr<GameObject> otherGameObject)
			{
				// Get the attributes.
				std::shared_ptr<Attribute::Transform> transform1 = gameObject->GetAttribute<Attribute::Transform>();
				std::shared_ptr<Attribute::BoundingSphere> boundingSphere1 = gameObject->GetAttribute<Attribute::BoundingSphere>();
				std::shared_ptr<Attribute::Transform> transform2 = otherGameObject->GetAttribute<Attribute::Transform>();
				std::shared_ptr<Attribute::BoundingSphere> boundingSphere2 = otherGameObject->GetAttribute<Attribute::BoundingSphere>();

				// Get the scale factors for the bounding spheres.
				const float scaleFactor1 = glm::compMax(transform1->GetScale());
				const float scaleFactor2 = glm::compMax(transform2->GetScale());

				// Get the scaled sphere radii.
				const float radius1 = boundingSphere1->GetRadius() * scaleFactor1;
				const float radius2 = boundingSphere2->GetRadius() * scaleFactor2;

				// Get the square distance between the center of two Game Objects.
				const float centerSquareDistance = glm::distance2(
					transform1->GetPosition(),
					transform2->GetPosition()
				);

				// Determine whether the bounding spheres overlap.
				// The two spheres are overlapping if the square of the distance
				// between their centers is less than the squre of the sum of the
				// radii.
				if (centerSquareDistance < (radius1 + radius2) * (radius1 + radius2))
				{
					gameObject->EnqueueEvent<Event::CollisionEvent>(
						otherGameObject,
						boundingSphere2->GetTag(),
						boundingSphere1->GetTag()
					);
					otherGameObject->EnqueueEvent<Event::CollisionEvent>(
						gameObject,
						boundingSphere1->GetTag(),
						boundingSphere2->GetTag()
					);
				}
			});
		}
		*/

		// For each unique pair of Game Objects in the collision detection
		// list...
		for (auto i = m_collisionList.begin(); i != m_collisionList.end(); ++i)
		{
			for (auto j = m_collisionList.rbegin(); *j != *i; ++j)
			{
				// Get pointers to the two game objects.
				std::shared_ptr<GameObject> gameObject1 = *i;
				std::shared_ptr<GameObject> gameObject2 = *j;

				// Get the attributes.
				std::shared_ptr<Attribute::Transform> transform1 = gameObject1->GetAttribute<Attribute::Transform>();
				std::shared_ptr<Attribute::BoundingSphere> boundingSphere1 = gameObject1->GetAttribute<Attribute::BoundingSphere>();
				std::shared_ptr<Attribute::Transform> transform2 = gameObject2->GetAttribute<Attribute::Transform>();
				std::shared_ptr<Attribute::BoundingSphere> boundingSphere2 = gameObject2->GetAttribute<Attribute::BoundingSphere>();

				// Get the scene coordinate scale factors for the bounding spheres.
				const float scaleFactor1 = glm::compMax(transform1->GetScale());
				const float scaleFactor2 = glm::compMax(transform2->GetScale());

				// Get the scaled sphere radii.
				const float radius1 = boundingSphere1->GetRadius() * scaleFactor1;
				const float radius2 = boundingSphere2->GetRadius() * scaleFactor2;

				// Get the square distance between the center of two Game Objects.
				const float centerSquareDistance = glm::distance2(
					transform1->GetPosition(),
					transform2->GetPosition()
				);

				// Determine whether the bounding spheres overlap.
				// The two spheres are overlapping if the square of the distance
				// between their centers is less than the squre of the sum of the
				// radii.
				if (centerSquareDistance < (radius1 + radius2) * (radius1 + radius2))
				{
					gameObject1->EnqueueEvent<Event::CollisionEvent>(
						gameObject2,
						boundingSphere2->GetTag(),
						boundingSphere1->GetTag()
					);
					gameObject2->EnqueueEvent<Event::CollisionEvent>(
						gameObject1,
						boundingSphere1->GetTag(),
						boundingSphere2->GetTag()
					);
				}
			}
		}
	}

	void Collider::ParseGameScene(std::shared_ptr<GameObject> gameObject)
	{
		if (!gameObject->IsDead())
		{
			// Check whether the specified game object should be added to the
			// collision detection list.
			if (gameObject->HasAttribute<Attribute::BoundingSphere>() &&
				gameObject->HasAttribute<Attribute::Transform>())
			{
				// Add the game object to the collision detection list.
				m_collisionList.push_back(gameObject);

				// Get the game object's transform component.
				std::shared_ptr<Attribute::Transform> transform =
					gameObject->GetAttribute<Attribute::Transform>();

				// Get the position of the game object in scene coordinates.
				const glm::vec3 position = transform->GetPosition();

				// Get the game object's bounding sphere.
				std::shared_ptr<Attribute::BoundingSphere> boundingSphere =
					gameObject->GetAttribute<Attribute::BoundingSphere>();

				// Get the scaled radius of the bounding sphere.
				const float radius = boundingSphere->GetRadius() * glm::compMax(transform->GetScale());

				// Calculate the extents of the bounding sphere geometry in
				// scene coordinates.
				const float left = position.x - radius;
				const float right = position.x + radius;
				const float top = position.y + radius;
				const float bottom = position.y - radius;
				const float near = position.z + radius;
				const float far = position.z - radius;

				// Update the scene bounds.
				m_bounds.SetLeft(std::min(m_bounds.GetLeft(), left));
				m_bounds.SetRight(std::max(m_bounds.GetRight(), right));
				m_bounds.SetTop(std::max(m_bounds.GetTop(), top));
				m_bounds.SetBottom(std::min(m_bounds.GetBottom(), bottom));
				m_bounds.SetNear(std::max(m_bounds.GetNear(), near));
				m_bounds.SetFar(std::min(m_bounds.GetFar(), far));
			}

			// Recursively check any child Game Objects.
			const unsigned int childCount = gameObject->GetChildCount();
			for (unsigned int i = 0; i < childCount; ++i)
			{
				ParseGameScene(gameObject->GetChild(i));
			}
		}
	}
}
