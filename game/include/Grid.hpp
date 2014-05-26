#ifndef GRID_H
#define	GRID_H

#include <vector>
#include <memory>
#include <deque>

#include <Engine/GameObject.hpp>

/**
 * Represents the location of a cell in the grid.
 */
struct Cell
{
	/**
	 * Default constructor.
	 */
	Cell()
	: m_row(0)
	, m_column(0)
	{
		// Nothing to do.
	}

	/**
	 * Constructor.
	 *
	 * @param rows Row for the cell.
	 * @param columns Column for the cell.
	 */
	Cell(unsigned int row, unsigned int column)
	: m_row(row)
	, m_column(column)
	{
		// Nothing to do.
	}

	/**
	 * Equality operator.
	 *
	 * @param other The other cell.
	 * @return True if the two cells are equal.
	 */
	bool operator==(const Cell& other) const
	{
		return (m_row == other.m_row) && (m_column == other.m_column);
	}

	/**
	 * Non-equality operator.
	 *
	 * @param other The other cell.
	 * @return True if the two cells are not equal.
	 */
	bool operator!=(const Cell& other) const
	{
		return (m_row != other.m_row) || (m_column != other.m_column);
	}

	/**
	 * Returns the Manhattan distance from this cell to the other
	 * specified cell. The distance is returned in units: "cells".
	 *
	 * @return Manhattan distance between the two cells.
	 */
	unsigned int ManhattanDistanceTo(const Cell& other)
	{
		return abs(m_row - other.m_row) + abs(m_column - other.m_column);
	}

	/**
	 * The row for the cell.
	 */
	int m_row;

	/**
	 * The column for the cell.
	 */
	int m_column;
};

/**
 * A sequence of cells that represents a path through a grid.
 */
class Path
{
public:
	/**
	 * Constructor.
	 */
	Path();

	/**
	 * Destructor.
	 */
	~Path();

	/**
	 * Returns the length of the path.
	 *
	 * @return Path length.
	 */
	unsigned int GetLength() const;

	/**
	 * Returns the index-th cell in the path.
	 *
	 * @param index Index for the cell to return.
	 * @return Index-th cell in the path.
	 */
	Cell GetCell(unsigned int index) const;

	/**
	 * Appends the specified cell to the end of the path.
	 *
	 * @param cell Cell to append to the end of the path.
	 */
	void AppendCell(Cell cell);

	/**
	 * Prepends the specified cell to the front of the path.
	 *
	 * @param cell Cell to prepend to the front of the path.
	 */
	void PrependCell(Cell cell);

	/**
	 * Removes the index-th cell in the path.
	 *
	 * @param index Index for the cell to remove.
	 */
	void RemoveCell(unsigned int index);

private:
	/**
	 * The path data structure.
	 */
	std::deque<Cell> m_path;
};

/**
 * Provides a simple interface for occupying, vacating and inspecting cells in a
 * 2D grid.
 */
template <typename OccupantType>
class Grid
{
public:
	/**
	 * Constructor.
	 *
	 * @param rows Number of rows in the grid.
	 * @param columns Number of columns in the grid.
	 */
	Grid(unsigned int rows, unsigned int columns)
	: m_rows(rows)
	, m_columns(columns)
	, m_grid()
	{
		// Resize the grid data structure to allocate enough memory in order
		// to store the specified number of occupants.
		m_grid.resize(rows * columns);
	}

	/**
	 * Destructor.
	 */
	~Grid()
	{
		// Nothing to do.
	}

	/**
	 * Returns the number of rows in the grid.
	 *
	 * @return Rows in the grid.
	 */
	unsigned int GetRows() const
	{
		return m_rows;
	}

	/**
	 * Returns the number of columns in the grid.
	 *
	 * @return Number of columns in the grid.
	 */
	unsigned int GetColumns() const
	{
		return m_columns;
	}

	/**
	 * Returns true if the specified cell is within the bounds of the grid.
	 *
	 * @param cell Cell to check.
	 * @return True if the cell is within the bounds of the grid.
	 */
	bool ContainsCell(Cell cell) const
	{
		return (cell.m_row >= 0) && (cell.m_row < m_rows) && (cell.m_column >= 0) && (cell.m_column < m_columns);
	}

	/**
	 * Returns a shared pointer to the occupant of the specified cell.
	 *
	 * @param cell Cell to inspect.
	 * @return Shared pointer to the cell's occupant.
	 */
	std::shared_ptr<OccupantType> GetOccupant(Cell cell) const
	{
		assert(ContainsCell(cell));

		return m_grid[cell.m_column * m_columns + cell.m_row];
	}

	/**
	 * Occupies the specified cell.
	 *
	 * @param cell Cell to occupy.
	 * @param occupant Shared pointer to the occupant that should occupy the
	 * specified cell.
	 */
	void OccupyCell(Cell cell, std::shared_ptr<OccupantType> occupant)
	{
		assert(ContainsCell(cell));
		assert(!m_grid[cell.m_column * m_columns + cell.m_row]);

		// Occupy the cell.
		m_grid[cell.m_column * m_columns + cell.m_row] = occupant;
	}

	/**
	 * Vacates the specified cell and returns a shared pointer to the previous
	 * occupant.
	 *
	 * @param cell Cell to vacate.
	 * @return Shared pointer to the previous occupant.
	 */
	std::shared_ptr<OccupantType> VacateCell(Cell cell)
	{
		assert(ContainsCell(cell));
		assert(m_grid[cell.m_column * m_columns + cell.m_row]);

		// Get the occupant.
		std::shared_ptr<OccupantType> occupant = m_grid[cell.m_column * m_columns + cell.m_row];

		// Vacate the cell.
		m_grid[cell.m_column * m_columns + cell.m_row] = nullptr;

		// Return the previous occupant.
		return occupant;
	}

private:
	/**
	 * Number of rows in the grid.
	 */
	unsigned int m_rows;

	/**
	 * Number of columns in the grid.
	 */
	unsigned int m_columns;

	/**
	 * The grid data structure.
	 *
	 * @note This is a 1D data structure, with occupants stored in row-major
	 * order. Unoccupied cells are represented by containing a nullptr.
	 */
	std::vector<std::shared_ptr<OccupantType>> m_grid;
};

#endif
