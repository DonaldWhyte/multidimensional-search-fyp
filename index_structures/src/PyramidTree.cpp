#include "PyramidTree.h"
#include "Hashing.h"
#include "Util.h"
#include <algorithm>

namespace mdsearch
{

	PyramidTree::PyramidTree(unsigned int nDimensions, const Region& treeBoundary) :
		IndexStructure(nDimensions), boundary(treeBoundary),
		bucketInterval(1)
	{
		// Compute the interval between buckets 
		bucketInterval = static_cast<Real>(
			MAX_BUCKET_NUMBER / (numDimensions * 2)
		);
		bucketInterval = floor(bucketInterval);
		medianPoint = computeInitialMedianPoint(MAX_BUCKET_NUMBER, numDimensions);

		// Ensure boundaries are NEVER ZERO SIZED and the maximum
		// values are always larger than the minimum
		for (unsigned int d = 0; d < numDimensions; d++)
		{
			if (boundary[d].max <= boundary[d].min)
			{
				boundary[d].max = boundary[d].min + 1;
			}
		}
		cumulativeMedianProducts = computeCumMedianProducts(medianPoint);
	}

	void PyramidTree::clear()
	{
		// NOTE: Using assigment not clear() to ensure memory is de-allocated
		// (through destructors of containers)
		hashMap = OneDMap();
	}

	bool PyramidTree::insert(const Point& point)
	{
		// TODO: add boundary check w/ point here???
		
		PTBucket* bucket = getContainingBucket(point);
		if (bucket) // if bucket for point exists
		{
			// If point is stored in bucket - it ALREADY EXISTS
			if (getPointIndexInBucket(point, bucket) != -1)
			{
				return false;
			}
			else
			{
				insertToStructure(point, bucket);
				return true;
			}
		}
		else // if bucker does not exist, pass NULL to indicate new bucket
		{
			insertToStructure(point, NULL);
			return true;
		}
	}

	bool PyramidTree::remove(const Point& point)
	{
		PTBucket* bucket = getContainingBucket(point);
		// PTBucket has been found, point MIGHT be stored in structure
		if (bucket)
		{
			int index = getPointIndexInBucket(point, bucket);
			// If the point was found in bucket
			if (index != -1)
			{
				bucket->points.erase(bucket->points.begin() + index);
				bucket->pointSums.erase(bucket->pointSums.begin() + index);
				return true;
			}
			// Point is not contained in bucket -- cannot remove
			else
			{
				return false;
			}
		}
		// No bucket found, so point is not being stored in the structure
		else
		{
			return false;
		}
	}

	bool PyramidTree::update(const Point& oldPoint, const Point& newPoint)
	{
		if (remove(oldPoint)) // if point was removed successfully (i.e. it existed)
		{
			insert(newPoint); // insert a new point in its place!
			return true;
		}
		else
		{
			return false;
		}
	}

	bool PyramidTree::pointExists(const Point& point)
	{
		PTBucket* bucket = getContainingBucket(point);
		return (bucket && (getPointIndexInBucket(point, bucket) != -1));
	}

	PointList PyramidTree::pointsInRegion(const Region& region)
	{
		// TODO
		return PointList();
	}

	const Region& PyramidTree::getBoundary() const
	{
		return boundary;
	}

	void PyramidTree::insertToStructure(const Point& point, PTBucket* bucket)
	{
		if (bucket) // if bucket for point already exisrs
		{
			bucket->points.push_back(point);
			bucket->pointSums.push_back(point.sum());
		}
		else // if bucket does not exist for point, create it!
		{
			int searchKey = hashPoint(numDimensions, point, boundary, medianPoint, cumulativeMedianProducts);
			PTBucket newBucket;
			newBucket.points.push_back(point);
			newBucket.pointSums.push_back(point.sum());
			hashMap[searchKey] = newBucket;
		}
	}

	PTBucket* PyramidTree::getContainingBucket(const Point& point)
	{
		// Hash point into one-dimensional key
		int searchKey = hashPoint(numDimensions, point, boundary, medianPoint, cumulativeMedianProducts);
		// Search underlying splay tree to find point's bucket
		OneDMap::iterator it = hashMap.find(searchKey);
		if (it != hashMap.end())
			return &(it->second); // pointer to bucket
		else
			return NULL;
	}

	int PyramidTree::getPointIndexInBucket(const Point& point,
		const PTBucket* bucket) const
	{
		// Search through points in bucket to see if it contains the given point
		Real pSum = point.sum();
		for (int index = 0; (index < bucket->points.size()); index++)
			if (pSum == bucket->pointSums[index] && point == bucket->points[index])
				return index;
		return -1;
	}

}
