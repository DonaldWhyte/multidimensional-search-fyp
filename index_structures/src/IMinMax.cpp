#include "IMinMax.h"
#include "Hashing.h"

namespace mdsearch
{

	inline Real computeMinMaxValue(unsigned int numDimensions, const Point& p,
		const Point& minPoint, const Point& maxPoint, Real theta)
	{
		// Determine minimum and maximum dimensions
		int dMin = 0;
		int dMax = 0;
		for (unsigned int d = 0; (d < numDimensions); d++)
		{
			if (p[d] < p[dMin])
				dMin = d;
			else if (p[d] > p[dMax])
				dMax = d;
		}

		Real xMin = normaliseCoord(p[dMin], minPoint[dMin], maxPoint[dMin]);
		Real xMax = normaliseCoord(p[dMax], minPoint[dMax], maxPoint[dMax]);
		if (xMin + theta < 1 - xMax)
			return static_cast<Real>(dMin) + xMin;
		else
			return static_cast<Real>(dMax) + xMax;
	}

	IMinMax::IMinMax(unsigned int numDimensions, const Region& boundary, Real theta)
		: PyramidTree(numDimensions, boundary), theta(theta)
	{
	}

	bool IMinMax::insert(const Point& point)
	{
		// Retrieve containing bucket by hashing point into key
		int searchKey = computeMinMaxValue(numDimensions, point, minPoint, maxPoint, theta);
		// Search underlying 1D structure to find point's bucket
		PTBucket* bucket = NULL;
		OneDMap::iterator it = hashMap.find(searchKey);
		if (it != hashMap.end())
			bucket = &(it->second);

		if (bucket) // if bucket for point exists
		{
			// If point is stored in bucket - it ALREADY EXISTS
			if (getPointIndexInBucket(point, bucket) != -1)
			{
				return false;
			}
			else
			{
				bucket->points.push_back(point);
				bucket->pointSums.push_back(point.sum());
				return true;
			}
		}
		else // if bucket does not exist for point, create it!
		{
			PTBucket newBucket;
			newBucket.points.push_back(point);
			newBucket.pointSums.push_back(point.sum());
			hashMap[searchKey] = newBucket;
			return true;
		}
	}

	PTBucket* IMinMax::getContainingBucket(const Point& point)
	{
		// Hash point into one-dimensional key
		int searchKey = computeMinMaxValue(numDimensions, point, minPoint, maxPoint, theta);
		// Search underlying structure to find point's bucket
		OneDMap::iterator it = hashMap.find(searchKey);
		if (it != hashMap.end())
			return &(it->second); // pointer to bucket
		else
			return NULL;
	}

}