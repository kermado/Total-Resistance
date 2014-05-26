#include "PathFinder.hpp"

#include <iostream>
#include <array>

Path PathFinder::ShortestPath(const Grid<Engine::GameObject>& grid, Cell start, Cell end)
{
	// Bounds checking.
	assert(grid.ContainsCell(start) && grid.ContainsCell(end));

	// Grid dimensions.
	const unsigned int rows = grid.GetRows();
	const unsigned int columns = grid.GetColumns();

	// Closed list.
	std::vector<Cell> closedList;
	closedList.reserve(rows * columns);

	// Open list.
	std::vector<Cell> openList;
	openList.reserve(rows * columns);

	// Cell parents.
	std::vector<Cell> parents;
	parents.resize(rows * columns);

	// G scores.
	std::vector<unsigned int> gScores;
	gScores.resize(rows * columns);

	// F scores.
	std::vector<unsigned int> fScores;
	fScores.resize(rows * columns);

	// We start by adding the starting cell to the open list.
	openList.push_back(start);

	// Set the scores for the starting cell.
	gScores[columns * start.m_row + start.m_column] = 0;
	fScores[columns * start.m_row + start.m_column] = start.ManhattanDistanceTo(end);

	while (!openList.empty())
	{
		// Find the cell in the open list with the lowest F score.
		Cell current = openList.front();
		for (Cell cell : openList)
		{
			if (fScores[columns * cell.m_row + cell.m_column] < fScores[columns * current.m_row + current.m_column])
			{
				current = cell;
			}
		}

		// If the current cell just added to the closed list is the ending
		// cell, then we have found the path.
		if (current == end)
		{
			// Return the complete path, from the starting cell to the
			// ending cell.
			Path path;
			for (Cell cell = end; cell != start; cell = parents[columns * cell.m_row + cell.m_column])
			{
				path.PrependCell(cell);
			}
			path.PrependCell(start);
			return path;
		}

		// Move the current cell from the open list to the closed list.
		openList.erase(std::remove(openList.begin(), openList.end(), current), openList.end());
		closedList.push_back(current);

		// Determine all the nodes adjacent to the current node within
		// the bounds of the grid.
		std::array<Cell, 4> adjacent;
		adjacent[0] = Cell(current.m_row - 1, current.m_column); // top
		adjacent[1] = Cell(current.m_row, current.m_column - 1); // left
		adjacent[2] = Cell(current.m_row + 1, current.m_column); // bottom
		adjacent[3] = Cell(current.m_row, current.m_column + 1); // right

		// Shuffle the vector of adjacent cells.
		// This ensures that a shorest path will be selected at random if
		// multiple shortest paths exist.
		std::random_shuffle(adjacent.begin(), adjacent.end());

		// Consider each neighbour...
		for (Cell neighbour : adjacent)
		{
			// Skip if the neighbour is outside the bounds of the grid,
			// is currently occupied or is already in the closed list.
			if (!grid.ContainsCell(neighbour) ||
				grid.GetOccupant(neighbour) ||
				std::find(closedList.begin(), closedList.end(), neighbour) != closedList.end())
			{
				continue;
			}

			// Calculate the neighbour's scores.
			// The neighbour's G score is the G score of the current cell plus the
			// distance from the current cell to the neighbour (which is always 1).
			const unsigned int g = gScores[columns * current.m_row + current.m_column] + 10;
			const unsigned int h = neighbour.ManhattanDistanceTo(end);

			// If the neighbour is not in the open list then we add it to
			// the open list and make the current cell the neighbour's
			// parent.
			if (std::find(openList.begin(), openList.end(), neighbour) == openList.end())
			{
				openList.push_back(neighbour);
				parents[columns * neighbour.m_row + neighbour.m_column] = current;

				// Update the neighbour's scores.
				gScores[columns * neighbour.m_row + neighbour.m_column] = g;
				fScores[columns * neighbour.m_row + neighbour.m_column] = g + h;
			}
			else
			{
				// If the neighbour is already in the open list, then we check to
				// see if the path to the neighbour via the path to the current
				// cell is shorter. If it is, then we update the path to the
				// neighbour.
				if (g < gScores[columns * neighbour.m_row + neighbour.m_column])
				{
					parents[columns * neighbour.m_row + neighbour.m_column] = current;
					gScores[columns * neighbour.m_row + neighbour.m_column] = g;
					fScores[columns * neighbour.m_row + neighbour.m_column] = g + h;
				}
			}
		}
	}

	// Path not found - return an empty path.
	return Path();
}
