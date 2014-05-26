#include "PlayingSurface.hpp"

#include <iostream>

PlayingSurface::PlayingSurface(const glm::vec2& dimensions, unsigned int rows, unsigned int columns)
: Grid<Engine::GameObject>(rows, columns)
, m_dimensions(dimensions)
{
	// Nothing to do.
}

PlayingSurface::~PlayingSurface()
{
	// Nothing to do.
}

const glm::vec2& PlayingSurface::GetDimensions() const
{
	return m_dimensions;
}

glm::vec2 PlayingSurface::GetCellDimensions() const
{
	return m_dimensions / glm::vec2(static_cast<float>(GetColumns()), static_cast<float>(GetRows()));
}

Cell PlayingSurface::GetCellForPosition(const glm::vec3& position) const
{
	const glm::vec2 originCell(0.5f * GetColumns(), 0.5f * GetRows());
	const glm::vec2 cell = originCell + glm::vec2(position.x, position.z) / GetCellDimensions();
	return Cell(static_cast<int>(cell.y), static_cast<int>(cell.x));
}

glm::vec3 PlayingSurface::GetPositionForCell(Cell cell) const
{
	const glm::vec2 fCell(static_cast<float>(cell.m_column), static_cast<float>(cell.m_row));
	const glm::vec2 cellPosition((fCell + 0.5f) * GetCellDimensions() - (GetDimensions() * 0.5f));
	return glm::vec3(cellPosition.x, 0.0f, cellPosition.y);
}
