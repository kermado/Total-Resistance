#include "Behaviour/NavigateToCell.hpp"

#include <cassert>

#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>

#include <Engine/Event/DestroyGameObjectEvent.hpp>

#include "Event/EnemySurvivedEvent.hpp"
#include "PathFinder.hpp"

namespace Behaviour
{
	NavigateToCell::NavigateToCell(std::shared_ptr<Engine::Window> window,
		std::shared_ptr<Engine::ResourceManager> resourceManager,
		std::shared_ptr<Engine::EventDispatcher> sceneEventDispatcher,
		std::shared_ptr<Engine::EventDispatcher> gameObjectEventDispatcher,
		std::weak_ptr<Engine::GameObject> gameObject,
		std::shared_ptr<Engine::Attribute::Transform> transformAttribute,
		const PlayingSurface& playingSurface,
		const Cell& target,
		float speed)
	: IBehaviour(window, resourceManager, sceneEventDispatcher, gameObjectEventDispatcher, gameObject)
	, m_transformAttribute(transformAttribute)
	, m_playingSurface(playingSurface)
	, m_target(target)
	, m_speed(speed)
	, m_playingSurfaceModifiedSubscription(0)
	{
		// Subscribe to be notified when the playing surface has been modifed.
		// This can occur when towers have been added to the playing surface.
		m_playingSurfaceModifiedSubscription = GetSceneEventDispatcher()->Subscribe<Event::PlayingSurfaceModifiedEvent>(CALLBACK(NavigateToCell::HandlePlayingSurfaceModifiedEvent));

		// Get the game object's current position.
		const glm::vec3 currentPosition = m_transformAttribute->GetPosition();

		// Get the cell that corresponds to the game object's current position.
		const Cell currentCell = m_playingSurface.GetCellForPosition(currentPosition);

		// Find the shortest path from the game object's current cell to the
		// target cell over the playing surface.
		m_shortestPath = PathFinder::ShortestPath(m_playingSurface, currentCell, m_target);
		assert(m_shortestPath.GetLength() >= 1);
	}

	NavigateToCell::~NavigateToCell()
	{
		// Unsubscribe for events.
		GetSceneEventDispatcher()->Unsubscribe<Event::PlayingSurfaceModifiedEvent>(m_playingSurfaceModifiedSubscription);
	}

	void NavigateToCell::Update(double deltaTime)
	{
		// Ensure that we have not arrived at the target cell yet.
		if (m_shortestPath.GetLength() <= 0)
		{
			return;
		}

		// Get the game object's current position.
		const glm::vec3 currentPosition = m_transformAttribute->GetPosition();

		// Get the next cell.
		const Cell nextCell = m_shortestPath.GetCell(0);

		// Get the position of the next cell.
		glm::vec3 nextCellPosition = m_playingSurface.GetPositionForCell(nextCell);
		nextCellPosition.y = currentPosition.y; // Ignore the cell y-coordinate.

		// Get the vector from the game object to the next cell.
		const glm::vec3 vectorToNextCell = nextCellPosition - currentPosition;

		// Calculate the distance to the next cell.
		const float nextCellDistance = glm::length(vectorToNextCell);

		// Calculate the maximum possible distance that can be travelled.
		const float maximumDistance = m_speed * deltaTime;

		// If the maximum possible distance is greater than the distance to the
		// next cell, then move the game object to the position of the next cell
		// and remove this cell from the path. Otherwise, move the game object
		// in the direction of the next cell by the maximum distance.
		if (maximumDistance >= nextCellDistance)
		{
			m_transformAttribute->SetPosition(nextCellPosition);
			m_shortestPath.RemoveCell(0);

			// Check whether we have just arrived at our destination cell.
			if (m_shortestPath.GetLength() <= 0)
			{
				// Publish an event to inform others that the enemy reached its
				// target cell.
				GetSceneEventDispatcher()->Enqueue<Event::EnemySurvivedEvent>();

				// Request to destroy the Game Object.
				GetGameObjectEventDispatcher()->Enqueue<Engine::Event::DestroyGameObjectEvent>();
			}
		}
		else
		{
			// Move the game object by the maximum distance possible towards
			// the next cell.
			m_transformAttribute->Translate(glm::normalize(vectorToNextCell) * maximumDistance);

			// Rotate the game object to look at the next cell.
			m_transformAttribute->LookAt(nextCellPosition);
		}
	}

	void NavigateToCell::HandlePlayingSurfaceModifiedEvent(const Event::PlayingSurfaceModifiedEvent& event)
	{
		// Recalculate the shortest path.
		// Note that it is not sufficient to only recalculate the shortest path
		// if one of the cells in the current shortest path has been modified.
		// This is because cells can be vacated, resulting in an even shorter
		// possible path to the destination cell.
		m_playingSurface = event.GetPlayingSurface();
		const glm::vec3 currentPosition = m_transformAttribute->GetPosition();
		const Cell currentCell = m_playingSurface.GetCellForPosition(currentPosition);
		m_shortestPath = PathFinder::ShortestPath(m_playingSurface, currentCell, m_target);

		// Check whether we have passed the centroid position of the cell that
		// we are currently inside and if so, remove this cell from the path.
		if (m_shortestPath.GetLength() >= 2)
		{
			const Cell subsequentCell = m_shortestPath.GetCell(1);
			const glm::vec3 subsequentCellPosition = m_playingSurface.GetPositionForCell(subsequentCell);

			if (glm::distance2(currentPosition, subsequentCellPosition) < glm::length2(m_playingSurface.GetCellDimensions()))
			{
				m_shortestPath.RemoveCell(0);
			}
		}
	}
}
