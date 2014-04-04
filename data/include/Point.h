#ifndef MDSEARCH_POINT_H
#define MDSEARCH_POINT_H

#include <iostream>
#include <vector>
#include "DataTypes.h"

// Only define this if points should use SSE-enabled comparison
//#define MDSEARCH_USE_SSE_POINT_COMPARISON 1

#if !defined(WIN32) && MDSEARCH_USE_SSE_POINT_COMPARISON
	#include <xmmintrin.h>
#endif

namespace mdsearch
{

	class Point
	{

	public:
		Point(int n, Real initialValue = 0);
		Point(int n, const Real* initialValues);

		// Inline functions have been declared in the header file
		// to prevent linker issues
		// Equality operators
		inline bool operator==(const Point& other) const
		{
			// Ensure the two points have the same dimensionality
			if (nDimensions != other.nDimensions)
			{
				return false;
			}
			else
			{
				// NOTE: The direct cast (__m128i)... only works on non-Windows systems.
				// The cast is critical to the SSE comparison, so Windows binaries
				// currently will not use SSE-accelerated point comparison.
				#if defined(WIN32) || !defined(MDSEARCH_USE_SSE_POINT_COMPARISON)
					// Perform equality check element-by-element
					for (int i = 0; (i < nDimensions); i++)
						if (values[i] != other.values[i]) // TODO: float equality? how to check?
							return false;
					return true;
				#else
					int numLoops = nDimensions / 4;
					if ((nDimensions % 4) != 0)
						numLoops += 1;
					// Input data recast to 128-bit values (containing four values each)
					const __m128* mA = reinterpret_cast<const __m128*>(&values[0]);
					const __m128* mB = reinterpret_cast<const __m128*>(&other.values[0]);
					__m128i mask;

					for (unsigned int i = 0; (i < numLoops); i++)
					{
						mask = (__m128i)_mm_cmpeq_ps(*mA, *mB); 
						unsigned short test = _mm_movemask_epi8(mask);
						if (test != 0xFFFF) // if all elements are NOT equal
							return false;

						mA++;
						mB++;
					}
					return true;
				#endif
			}
		}

		inline bool operator!=(const Point& other) const
		{
			return !(*this == other);
		}

		inline Real& operator[](int index) { return values[index]; }
		inline Real operator[](int index) const { return values[index]; }
		inline unsigned int numDimensions() const { return nDimensions; }

		/* Return point's coordinates as a C-array. */
		inline Real* toArray() { return &values[0]; }
		inline const Real* toArray() const { return &values[0]; }

		/* Return sum of all elements/coordinates of this point. */
		inline Real sum() const
		{
			Real s = 0;
			for (int i = 0; (i < nDimensions); i++)
				s += values[i];
			return s;
		}

	private:
		RealList values;
		unsigned int nDimensions;

	};

	std::ostream& operator<<(std::ostream& out, const Point& point);
	
	typedef std::vector<Point> PointList;

}

#endif