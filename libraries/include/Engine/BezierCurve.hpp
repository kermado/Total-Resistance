#ifndef BEZIERCURVE_H
#define BEZIERCURVE_H

#include <array>
#include <cassert>

#include <iostream>

namespace Engine
{
	template<unsigned int N, typename T>
	class BezierCurve
	{
	public:
		/**
		 * Constructor.
		 *
		 * @param points... The set of control points that define the curve.
		 */
		template <typename... Arguments>
		BezierCurve(Arguments... points)
		: m_points{{points...}}
		{
			// Ensure that we were supplied with the correct number of
			// control points.
			assert(sizeof...(points) == N);
		}

		/**
		 * Constructor.
		 *
		 * @param points The array of control points that should be used to
		 * define the curve.
		 */
		BezierCurve(const std::array<T, N>& points)
		: m_points(points)
		{
			// Nothing to do.
		}

		/**
		 * Copy constructor.
		 *
		 * @param other The other bezier curve to instantiate this bezier curve
		 * from.
		 */
		BezierCurve(const BezierCurve<N, T>& other)
		: m_points(other.m_points)
		{
			// Nothing to do.
		}

		/**
		 * Copy assignment operator.
		 *
		 * @param rhs The other bezier curve to instantiate this bezier curve
		 * from.
		 */
		BezierCurve<N, T>& operator=(const BezierCurve<N, T>& rhs)
		{
			if (this != &rhs)
			{
				m_points = rhs.m_points;
			}

			return *this;
		}

		/**
		 * Destructor.
		 */
		~BezierCurve()
		{
			// Nothing to do.
		}

		/**
		 * Returns the point on the curve that corresponds to the specified
		 * time.
		 *
		 * @param t The time for which to evaluate the curve. This should be in
		 * the range [0, 1].
		 * @return Point on the curve corresponding to the specified time.
		 */
		T GetPointAtTime(float t) const
		{
			// Ensure that the time supplied is in the range [0, 1].
			assert(t >= 0.0f && t <= 1.0f);

			// Evaluate and return the general n-point bezier curve equation.
			T result;
			for (int i = 0; i <= N - 1; ++i)
			{
				result += static_cast<float>(BinomialCoeffient(N - 1, i))
						* static_cast<float>(std::pow(1.0f - t, N - 1 - i))
						* static_cast<float>(std::pow(t, i))
						* m_points[i];
			}
			return result;
		}

		/**
		 * Returns the slope of the tangent line to the curve at the specified
		 * time.
		 *
		 * @param t The time for which to evaluate the derivative of the bezier
		 * curve. This should be in the range [0, 1].
		 * @return Slope of the tangent line to the curve corresponding to the
		 * specified time.
		 */
		T GetTangentSlopeAtTime(float t) const
		{
			// Ensure that the time supplied is in the range [0, 1].
			assert(t >= 0.0f && t <= 1.0f);

			// Evaluate and return the derivative of the n-point bezier curve
			// equation.
			T result;
			for (int i = 0; i <= N - 2; ++i)
			{
				result += static_cast<float>(BinomialCoeffient(N - 2, i))
						* static_cast<float>(std::pow(1.0f - t, N - 2 - i))
						* static_cast<float>(std::pow(t, i))
						* static_cast<float>(N - 1)
						* (m_points[i + 1] - m_points[i]);
			}
			return result;
		}

		/**
		 * Equality operator.
		 *
		 * @param rhs The other bezier curve to check for equality with.
		 */
		bool operator==(const BezierCurve<N, T>& rhs)
		{
			return m_points == rhs.m_points;
		}

		/**
		 * Inequality operator.
		 *
		 * @param rhs The other bezier curve to check for inequality with.
		 */
		bool operator!=(const BezierCurve<N, T>& rhs)
		{
			return m_points != rhs.m_points;
		}

		/**
		 * Concatenation operator.
		 *
		 * @param rhs The other bezier curve to concatenate to the end of this
		 * curve.
		 */
		template <unsigned int OtherN>
		BezierCurve<N + OtherN, T> operator+(const BezierCurve<OtherN, T>& rhs)
		{
			// Create an array that contains all of the points by concatenating
			// the arrays of points that define both curves.
			std::array<T, N + OtherN> points;
			std::copy(m_points.begin(), m_points.end(), points.begin());
			std::copy(rhs.m_points.begin(), rhs.m_points.end(), points.begin() + m_points.size());

			// Return the new bezier curve.
			return BezierCurve<N + OtherN, T>{ points };
		}

	private:
		/**
		 * Calculates and returns the binomial coefficient "n choose k". This
		 * is the number of distinct k-element subsets that can be formed from
		 * a set of size n.
		 *
		 * @param n Population size.
		 * @param k Number of elements to sample (without replacement) from the
		 * population.
		 * @return The binomial coefficient, "n choose k".
		 */
		static int BinomialCoeffient(int n, int k)
		{
			int result = 1;
			for (unsigned int i = 1; i <= k; ++i)
			{
				result *= ((n - (k - i)) / i);
			}
			return result;
		}

	private:
		/**
		 * The control points that define the curve.
		 */
		std::array<T, N> m_points;
	};
}

#endif
