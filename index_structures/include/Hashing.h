#ifndef MDSEARCH_HASHING_H
#define MDSEARCH_HASHING_H

#include <xmmintrin.h>
#include <vector>
#include "Point.h"
#include "Region.h"

namespace mdsearch
{

	/* Use Pyramid-Technique to hash n-dimensional point to a single dimension,
	 * which is an integer value. */
	inline int hashPoint(unsigned int numDimensions, const Point& point, const Region& boundary,
		const std::vector<int>& medianPoint, const std::vector<int>& cumulativeMedianProducts)
	{
		int searchKey = 0;
		for (int d = 0; d < numDimensions; d++)
		{
			int value = std::min(
				static_cast<int>((point[d] - boundary[d].min) / (boundary[d].max - boundary[d].min) * medianPoint[d]),
				medianPoint[d] - 1
			);
			//std::cout << value << std::endl;
			searchKey += value * cumulativeMedianProducts[d];
		}
		//std::cout << searchKey << std::endl;
		//exit(1);
		return searchKey;
	}

	#define printSSEValue(val) \
		_mm_storeu_ps(components, val); \
		std::cout << components[0] << " " << components[1] << " " << components[2] << " " << components[3] << std::endl;

	inline int hashPointSSE(unsigned int numDimensions, const Real* point,
		const Real* minPoint, const Real* maxPoint,
		const Real* medianPoint, const Real* cumulativeMedianProducts)
	{
		// NOTE: Assuming dimensions are divisible by four
		int numLoops = numDimensions / 4;

		float components[4];

		const __m128* mPoint = reinterpret_cast<const __m128*>(point);
		const __m128* mBoundaryMin = reinterpret_cast<const __m128*>(minPoint);
		const __m128* mBoundaryMax = reinterpret_cast<const __m128*>(maxPoint);
		const __m128* mMedian = reinterpret_cast<const __m128*>(&medianPoint[0]);
		const __m128* mCumMedProducts = reinterpret_cast<const __m128*>(&cumulativeMedianProducts[0]);

		// Store results of temporary computation
		__m128 m1, m2, m3, m4;

		__m128 searchKey = _mm_set_ps1(0);
		//printSSEValue(*mCumMedProducts);
		for (int i = 0; i < numLoops; i++)
		{
			// Compute distance of point from median in current dimensions
			m1 = _mm_sub_ps(*mPoint, *mBoundaryMin);
			m2 = _mm_sub_ps(*mBoundaryMax, *mBoundaryMin);
			m3 = _mm_div_ps(m1, m2);
			m1 = _mm_mul_ps(m3, *mMedian);
			//printSSEValue(m1);
			// Ensure value does not exceed the median point
			// TODO: bitwise min()
			// Apply cumulative median products
			m2 = _mm_mul_ps(m1, *mCumMedProducts);
			searchKey = _mm_add_ps(searchKey, m2);

			mPoint++;
			mBoundaryMin++;
			mBoundaryMax++;
			mMedian++;
			mCumMedProducts++;
		}

		// Sum up individual search key components to get actual key
		_mm_storeu_ps(components, searchKey);
		//std::cout << static_cast<int>(components[0] + components[1] + components[2] + components[3]) << std::endl;
		//exit(1);
		return static_cast<int>( components[0] + components[1] + components[2] + components[3]);
	}

}

#endif