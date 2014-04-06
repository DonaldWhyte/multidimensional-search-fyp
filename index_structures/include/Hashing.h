#ifndef MDSEARCH_HASHING_H
#define MDSEARCH_HASHING_H

#include <xmmintrin.h>
#include <vector>
#include "Point.h"
#include "Region.h"
#include <complex>

// Only define this is index structures should use SSE-enabled hashing
// (done to potentially increase speed of structures)
//#define MDSEARCH_USE_SSE_HASHING 1

namespace mdsearch
{

	/* Compute pyramid value of the given point, using the original Pyramid-technique. */
	inline int computePyramidValue(unsigned int numDimensions,
		const Point& p, const Point& minPoint, const Point& maxPoint,
		Real bucketInterval)
	{
		int index = 0;
		int dMax = 0;
		Real dMaxHeight = std::abs(
			0.5f - ((p[0] - minPoint[0]) / (maxPoint[0] - minPoint[0]))
		);
		for (int d = 1; (d < numDimensions); d++)
		{
			Real currentHeight = std::abs(
				0.5f - ((p[d] - minPoint[d]) / (maxPoint[d] - minPoint[d]))
			);
			if (dMaxHeight < currentHeight)
			{
				dMax = d;
				dMaxHeight = currentHeight;
			}
		}
		if (p[dMax] < 0.5f)
			index = dMax; // pyramid lower than central point
		else 
		{
			index = dMax + numDimensions; // pyramid higher than central point
		}
		return (index + std::abs(0.5f - p[index % numDimensions])) * bucketInterval;
	}

	/* Compute pseudo-pyramid value (hash) of n-dimensional point.
	 * Credit: Zheng Ghao. */
	inline int computePseudoPyramidValue(unsigned int numDimensions, const Point& point,
		const Point& minPoint, const Point& maxPoint, const std::vector<int>& scaleFactor,
		const std::vector<int>& cumulativeSFProducts)
	{
		int searchKey = 0;
		for (int d = 0; d < numDimensions; d++)
		{
			int value = std::min(
				static_cast<int>((point[d] - minPoint[d]) / (maxPoint[d] - minPoint[d]) * scaleFactor[d]),
				scaleFactor[d] - 1
			);
			searchKey += value * cumulativeSFProducts[d];
		}
		return searchKey;
	}

	/* Scale factor to allow hashing function to distinguish points better
	 * and separate them into different buckets.
	 * NOTE: Higher values typically decrease bucket utilisation. */
	static const Real BDH_SCALING_FACTOR = 300.0f;

	/* Use Boundary Distance Hashing  to reduce n-dimensional point to
	 * one-dimensional search key. */
	inline int boundaryDistanceHash(unsigned int numDimensions, const Point& point,
		const Point& minPoint, const Point& maxPoint, const Point& scaleFactor)
	{
		int hashValue = 0;
		for (int d = 0; d < numDimensions; d++)
		{
			hashValue += static_cast<int>(
				(point[d] - minPoint[d]) / (maxPoint[d] - minPoint[d]) * scaleFactor[d] * BDH_SCALING_FACTOR
			);
		}
		return hashValue;
	}
	inline int boundaryDistanceHashSSE(unsigned int numDimensions, const Point& point,
		const Point& minPoint, const Point& maxPoint, const Point& scaleFactor)
	{
		int numLoops = numDimensions / 4;
		int numLeftover = numDimensions % 4;

		const __m128* mPoint = reinterpret_cast<const __m128*>(point.toArray());
		const __m128* mBoundaryMin = reinterpret_cast<const __m128*>(minPoint.toArray());
		const __m128* mBoundaryMax = reinterpret_cast<const __m128*>(maxPoint.toArray());
		const __m128* mScaleFactor = reinterpret_cast<const __m128*>(scaleFactor.toArray());
		const __m128 mExtraScalingFactor = _mm_set_ps1(BDH_SCALING_FACTOR);

		__m128 m1, m2, m3;
		float components[4];

		int hashValue = 0;
		for (int i = 0; (i < numLoops); i++)
		{
			m1 = _mm_sub_ps(*mPoint, *mBoundaryMin);
			m2 = _mm_sub_ps(*mBoundaryMax, *mBoundaryMin);
			m3 = _mm_div_ps(m1, m2);
			m1 = _mm_mul_ps(m3, *mScaleFactor);
			m1 = _mm_mul_ps(m1, mExtraScalingFactor);
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
			mScaleFactor++;
		}
		// Handle leftover case sequentially
		for (int d = (numDimensions - numLeftover - 1); (d < numDimensions); d++)
		{
			hashValue += static_cast<int>(
				(point[d] - minPoint[d]) / (maxPoint[d] - minPoint[d]) * scaleFactor[d] * BDH_SCALING_FACTOR
			);
		}

		return hashValue;
	}

}

#endif