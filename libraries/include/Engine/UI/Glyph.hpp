#ifndef GLYPH_H
#define GLYPH_H

#include <Engine/Rectangle.hpp>

namespace Engine
{
	namespace UI
	{
		struct Glyph
		{
			/**
			 * Constructor.
			 */
			Glyph();

			/**
			 * Copy constructor.
			 */
			Glyph(const Glyph& other);

			/**
			 * Destructor.
			 */
			~Glyph();

			/**
			 * Horizontal offset to move to the starting position of the next
			 * glyph.
			 */
			int advance;

			/**
			 * The bounding rectangle for the glyph, relative to the baseline.
			 */
			Engine::Rectangle<int> bounds;

			/**
			 * The texture coordinates for the glyph inside of the font's
			 * texture.
			 */
			Engine::Rectangle<int> textureRectangle;
		};
	}
}

#endif
