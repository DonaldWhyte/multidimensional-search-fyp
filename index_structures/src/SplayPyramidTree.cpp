#include "SplayPyramidTree.h"

namespace mdsearch
{


	SplayPyramidTree::SplayPyramidTree(unsigned int nDimensions,
		const Region& treeBoundary) : IndexStructure(nDimensions),
		boundary(treeBoundary), bucketInterval(1), medianPoint(nDimensions)
	{
		// Compute the interval between buckets 
		bucketInterval = static_cast<Real>(
			MAX_BUCKET_NUMBER / (numDimensions * 2)
		);
		bucketInterval = floor(bucketInterval);
		// Compute initial median value
		Real m = static_cast<Real>(1.0 / numDimensions);
		int div = ceil(pow(MAX_BUCKET_NUMBER, m));
		for (unsigned int d = 0; d < numDimensions; d++)
		{
			medianPoint[d] = div;
		}
		// Ensure boundaries are NEVER ZERO SIZED and the maximum
		// values are always larger than the minimum
		for (unsigned int d = 0; d < numDimensions; d++)
		{
			if (boundary[d].max <= boundary[d].min)
			{
				boundary[d].max = boundary[d].min + 1;
			}
		}
	}

	void SplayPyramidTree::clear()
	{
		splayTree.clear();
	}

	bool SplayPyramidTree::insert(const Point& point)
	{
		// TODO: add boundary check w/ point here???
		
		Bucket* bucket = getContainingBucket(point);
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

	bool SplayPyramidTree::remove(const Point& point)
	{
		Bucket* bucket = getContainingBucket(point);
		// Bucket has been found, point MIGHT be stored in structure
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

	bool SplayPyramidTree::update(const Point& oldPoint, const Point& newPoint)
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

	bool SplayPyramidTree::pointExists(const Point& point)
	{
		Bucket* bucket = getContainingBucket(point);
		return (bucket && (getPointIndexInBucket(point, bucket) != -1));
	}

	PointList SplayPyramidTree::pointsInRegion(const Region& region)
	{
		// TODO
		return PointList();
	}

	const Region& SplayPyramidTree::getBoundary() const
	{
		return boundary;
	}

	void SplayPyramidTree::insertToStructure(const Point& point,
		SplayPyramidTree::Bucket* bucket)
	{
		if (bucket) // if bucket for point already exisrs
		{
			bucket->points.push_back(point);
			bucket->pointSums.push_back(point.sum());
		}
		else // if bucket does not exist for point, create it!
		{
			int searchKey = hashPoint(point);
			Bucket newBucket;
			newBucket.points.push_back(point);
			newBucket.pointSums.push_back(point.sum());
			splayTree.insert(searchKey, newBucket);
		}
	}

	SplayPyramidTree::Bucket* SplayPyramidTree::getContainingBucket(const Point& point)
	{
		// Hash point into one-dimensional key
		int searchKey = hashPoint(point);
		// Search underlying splay tree to find point's bucket
		return splayTree.getValue(searchKey);
	}

	int SplayPyramidTree::getPointIndexInBucket(const Point& point,
		const SplayPyramidTree::Bucket* bucket) const
	{
		// Search through points in bucket to see if it contains the given point
		Real pSum = point.sum();
		for (int index = 0; (index < bucket->points.size()); index++)
			if (pSum == bucket->pointSums[index] && point == bucket->points[index])
				return index;
		return -1;
	}

	int SplayPyramidTree::hashPoint(const Point& point)
	{
		int searchKey = 0 ;
		int value[numDimensions];
		for (int d = 0; d < numDimensions; d++ )
		{
			value[d] = static_cast<int>(
				static_cast<Real>((point[d] - boundary[d].min) / (boundary[d].max - boundary[d].min))
				* medianPoint[d]);
			if (value[d] >= medianPoint[d])
			{
				value[d] = medianPoint[d] - 1;
			}
		}
		
		for (int d = 0; d < numDimensions; d++)
		{
			int temp = value[d];
			for (int j = 0; j < d; j++)
			{	
				temp = temp * medianPoint[j];
			}
			searchKey = searchKey + temp;
		}
		return searchKey;
	}

}