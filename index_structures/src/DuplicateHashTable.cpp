#include "DuplicateHashTable.h"
#include "Hashing.h"
#include "Util.h"

namespace mdsearch
{

	DuplicateHashTable::DuplicateHashTable(unsigned int numDimensions) : IndexStructure(numDimensions)
	{
		clear();
	}

	void DuplicateHashTable::clear()
	{
		hashMap = PointMap();
	}

	bool DuplicateHashTable::insert(const Point& point)
	{
		Real searchKey = hashPoint(point);
		PointMap::iterator it = hashMap.find(searchKey);
		if (it != hashMap.end())
		{
			// If point is stored in bucket - it ALREADY EXISTS
			if (getPointIndexInBucket(point, it->second) != -1)
			{
				return false;
			}
			else
			{
				it->second.push_back(point);
				return true;
			}
		}
		else // if bucket does not exist for point, create it!
		{
			PointList bucket;
			bucket.push_back(point);
			hashMap.insert(std::make_pair(searchKey, bucket));
			return true;
		}
	}

	bool DuplicateHashTable::remove(const Point& point)
	{
		Real searchKey = hashPoint(point);
		PointMap::iterator it = hashMap.find(searchKey);
		// PTBucket has been found, point MIGHT be stored in structure
		if (it != hashMap.end())
		{
			int index = getPointIndexInBucket(point, it->second);
			// If the point was found in bucket
			if (index != -1)
			{
				// Swapping last element with the element to remove, so when the
				// desired point is removed it won't cause a potentially O(n) move
				// operation.
				removeElementAtIndex(it->second, index);
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

	bool DuplicateHashTable::update(const Point& oldPoint, const Point& newPoint)
	{
		if (remove(oldPoint))
		{
			insert(newPoint);
			return true;
		}
		else
		{
			return false;
		}
	}

	bool DuplicateHashTable::pointExists(const Point& point)
	{
		Real searchKey = hashPoint(point);
		PointMap::const_iterator it = hashMap.find(searchKey);
		if (it != hashMap.end())
			return (getPointIndexInBucket(point, it->second) != -1);
		else
			return false;
	}

	int DuplicateHashTable::getPointIndexInBucket(const Point& point, const PointList& bucket) const
	{
		// Search through points in bucket to see if it contains the given point
		for (int index = 0; (index < bucket.size()); index++)
			if (point == bucket[index])
				return index;
		return -1;
	}

	std::string DuplicateHashTable::toString() const
	{
		std::stringstream ss;
		ss << "DuplicateHashTable\n";
		ss << "\tLoad Factor of Underlying Hash Table: " << hashMap.load_factor() << "\n";
		ss << "\tTotal Points Stored: " << numPointsStored() << "\n";
		ss << "\tAverage Points Per Bucket: " << averagePointsPerBucket() << "\n";
		ss << "\tStandard deviation of points per bucket: " << stdevPointsPerBucket() << "\n";
		ss << "\tMin points per bucket: " << minPointsPerBucket() << "\n";
		ss << "\tMax points per bucket: " << maxPointsPerBucket() << "\n";
		ss << "\tBuckets:\n";
		for (PointMap::const_iterator it = hashMap.begin(); (it != hashMap.end()); it++)
		{
			//ss << "\t\tKey: " << it->first << ", Num Elements: " << it->second.size() << "\n";
		}
		return ss.str();
	}

	unsigned int DuplicateHashTable::numPointsStored() const
	{
		int total = 0;
		for (PointMap::const_iterator it = hashMap.begin(); (it != hashMap.end()); it++)
			total += it->second.size();
		return total;
	}

	Real DuplicateHashTable::averagePointsPerBucket() const
	{
		return static_cast<Real>(numPointsStored()) / hashMap.size();
	}

	Real DuplicateHashTable::stdevPointsPerBucket() const
	{
		Real mean = averagePointsPerBucket();
		Real sum = 0;
		for (PointMap::const_iterator it = hashMap.begin(); (it != hashMap.end()); it++)
		{
			Real difference = static_cast<Real>(it->second.size()) - mean;
			sum += (difference * difference);
		}
		Real inverseBucketCount = 1.0f / hashMap.size();
		return sqrt(inverseBucketCount * sum);
	}

	unsigned int DuplicateHashTable::minPointsPerBucket() const
	{
		size_t minCount = 0;
		for (PointMap::const_iterator it = hashMap.begin(); (it != hashMap.end()); it++)
			minCount = std::min(minCount, it->second.size());
		return minCount;
	}

	unsigned int DuplicateHashTable::maxPointsPerBucket() const
	{
		size_t maxCount = 0;
		for (PointMap::const_iterator it = hashMap.begin(); (it != hashMap.end()); it++)
			maxCount = std::max(maxCount, it->second.size());
		return maxCount;
	}	

}