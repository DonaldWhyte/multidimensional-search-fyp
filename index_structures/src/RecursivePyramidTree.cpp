#include "RecursivePyramidTree.h"
#include "PyramidTree.h"
#include "Hashing.h"
#include "Util.h"
#include <algorithm>
#include <sstream>
#include <cmath>

namespace mdsearch
{

	/* Determine what dimension a Pyramid value is for. */
	inline int pyramidDimension(Real pyramidValue, unsigned int numDimensions)
	{
		return (static_cast<int>(pyramidValue) % numDimensions);
	}

	/* Return copy of given region with specified dimension removed. */
	inline Region removeDimension(const Region& boundary, unsigned int dimension)
	{
		Region reducedBoundary(boundary.numDimensions() - 1);
		int index = 0;
		for (unsigned int d = 0; (d < dimension); d++)
		{
			reducedBoundary[index] = boundary[d];
			++index;
		}
		for (unsigned int d = dimension + 1; (d < boundary.numDimensions()); d++)
		{
			reducedBoundary[index] = boundary[d];
			++index;
		}
		return reducedBoundary;
	}

	/* Return copy of given point with specified dimension removed. */
	inline Point removeDimension(const Point& p, unsigned int dimension)
	{
		Point reducedPoint(p.numDimensions() - 1);
		int index = 0;
		for (unsigned int d = 0; (d < dimension); d++)
		{
			reducedPoint[index] = p[d];
			++index;
		}
		for (unsigned int d = dimension + 1; (d < p.numDimensions()); d++)
		{
			reducedPoint[index] = p[d];
			++index;
		}
		return reducedPoint;
	}

	/* Construct d - 1 Pyramid tree for d-dimensional boundary using
	 * given Pyramid value. */
	// TODO: pass removed dimension into this to save computation???
	inline RecursivePyramidTree* constructRecursivePyramidTree(Real pyramidValue, const Region& boundary)
	{
		// Determine dimension pyramid is for and the HEIGHT of the value in the pyramid
		Real pyramid;
		Real height = std::modf(pyramidValue, &pyramid);
		int dimension = static_cast<int>(pyramid) % boundary.numDimensions();
		// Create Pyramid tree with d - 1 boundary containing all
		// dimensions not for this Pyramid
		return new RecursivePyramidTree(boundary.numDimensions() - 1,
			removeDimension(boundary, dimension)
		);
	}

	RecursivePyramidTree::RecursivePyramidTree(unsigned int nDimensions, const Region& treeBoundary) :
		IndexStructure(nDimensions), boundary(treeBoundary),
		minPoint(nDimensions), maxPoint(nDimensions), medianPoint(nDimensions),
		bucketInterval(1),
		maxBucketSize(nDimensions) // d is maximum number of points allowed in single bucket! (d O(d) comparisons means O(d^2)))
	{
		// Compute the interval between buckets 
		bucketInterval = static_cast<Real>(
			MAX_BUCKET_NUMBER / (numDimensions * 2)
		);
		bucketInterval = floor(bucketInterval);
		medianPoint = computeInitialMedianPoint(MAX_BUCKET_NUMBER, numDimensions);

		// Ensure boundaries are NEVER ZERO SIZED and the maximum
		// values are always larger than the minimum
		for (unsigned int d = 0; (d < numDimensions); d++)
			if (boundary[d].max <= boundary[d].min)
				boundary[d].max = boundary[d].min + 1;
		// Construct min-max points for SSE hashing function
		for (unsigned int i = 0; (i < boundary.numDimensions()); i++)
		{
			minPoint[i] = boundary[i].min;
			maxPoint[i] = boundary[i].max;
		}
	}

	void RecursivePyramidTree::clear()
	{
		// NOTE: Using assigment not clear() to ensure memory is de-allocated
		// (through destructors of containers)
		hashMap = OneDMap();
	}

	bool RecursivePyramidTree::insert(const Point& point)
	{	
		// Retrieve containing bucket by hashing point into key
		Real searchKey = computePyramidValue(numDimensions, point, minPoint, maxPoint, bucketInterval);
		// Search underlying 1D structure to find point's bucket
		RPTBucket* bucket = NULL;
		OneDMap::iterator it = hashMap.find(searchKey);
		if (it != hashMap.end())
			bucket = &(it->second);

		if (bucket) // if bucket for point exists
		{
			// If bucket is a recursive Pyramid tree
			if (bucket->subTree)
			{
				return bucket->subTree->insert( removeDimension(point, bucket->removedDimension) );
			}
			// If bucket is array, try and get its index in the bucket.
			// If it exists, the we cannot insert the point!
			else if (getPointIndexInBucket(point, bucket) != -1)
			{
				return false;
			}
			else
			{
				if (bucket->points.size() >= maxBucketSize) // if adding point will exceed max bucket size 
				{
					// Construct recursive Pyramid tree to contine bucket's points
					bucket->removedDimension = pyramidDimension(searchKey, numDimensions);
					bucket->subTree = constructRecursivePyramidTree(searchKey, boundary);
					// Insert all points currently within bucket into the sub-tree
					for (PointList::const_iterator p = bucket->points.begin(); (p != bucket->points.end()); ++p)
						bucket->subTree->insert( removeDimension(*p, bucket->removedDimension) );
					// Now insert new point into the sub-tree
					bucket->subTree->insert( removeDimension(point, bucket->removedDimension) );
					// Clear points stored in this bucket since they've been
					// inserted into the sub tree
					bucket->points.clear();
					bucket->pointSums.clear();					
				}
				else // if not, just add point to bucket
				{
					bucket->points.push_back(point);
					bucket->pointSums.push_back(point.sum());
				}
				return true;
			}
		}
		else // if bucket does not exist for point, create it!
		{
			RPTBucket newBucket;
			newBucket.points.push_back(point);
			newBucket.pointSums.push_back(point.sum());
			hashMap[searchKey] = newBucket;
			return true;
		}
	}

	bool RecursivePyramidTree::remove(const Point& point)
	{
		return false;
		/*
		RPTBucket* bucket = getContainingBucket(point);
		// RPTBucket has been found, point MIGHT be stored in structure
		if (bucket)
		{
			int index = getPointIndexInBucket(point, bucket);
			// If the point was found in bucket
			if (index != -1)
			{
				// Swapping last element with the element to remove, so when the
				// desired point is removed it won't cause a potentially O(n) move
				// operation.
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
		}*/
	}

	bool RecursivePyramidTree::update(const Point& oldPoint, const Point& newPoint)
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

	bool RecursivePyramidTree::pointExists(const Point& point)
	{
		RPTBucket* bucket = getContainingBucket(point);
		if (bucket)
		{
			if (bucket->subTree)
			{
				return bucket->subTree->pointExists( removeDimension(point, bucket->removedDimension) );
			}
			else 
			{
				return (getPointIndexInBucket(point, bucket) != -1);
			}
		}
		else
		{
			return false;
		}
	}

	const Region& RecursivePyramidTree::getBoundary() const
	{
		return boundary;
	}

	unsigned int RecursivePyramidTree::numPointsStored() const
	{
		int total = 0;
		for (OneDMap::const_iterator it = hashMap.begin(); (it != hashMap.end()); it++)
			total += it->second.points.size();
		return total;
	}

	Real RecursivePyramidTree::averagePointsPerBucket() const
	{
		return static_cast<Real>(numPointsStored()) / hashMap.size();
	}

	Real RecursivePyramidTree::stdevPointsPerBucket() const
	{
		Real mean = averagePointsPerBucket();
		Real sum = 0;
		for (OneDMap::const_iterator it = hashMap.begin(); (it != hashMap.end()); it++)
		{
			Real difference = static_cast<Real>(it->second.points.size()) - mean;
			sum += (difference * difference);
		}
		Real inverseBucketCount = 1.0f / hashMap.size();
		return sqrt(inverseBucketCount * sum);
	}

	unsigned int RecursivePyramidTree::minPointsPerBucket() const
	{
		size_t minCount = 0;
		for (OneDMap::const_iterator it = hashMap.begin(); (it != hashMap.end()); it++)
			minCount = std::min(minCount, it->second.points.size());
		return minCount;
	}
	unsigned int RecursivePyramidTree::maxPointsPerBucket() const
	{
		size_t maxCount = 0;
		for (OneDMap::const_iterator it = hashMap.begin(); (it != hashMap.end()); it++)
			maxCount = std::max(maxCount, it->second.points.size());
		return maxCount;
	}

	std::string RecursivePyramidTree::toString() const
	{
		std::stringstream ss;
		// TODO
		return ss.str();
	}

	RPTBucket* RecursivePyramidTree::getContainingBucket(const Point& point)
	{
		// Hash point into one-dimensional key
		Real searchKey = computePyramidValue(numDimensions, point, minPoint, maxPoint, bucketInterval);
		// Search underlying structure to find point's bucket
		OneDMap::iterator it = hashMap.find(searchKey);
		if (it != hashMap.end())
			return &(it->second); // pointer to bucket
		else
			return NULL;
	}

	int RecursivePyramidTree::getPointIndexInBucket(const Point& point,
		const RPTBucket* bucket) const
	{
		// Search through points in bucket to see if it contains the given point
		Real pSum = point.sum();
		for (int index = 0; (index < bucket->points.size()); index++)
			if (pSum == bucket->pointSums[index] && point == bucket->points[index])
				return index;
		return -1;
	}

}
