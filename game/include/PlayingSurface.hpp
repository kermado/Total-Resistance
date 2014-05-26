#ifndef PLAYINGSURFACE_H
#define PLAYINGSURFACE_H

#include <glm/glm.hpp>

#include <Engine/GameObject.hpp>

#include "Grid.hpp"

/**
 * Represents the playing surface over which enemy units move and towers are
 * placed.
 *
 * @note The playing surface's center is assumed to be positioned at the origin
 * and parallel to the xz-axis.
 */
class PlayingSurface : public Grid<Engine::GameObject>
{
public:
	/**
	 * Constructor.
	 *
	 * @param dimensions Dimensions (width, depth) for the playing surface.
	 * @param rows Number of rows to divide the playing surface depth into.
	 * @param columns Number of columns to divide the playing surface width
	 * into.
	 */
	PlayingSurface(const glm::vec2& dimensions, unsigned int rows, unsigned int columns);

	/**
	 * Destructor.
	 */
	~PlayingSurface();

	/**
	 * Returns the playing surface's dimensions.
	 *
	 * @return Playing surface dimensions (width, depth).
	 */
	const glm::vec2& GetDimensions() const;

	/**
	 * Returns the dimensions of each cell in the grid.
	 *
	 * @return Dimensions for each cell.
	 */
	glm::vec2 GetCellDimensions() const;

	/**
	 * Returns the cell that encompasses the specified point.
	 *
	 * @note The y-coordinate of the position supplied is ignored.
	 *
	 * @param position Point for which to determine the corresponding cell.
	 * @return Cell that encompasses the specified point.
	 */
	Cell GetCellForPosition(const glm::vec3& position) const;

	/**
	 * Returns the centroid position of the specified cell.
	 *
	 * @param cell Cell for which to return the centroid position.
	 * @return Centroid position of the specified cell.
	 */
	glm::vec3 GetPositionForCell(Cell cell) const;

private:
	/**
	 * The dimensions of the playing surface.
	 */
	glm::vec2 m_dimensions;
};

#endif
