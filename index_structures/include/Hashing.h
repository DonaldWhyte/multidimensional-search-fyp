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
		return searchKey;
	}

	/*inline int hashPoint(unsigned int numDimensions, const Point& point, const Region& boundary,
		const std::vector<int>& medianPoint, const std::vector<int>& cumulativeMedianProducts)
	{
		int numDimensions = numDimensions / 4;
		int numLeftover = numDimensions % 4;

		__m128 m1, m2, m3, m4;

		mPoint = reinterpret_cast<__m128*>(&point.values[0]);
		mBoundaryMin = reinterpret_cast<__m128*>(TODO);
		mBoundaryMax = reinterpret_cast<__m128*>(TODO);

		int searchKey = 0;
		for (int d = 0; d < numDimensions; d++)
		{
			int value = std::min(
				static_cast<int>((point[d] - boundary[d].min) / (boundary[d].max - boundary[d].min) * medianPoint[d]),
				medianPoint[d] - 1
			);
			searchKey += value * cumulativeMedianProducts[d];
		}
		return searchKey;
	}*/


}

#endif