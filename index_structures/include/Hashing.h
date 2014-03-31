#ifndef MDSEARCH_HASHING_H
#define MDSEARCH_HASHING_H

#include <xmmintrin.h>
#include <vector>
#include "Point.h"
#include "Region.h"

namespace mdsearch
{


	inline int hashPointSSE(unsigned int numDimensions, const Point& point,
		const Point& minPoint, const Point& maxPoint, const Point& median)
	{
		// TODO: handle dimensions not divisible by four
		int numLoops = numDimensions / 4;

		const __m128* mPoint = reinterpret_cast<const __m128*>(point.toArray());
		const __m128* mBoundaryMin = reinterpret_cast<const __m128*>(minPoint.toArray());
		const __m128* mBoundaryMax = reinterpret_cast<const __m128*>(maxPoint.toArray());
		const __m128* mMedian = reinterpret_cast<const __m128*>(median.toArray());

		__m128 m1, m2, m3, m4;
		float components[4];

		int hashValue = 0;
		for (int i = 0; (i < numLoops); i++)
		{
			m1 = _mm_sub_ps(*mPoint, *mBoundaryMin);
			m2 = _mm_sub_ps(*mBoundaryMax, *mBoundaryMin);
			m3 = _mm_div_ps(m1, m2);
			m1 = _mm_mul_ps(m3, *mMedian);
			_mm_store_ps(components, m1);
			// NOTE: Reason first addition is wrapped in paranthesis is so its all
			// cast to an integer, so each float will be truncated before adding,
			// rather than adding all the floats and THEN truncating. This will
			// given slightly different results to the original, non-SSE hashing
			// algorithm.
			hashValue += static_cast<int>(components[0]) +
				static_cast<int>(components[1]) +
				static_cast<int>(components[2]) +
				static_cast<int>(components[3]);

			mPoint++;
			mBoundaryMin++;
			mBoundaryMax++;
			mMedian++;
		}
		return hashValue;
	}

	/* Use Pyramid-Technique to hash n-dimensional point to a single dimension,
	 * which is an integer value. */
	inline int hashPoint(unsigned int numDimensions, const Point& point,
		const Region& boundary, const std::vector<int>& medianPoint)
	{
		int searchKey = 0;
		for (int d = 0; d < numDimensions; d++)
		{
			searchKey += static_cast<int>(
				(point[d] - boundary[d].min) / (boundary[d].max - boundary[d].min) * medianPoint[d]
			);
		}

		/*Point minPoint(numDimensions);
		Point maxPoint(numDimensions);
		for (unsigned int i = 0; (i < boundary.numDimensions()); i++)
		{
			minPoint[i] = boundary[i].min;
			maxPoint[i] = boundary[i].max;
		}
		Point median(numDimensions);
		for (unsigned int i = 0; (i < medianPoint.size()); i++)
			median[i] = medianPoint[i];

		std::cout << "Non-SSE: " << searchKey << std::endl;
		std::cout << "SSE: " << hashPointSSE(numDimensions, point, minPoint, maxPoint, median) << std::endl;*/

		return searchKey;
	}


}

#endif