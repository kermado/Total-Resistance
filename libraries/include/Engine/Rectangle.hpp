#ifndef RECTANGLE_H
#define RECTANGLE_H

namespace Engine
{
	template<typename T>
	class Rectangle
	{
	public:
		/**
		 * Default constructor.
		 *
		 * Creates a rectangle of zero dimensions located at the origin.
		 */
		Rectangle()
		: m_left(0)
		, m_top(0)
		, m_width(0)
		, m_height(0)
		{
			// Nothing to do.
		}

		/**
		 * Constructor.
		 *
		 * @param left Left coordinate for the rectange.
		 * @param Top coordinate for the rectangle.
		 * @param width Width for the rectangle.
		 * @param height Height for the rectangle.
		 */
		Rectangle(T left, T top, T width, T height)
		: m_left(left)
		, m_top(top)
		, m_width(width)
		, m_height(height)
		{
			// Nothing to do.
		}

		/**
		 * Returns the left coordinate of the rectangle.
		 *
		 * @return Left coordinate of the rectangle.
		 */
		T GetLeft() const
		{
			return m_left;
		}

		/**
		 * Sets the left coordinate for the rectange.
		 *
		 * @param left Left coordinate for the rectangle.
		 */
		void SetLeft(T left)
		{
			m_left = left;
		}

		/**
		 * Returns the top coordinate of the rectangle.
		 *
		 * @return Top coordinate of the rectangle.
		 */
		T GetTop() const
		{
			return m_top;
		}

		/**
		 * Sets the top coordinate for the rectange.
		 *
		 * @param top Top coordinate for the rectangle.
		 */
		void SetTop(T top)
		{
			m_top = top;
		}

		/**
		 * Returns the width of the rectangle.
		 *
		 * @return Width of the rectangle.
		 */
		T GetWidth() const
		{
			return m_width;
		}

		/**
		 * Sets the width for the rectangle.
		 *
		 * @param width Width for the rectangle.
		 */
		void SetWidth(T width)
		{
			m_width = width;
		}

		/**
		 * Returns the height of the rectangle.
		 *
		 * @return Height of the rectangle.
		 */
		T GetHeight() const
		{
			return m_height;
		}

		/**
		 * Sets the height for the rectangle.
		 *
		 * @param height Height for the rectangle.
		 */
		void SetHeight(T height)
		{
			m_height = height;
		}


	private:
		/**
		 * The left coordinate of the rectangle.
		 */
		T m_left;

		/**
		 * The top coordinate of the rectangle.
		 */
		T m_top;

		/**
		 * The width of the rectangle.
		 */
		T m_width;

		/**
		 * The height of the rectangle.
		 */
		T m_height;
	};

	/**
	 * Rectangle with integer coordinates and dimensions.
	 */
	typedef Rectangle<int> IntRectangle;

	/**
	 * Rectangle with floating point coordinates and dimensions.
	 */
	typedef Rectangle<float> FloatRectangle;

	/**
	 * Rectangle with double precision floating point coordinates and
	 * dimensions.
	 */
	typedef Rectangle<double> DoubleRectangle;
}

#endif
