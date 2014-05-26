#include "Grid.hpp"

#include <cassert>

Path::Path()
: m_path()
{
	// Nothing to do.
}

Path::~Path()
{
	// Nothing to do.
}

unsigned int Path::GetLength() const
{
	return m_path.size();
}

Cell Path::GetCell(unsigned int index) const
{
	assert(index < m_path.size());
	return m_path[index];
}

void Path::AppendCell(Cell cell)
{
	m_path.push_back(cell);
}

void Path::PrependCell(Cell cell)
{
	m_path.push_front(cell);
}

void Path::RemoveCell(unsigned int index)
{
	assert(index < m_path.size());
	m_path.erase(m_path.begin() + index);
}
