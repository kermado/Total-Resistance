#ifndef CUBOID_H
#define CUBOID_H

namespace Engine
{
	template <typename T>
	class Cuboid
	{
	public:
		/**
		 * Default constructor.
		 *
		 * Initializes all coordinates to zero.
		 */
		Cuboid()
		: m_left(0)
		, m_right(0)
		, m_top(0)
		, m_bottom(0)
		, m_near(0)
		, m_far(0)
		{
			// Nothing to do.
		}

		/**
		 * Constructor.
		 *
		 * Initializes all coordinates to zero.
		 */
		Cuboid(T left, T right, T top, T bottom, T near, T far)
		: m_left(left)
		, m_right(right)
		, m_top(top)
		, m_bottom(bottom)
		, m_near(near)
		, m_far(far)
		{
			// Nothing to do.
		}

		/**
		 * Copy constructor.
		 */
		Cuboid(const Cuboid& other)
		: m_left(other.m_left)
		, m_right(other.m_right)
		, m_top(other.m_top)
		, m_bottom(other.m_bottom)
		, m_near(other.m_near)
		, m_far(other.m_far)
		{
			// Nothing to do.
		}

		/**
		 * Destructor.
		 */
		~Cuboid()
		{
			// Nothing to do.
		}

		/**
		 * Returns the left coordinate of the cuboid.
		 *
		 * @return Left coordinate of the cuboid.
		 */
		T GetLeft() const
		{
			return m_left;
		}

		/**
		 * Sets the left coordinate for the cuboid.
		 *
		 * @param left Left coordinate for the cuboid.
		 */
		void SetLeft(T left)
		{
			m_left = left;
		}

		/**
		 * Returns the right coordinate of the cuboid.
		 *
		 * @return Right coordinate of the cuboid.
		 */
		T GetRight() const
		{
			return m_right;
		}

		/**
		 * Sets the right coordinate for the cuboid.
		 *
		 * @param right Right coordinate for the cuboid.
		 */
		void SetRight(T right)
		{
			m_right = right;
		}

		/**
		 * Returns the top coordinate of the cuboid.
		 *
		 * @return Top coordinate of the cuboid.
		 */
		T GetTop() const
		{
			return m_top;
		}

		/**
		 * Sets the top coordinate for the cuboid.
		 *
		 * @param top Top coordinate for the cuboid.
		 */
		void SetTop(T top)
		{
			m_top = top;
		}

		/**
		 * Returns the bottom coordinate of the cuboid.
		 *
		 * @return Bottom coordinate of the cuboid.
		 */
		T GetBottom() const
		{
			return m_bottom;
		}

		/**
		 * Sets the bottom coordinate for the cuboid.
		 *
		 * @param bottom Bottom coordinate for the cuboid.
		 */
		void SetBottom(T bottom)
		{
			m_bottom = bottom;
		}

		/**
		 * Returns the near coordinate of the cuboid.
		 *
		 * @return Near coordinate of the cuboid.
		 */
		T GetNear() const
		{
			return m_near;
		}

		/**
		 * Sets the near coordinate for the cuboid.
		 *
		 * @param near Near coordinate for the cuboid.
		 */
		void SetNear(T near)
		{
			m_near = near;
		}

		/**
		 * Returns the far coordinate of the cuboid.
		 *
		 * @return Far coordinate of the cuboid.
		 */
		T GetFar() const
		{
			return m_far;
		}

		/**
		 * Sets the far coordinate for the cuboid.
		 *
		 * @param far Near coordinate for the cuboid.
		 */
		void SetFar(T far)
		{
			m_far = far;
		}

	private:
		/**
		 * The left coordinate of the cuboid.
		 */
		T m_left;

		/**
		 * The right coordinate of the cuboid.
		 */
		T m_right;

		/**
		 * The top coordinate of the cuboid.
		 */
		T m_top;

		/**
		 * The bottom coordinate of the cuboid.
		 */
		T m_bottom;

		/**
		 * The near coordinate of the cuboid.
		 */
		T m_near;

		/**
		 * The far coordinate of the cuboid.
		 */
		T m_far;
	};

	/**
	 * Cuboid with integer coordinates.
	 */
	typedef Cuboid<int> IntCuboid;

	/**
	 * Cuboid with floating point coordinates.
	 */
	typedef Cuboid<float> FloatCuboid;

	/**
	 * Cuboid with double precision floating point coordinates.
	 */
	typedef Cuboid<double> DoubleCuboid;
}

#endif
