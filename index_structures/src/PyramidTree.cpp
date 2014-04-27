#include "PyramidTree.h"
#include "Hashing.h"
#include "Util.h"
#include <algorithm>
#include <sstream>
#include <fstream>

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

		for (unsigned int d = 0; (d < numDimensions); d++)
		{
			medianPoint[d] = treeBoundary[d].min + ((treeBoundary[d].max - treeBoundary[d].min) / 2.0f);
		}

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

	void PyramidTree::clear()
	{
		// NOTE: Using assignment not clear() to ensure memory is de-allocated
		// (through destructors of containers)
		hashMap = OneDMap();
	}

	bool PyramidTree::insert(const Point& point)
	{		
		// Retrieve containing bucket by hashing point into key
		Real searchKey = computePyramidValue(numDimensions, point, minPoint, maxPoint, bucketInterval);
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

	Real PyramidTree::stdevPointsPerBucket() const
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

	unsigned int PyramidTree::minPointsPerBucket() const
	{
		size_t minCount = 0;
		for (OneDMap::const_iterator it = hashMap.begin(); (it != hashMap.end()); it++)
			minCount = std::min(minCount, it->second.points.size());
		return minCount;
	}
	unsigned int PyramidTree::maxPointsPerBucket() const
	{
		size_t maxCount = 0;
		for (OneDMap::const_iterator it = hashMap.begin(); (it != hashMap.end()); it++)
			maxCount = std::max(maxCount, it->second.points.size());
		return maxCount;
	}

	std::string PyramidTree::toString() const
	{
		std::stringstream ss;
		ss << "PyramidTree\n";
		ss << "\tLoad Factor of Underlying Hash Table: " << hashMap.load_factor() << "\n";
		ss << "\tTotal Points Stored: " << numPointsStored() << "\n";
		ss << "\tAverage Points Per Bucket: " << averagePointsPerBucket() << "\n";
		ss << "\tStandard deviation of points per bucket: " << stdevPointsPerBucket() << "\n";
		ss << "\tMin points per bucket: " << minPointsPerBucket() << "\n";
		ss << "\tMax points per bucket: " << maxPointsPerBucket() << "\n";
		// Find bucket with largest points and print out the first 30 points
		int maxCount = 0;
		const PointList* maxBucket = NULL;
		for (OneDMap::const_iterator it = hashMap.begin(); (it != hashMap.end()); it++)
		{
			if (maxCount < it->second.points.size())
			{
				maxBucket = &(it->second.points);
				maxCount = maxBucket->size();
			}
		}
		unsigned int pointsToPrint = std::min(100, maxCount);
		for (unsigned int i = 0; (i < pointsToPrint); i++)
		{
			std::cout << computePyramidValue(numDimensions, (*maxBucket)[i], minPoint, maxPoint, bucketInterval)
				<< (*maxBucket)[i] << std::endl;
		}

		return ss.str();
	}

	void PyramidTree::toHistogramFile(const std::string& filename) const
	{
		std::ofstream file(filename.c_str());
		file << "1D Value Distribution\n";
		// Write each 2D value to the file, in the same number o
		for (OneDMap::const_iterator it = hashMap.begin(); (it != hashMap.end()); it++)
		{
			for (unsigned int i = 0; (i < it->second.points.size()); i++)
			{
				file << it->first << "\n";
			}
		}
	}

	PTBucket* PyramidTree::getContainingBucket(const Point& point)
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
