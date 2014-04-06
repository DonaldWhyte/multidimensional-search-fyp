#include "BoundaryDistanceHashing.h"
#include "Hashing.h"

namespace mdsearch
{

	BoundaryDistanceHashing::BoundaryDistanceHashing(unsigned int numDimensions, const Region& boundary)
		: PyramidTree(numDimensions, boundary)
	{
	}

	bool BoundaryDistanceHashing::insert(const Point& point)
	{
		// Retrieve containing bucket by hashing point into key
		#ifdef MDSEARCH_USE_SSE_HASHING
			int searchKey = boundaryDistanceHashSSE(numDimensions, point, minPoint, maxPoint, medianPoint);
		#else
			int searchKey = boundaryDistanceHash(numDimensions, point, minPoint, maxPoint, medianPoint);
		#endif
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

	PTBucket* BoundaryDistanceHashing::getContainingBucket(const Point& point)
	{
		// Hash point into one-dimensional key
		#ifdef MDSEARCH_USE_SSE_HASHING
			int searchKey = boundaryDistanceHashSSE(numDimensions, point, minPoint, maxPoint, medianPoint);
		#else
			int searchKey = boundaryDistanceHash(numDimensions, point, minPoint, maxPoint, medianPoint);
		#endif
		// Search underlying structure to find point's bucket
		OneDMap::iterator it = hashMap.find(searchKey);
		if (it != hashMap.end())
			return &(it->second); // pointer to bucket
		else
			return NULL;
	}

}