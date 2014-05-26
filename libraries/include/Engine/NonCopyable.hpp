#ifndef NONCOPYABLE_H
#define	NONCOPYABLE_H

namespace Engine
{
	class NonCopyable
	{
	protected:
		/**
		 * Default constructor.
		 */
		NonCopyable() {}

	private:
		/**
		 * Private (disabled) copy constructor.
		 */
		NonCopyable(const NonCopyable&);

		/**
		 * Private (disabled) assignment perator.
		 */
		NonCopyable& operator =(const NonCopyable&);
	};
}

#endif