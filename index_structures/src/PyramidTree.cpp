#include "PyramidTree.h"
#include "Hashing.h"
#include "Util.h"
#include <algorithm>
#include <sstream>

namespace mdsearch
{

	Point computeInitialMedianPoint(int maxBucketNumber, int numDimensions)
	{
		Real m = static_cast<Real>(1.0 / numDimensions);
		int div = ceil(pow(maxBucketNumber, m));
		return Point(numDimensions, div);
	}

	PyramidTree::PyramidTree(unsigned int nDimensions, const Region& treeBoundary) :
		IndexStructure(nDimensions), boundary(treeBoundary),
		minPoint(nDimensions), maxPoint(nDimensions), medianPoint(nDimensions),
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
		for (unsigned int d = 0; (d < numDimensions); d++)
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
				// Swapping last element with the element to remove, so when the
				// desired point is removed it won't cause a potentially O(n) move
				// operation.
				std::iter_swap(bucket->points.begin() + index, bucket->points.end() - 1);
				std::iter_swap(bucket->pointSums.begin() + index, bucket->pointSums.end() - 1);
				bucket->points.erase(bucket->points.end() - 1);
				bucket->pointSums.erase(bucket->pointSums.end() - 1);
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

	unsigned int PyramidTree::numPointsStored() const
	{
		int total = 0;
		for (OneDMap::const_iterator it = hashMap.begin(); (it != hashMap.end()); it++)
			total += it->second.points.size();
		return total;
	}

	Real PyramidTree::averagePointsPerBucket() const
	{
		return static_cast<Real>(numPointsStored()) / hashMap.size();
	}

	std::string PyramidTree::toString() const
	{
		std::stringstream ss;
		ss << "Pyramid Tree\n";
		ss << "\tLoad Factor of Hash Table: " << hashMap.load_factor() << "\n";
		ss << "\tTotal Points Stored: " << numPointsStored() << "\n";
		ss << "\tAverage Points Per Bucket: " << averagePointsPerBucket() << "\n";
		ss << "\tBuckets:\n";
		for (OneDMap::const_iterator it = hashMap.begin(); (it != hashMap.end()); it++)
		{
			ss << "\t\tKey: " << it->first << ", Num Elements: " << it->second.points.size() << "\n";
		}
		return ss.str();
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
			#ifdef MDSEARCH_USE_SSE_HASHING
				int searchKey = hashPointSSE(numDimensions, point, minPoint, maxPoint, medianPoint);
			#else
				int searchKey = hashPoint(numDimensions, point, minPoint, maxPoint, medianPoint);
			#endif
			PTBucket newBucket;
			newBucket.points.push_back(point);
			newBucket.pointSums.push_back(point.sum());
			hashMap[searchKey] = newBucket;
		}
	}

	PTBucket* PyramidTree::getContainingBucket(const Point& point)
	{
		// Hash point into one-dimensional key
		#ifdef MDSEARCH_USE_SSE_HASHING
			int searchKey = hashPointSSE(numDimensions, point, minPoint, maxPoint, medianPoint);
		#else
			int searchKey = hashPoint(numDimensions, point, minPoint, maxPoint, medianPoint);
		#endif
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
