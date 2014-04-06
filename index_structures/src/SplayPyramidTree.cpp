#include "SplayPyramidTree.h"
#include "Hashing.h"
#include "Util.h"

namespace mdsearch
{

	SplayPyramidTree::SplayPyramidTree(unsigned int nDimensions,
		const Region& treeBoundary) : IndexStructure(nDimensions),
		boundary(treeBoundary), minPoint(nDimensions), maxPoint(nDimensions),
		medianPoint(nDimensions), bucketInterval(1)
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
		// Construct min-max points for SSE hashing function
		for (unsigned int i = 0; (i < boundary.numDimensions()); i++)
		{
			minPoint[i] = boundary[i].min;
			maxPoint[i] = boundary[i].max;
		}			
	}

	void SplayPyramidTree::clear()
	{
		splayTree.clear();
	}

	bool SplayPyramidTree::insert(const Point& point)
	{
		// Retrieve containing bucket by hashing point into key
		int searchKey = computePyramidValue(numDimensions, point, minPoint, maxPoint, bucketInterval);
		// Search underlying 1D structure to find point's bucket
		PTBucket* bucket = splayTree.getValue(searchKey);

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
			splayTree.insert(searchKey, newBucket);
			return true;
		}
	}

	bool SplayPyramidTree::remove(const Point& point)
	{
		PTBucket* bucket = getContainingBucket(point);
		// PTBucket has been found, point MIGHT be stored in structure
		if (bucket)
		{
			int index = getPointIndexInBucket(point, bucket);
			// If the point was found in bucket
			if (index != -1)
			{
				removeElementAtIndex(bucket->points, index);
				removeElementAtIndex(bucket->pointSums, index);
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
		PTBucket* bucket = getContainingBucket(point);
		return (bucket && (getPointIndexInBucket(point, bucket) != -1));
	}

	const Region& SplayPyramidTree::getBoundary() const
	{
		return boundary;
	}

	PTBucket* SplayPyramidTree::getContainingBucket(const Point& point)
	{
		// Hash point into one-dimensional key
		int searchKey = computePyramidValue(numDimensions, point, minPoint, maxPoint, bucketInterval);
		// Search underlying splay tree to find point's bucket
		return splayTree.getValue(searchKey);
	}

	int SplayPyramidTree::getPointIndexInBucket(const Point& point, const PTBucket* bucket) const
	{
		// Search through points in bucket to see if it contains the given point
		Real pSum = point.sum();
		for (int index = 0; (index < bucket->points.size()); index++)
			if (pSum == bucket->pointSums[index] && point == bucket->points[index])
				return index;
		return -1;
	}

}