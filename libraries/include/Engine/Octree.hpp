#ifndef OCTREE_H
#define OCTREE_H

#include <vector>

#include <glm/gtx/component_wise.hpp>

#include <Engine/Cuboid.hpp>

#include <Engine/Attribute/Transform.hpp>
#include <Engine/Attribute/BoundingSphere.hpp>

namespace Engine
{
	class Octree
	{
	public:
		/**
		 * Constructor.
		 *
		 * @param bounds The 3D space occupied by the node.
		 */
		Octree(const Cuboid<float>& bounds)
		: m_nodeLevel(0)
		, m_bounds(bounds)
		, m_objects()
		, m_nodes()
		{
			// Resize the nodes vector to accomodate 8 sub-nodes.
			m_nodes.resize(8);
		}

		/**
		 * Constructor.
		 *
		 * @param nodeLevel The current node level (0 is the root node).
		 * @param bounds The 3D space occupied by the node.
		 */
		Octree(unsigned int nodeLevel, const Cuboid<float>& bounds)
		: m_nodeLevel(nodeLevel)
		, m_bounds(bounds)
		, m_objects()
		, m_nodes()
		{
			// Resize the nodes vector to accomodate 8 sub-nodes.
			m_nodes.resize(8);
		}

		/**
		 * Destructor.
		 */
		~Octree()
		{
			// Nothing to do.
		}

		/**
		 * Recursively clears the octree nodes of any existing sub-nodes and
		 * data.
		 */
		void Clear()
		{
			m_objects.clear();

			for (size_t i = 0; i < 8; ++i)
			{
				if (m_nodes[i] != nullptr)
				{
					m_nodes[i]->Clear();
					m_nodes[i] = nullptr;
				}
			}
		}

		/**
		 * Splits the octree node into 8 sub-nodes.
		 */
		void Split()
		{
			// Determine the dimensions for the sub-node bounds.
			const float subWidth = (m_bounds.GetRight() - m_bounds.GetLeft()) * 0.5f;
			const float subHeight = (m_bounds.GetTop() - m_bounds.GetBottom()) * 0.5f;
			const float subDepth = (m_bounds.GetNear() - m_bounds.GetFar()) * 0.5f;

			// Near top left node.
			m_nodes[0] = std::unique_ptr<Octree>(new Octree(
				m_nodeLevel + 1,
				Cuboid<float>(
					m_bounds.GetLeft(),
					m_bounds.GetLeft() + subWidth,
					m_bounds.GetTop(),
					m_bounds.GetTop() + subHeight,
					m_bounds.GetNear(),
					m_bounds.GetNear() + subDepth
				)
			));

			// Near top right node.
			m_nodes[1] = std::unique_ptr<Octree>(new Octree(
				m_nodeLevel + 1,
				Cuboid<float>(
					m_bounds.GetLeft() + subWidth,
					m_bounds.GetRight(),
					m_bounds.GetTop(),
					m_bounds.GetTop() + subHeight,
					m_bounds.GetNear(),
					m_bounds.GetNear() + subDepth
				)
			));

			// Near bottom left node.
			m_nodes[2] = std::unique_ptr<Octree>(new Octree(
				m_nodeLevel + 1,
				Cuboid<float>(
					m_bounds.GetLeft(),
					m_bounds.GetLeft() + subWidth,
					m_bounds.GetTop() + subHeight,
					m_bounds.GetBottom(),
					m_bounds.GetNear(),
					m_bounds.GetNear() + subDepth
				)
			));

			// Near bottom right node.
			m_nodes[3] = std::unique_ptr<Octree>(new Octree(
				m_nodeLevel + 1,
				Cuboid<float>(
					m_bounds.GetLeft() + subWidth,
					m_bounds.GetRight(),
					m_bounds.GetTop() + subHeight,
					m_bounds.GetBottom(),
					m_bounds.GetNear(),
					m_bounds.GetNear() + subDepth
				)
			));

			// Far top left node.
			m_nodes[4] = std::unique_ptr<Octree>(new Octree(
				m_nodeLevel + 1,
				Cuboid<float>(
					m_bounds.GetLeft(),
					m_bounds.GetLeft() + subWidth,
					m_bounds.GetTop(),
					m_bounds.GetTop() + subHeight,
					m_bounds.GetNear() + subDepth,
					m_bounds.GetFar()
				)
			));

			// Far top right node.
			m_nodes[5] = std::unique_ptr<Octree>(new Octree(
				m_nodeLevel + 1,
				Cuboid<float>(
					m_bounds.GetLeft() + subWidth,
					m_bounds.GetRight(),
					m_bounds.GetTop(),
					m_bounds.GetTop() + subHeight,
					m_bounds.GetNear() + subDepth,
					m_bounds.GetFar()
				)
			));

			// Far bottom left node.
			m_nodes[6] = std::unique_ptr<Octree>(new Octree(
				m_nodeLevel + 1,
				Cuboid<float>(
					m_bounds.GetLeft(),
					m_bounds.GetLeft() + subWidth,
					m_bounds.GetTop() + subHeight,
					m_bounds.GetBottom(),
					m_bounds.GetNear() + subDepth,
					m_bounds.GetFar()
				)
			));

			// Far bottom right node.
			m_nodes[7] = std::unique_ptr<Octree>(new Octree(
				m_nodeLevel + 1,
				Cuboid<float>(
					m_bounds.GetLeft() + subWidth,
					m_bounds.GetRight(),
					m_bounds.GetTop() + subHeight,
					m_bounds.GetBottom(),
					m_bounds.GetNear() + subDepth,
					m_bounds.GetFar()
				)
			));
		}

		/**
		 * Inserts the specified Game Object into the octree.
		 *
		 * @param gameObject Shared pointer to the Game Object to insert.
		 */
		void Insert(std::shared_ptr<GameObject> gameObject)
		{
			// Get the node index into which the Game Object should be inserted.
			const int nodeIndex = GetIndex(gameObject);

			// Check whether the Game Object can be inserted into one of the
			// sub-nodes.
			if (m_nodes[nodeIndex] != nullptr && nodeIndex > 0)
			{
				// Insert the Game Object into the determined sub-node.
				m_nodes[nodeIndex]->Insert(gameObject);
			}
			else
			{
				// The Game Object should be inserted into this node.
				m_objects.push_back(gameObject);

				// Determine whether we need to split the node.
				if (m_objects.size() > MAX_OBJECTS && m_nodeLevel < MAX_LEVELS)
				{
					if (m_nodes[nodeIndex] == nullptr)
					{
						Split();

						// Try to move all the objects contained within this node into
						// one of the sub-nodes if possible.
						auto iter = m_objects.begin();
						while(iter != m_objects.end())
						{
							const int index = GetIndex(*iter);
							if (index > 0)
							{
								m_nodes[index]->Insert(*iter);
								m_objects.erase(iter);
							}
							else
							{
								++iter;
							}
						}
					}
				}
			}
		}

		/**
		 * Processes each Game Object that could possibly be colliding with the
		 * specified Game Object.
		 *
		 * @param gameObject Shared pointer to the Game Object that is to be
		 * processed.
		 * @param callback Callback function that is passed each possible
		 * colliding Game Object.
		 */
		void ProcessObject(std::shared_ptr<GameObject> gameObject, std::function<void(std::shared_ptr<GameObject>)> callback)
		{
			// Determine the sub-node index for the Game Object.
			const int index = GetIndex(gameObject);

			// Recurse into sub-nodes.
			if (index != -1 && m_nodes[index] != nullptr)
			{
				m_nodes[index]->ProcessObject(gameObject, callback);
			}

			// Execute callback with all Game Objects in this node.
			for (size_t i = 0; i < m_objects.size(); ++i)
			{
				callback(m_objects[i]);
			}
		}

	private:
		/**
		 * Returns the node index into which the specified Game Object should
		 * be insterted.
		 *
		 * @note Returns -1 when the object cannot fit into any of the sub-nodes
		 * and should therefore be inserted into this node.
		 *
		 * @param gameObject Shared pointer to the Game Object.
		 * @return Node index into which the Game Object should be inserted.
		 */
		int GetIndex(std::shared_ptr<GameObject> gameObject) const
		{
			int index = -1;

			// Calculate mid points.
			const float midDepth = (m_bounds.GetNear() - m_bounds.GetFar()) * 0.5f;
			const float midHeight = (m_bounds.GetTop() - m_bounds.GetBottom()) * 0.5f;
			const float midWidth = (m_bounds.GetRight() - m_bounds.GetLeft()) * 0.5f;

			// Ensure required attributes.
			assert(gameObject->HasAttribute<Attribute::Transform>());
			assert(gameObject->HasAttribute<Attribute::BoundingSphere>());

			// Get the Game Object's transform attribute.
			std::shared_ptr<Attribute::Transform> transform =
				gameObject->GetAttribute<Attribute::Transform>();

			// Get the Game Object's bounding sphere attribute.
			std::shared_ptr<Attribute::BoundingSphere> boundingSphere =
				gameObject->GetAttribute<Attribute::BoundingSphere>();

			// Get the Game Object's position in scene coordinates.
			const glm::vec3 position = transform->GetPosition();

			// Get the scaled bounding sphere radius.
			const float radius = boundingSphere->GetRadius() * glm::compMax(transform->GetScale());

			// Determine whether the Game Object can completely fit within the
			// near four nodes or bottom four nodes.
			const bool fitsInNear = (position.z - radius) > midDepth;
			const bool fitsInFar = (position.z + radius) < midDepth;

			// Determine whther the Game Object can completely fit within the
			// top four nodes or bottom four nodes.
			const bool fitsInTop = (position.y - radius) > midHeight;
			const bool fitsInBottom = (position.y + radius) < midHeight;

			// Determine whther the Game Object can completely fit within the
			// left four nodes or right four nodes.
			const bool fitsInLeft = (position.x + radius) < midWidth;
			const bool fitsInRight = (position.x - radius) > midWidth;

			// Determine the exact node index.
			if (fitsInNear)
			{
				if (fitsInTop)
				{
					if (fitsInLeft)
					{
						return 0;
					}
					else if (fitsInRight)
					{
						return 1;
					}
				}
				else if (fitsInBottom)
				{
					if (fitsInLeft)
					{
						return 2;
					}
					else if (fitsInRight)
					{
						return 3;
					}
				}
			}
			else if (fitsInFar)
			{
				if (fitsInTop)
				{
					if (fitsInLeft)
					{
						return 4;
					}
					else if (fitsInRight)
					{
						return 5;
					}
				}
				else if (fitsInBottom)
				{
					if (fitsInLeft)
					{
						return 6;
					}
					else if (fitsInRight)
					{
						return 7;
					}
				}
			}

			// If we reach this point then the Game Object cannot be completely
			// contained by any of the sub-nodes.
			return -1;
		}

	private:
		/**
		 * The octree node level.
		 */
		unsigned int m_nodeLevel;

		/**
		 * The octree bounding space.
		 */
		Cuboid<float> m_bounds;

		/**
		 * The Game Objects contained within the node.
		 */
		std::vector<std::shared_ptr<GameObject>> m_objects;

		/**
		 * The sub-nodes.
		 */
		std::vector<std::unique_ptr<Octree>> m_nodes;

	private:
		/**
		 * The maximum number of objects that a node can contain before
		 * splitting.
		 */
		static constexpr int MAX_OBJECTS = 10;

		/**
		 * The maximum number of levels that the root quadtree can contain.
		 */
		static constexpr int MAX_LEVELS = 5;
	};
}

#endif
