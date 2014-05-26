#include <Engine/UI/Glyph.hpp>

namespace Engine
{
	namespace UI
	{
		Glyph::Glyph()
		: advance(0)
		, bounds()
		, textureRectangle()
		{
			// Nothing to do.
		}

		Glyph::Glyph(const Glyph& other)
		: advance(other.advance)
		, bounds(other.bounds)
		, textureRectangle(other.textureRectangle)
		{
			// Nothing to do.
		}

		Glyph::~Glyph()
		{
			// Nothing to do.
		}
	}
}
