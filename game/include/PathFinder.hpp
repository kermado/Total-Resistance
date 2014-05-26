#ifndef PATHFINDER_H
#define PATHFINDER_H

#include "Grid.hpp"

/**
 * Contains static functions for finding paths in a grid.
 */
class PathFinder
{
public:
	/**
	 * Returns a shortest path of unoccupied cells, starting at the specified
	 * start cell and ending at the specified end cell in the provided grid.
	 *
	 * @note The implementation of the A* algorithm used is not optimal.
	 *
	 * @param grid The grid on which to search for a shortest path.
	 * @param start The starting cell.
	 * @param end The ending cell.
	 * @return A shortest path.
	 */
	static Path ShortestPath(const Grid<Engine::GameObject>& grid, Cell start, Cell end);
};

#endif
