#include "PyramidTree.h"

namespace mdsearch
{

	PyramidTree::PyramidTree(unsigned int nDimensions, const Region& treeBoundary,
		unsigned int maxEmptyElements) :
		IndexStructure(nDimensions),
		maxEmptyElements(maxEmptyElements), boundary(treeBoundary), 
		bucketInterval(1), medianPoint(nDimensions)
	{
		// Compute the interval between buckets 
		bucketInterval = static_cast<double>(
			MAX_BUCKET_NUMBER / (numDimensions * 2)
		);
		bucketInterval = floor(bucketInterval);
		// Compute initial median value
		double m = static_cast<double>(1.0 / numDimensions);
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

	void PyramidTree::clear()
	{
		// NOTE: Using assigment not clear() to ensure memory is de-allocated
		// (through destructors of containers)
		points = PointList();
		pointSums = RealList();
		hashMap = OneDMap();
		emptyElementIndices = IndexList();
	}

	bool PyramidTree::insert(const Point& point)
	{
		int pointIndex = getPointIndex(point);
		if (pointIndex >= 0) // if point already exists in structure
		{
			return false;
		}
		else // if point does not exist, insert it!
		{
			bool searchKeyExists = (pointIndex == -2);
			insertToStructure(point, searchKeyExists);
			return true;
		}
	}

	bool PyramidTree::remove(const Point& point)
	{
		// Find bucket the point would belong to
		int searchKey = hashPoint(point);
		OneDMap::iterator keyValue = hashMap.find(searchKey);
		// Bucket has been found, point MIGHT be stored in structure
		if (keyValue != hashMap.end())
		{
			// Search through points in bucket to see if it contains the given point
			std::vector<int>& indices = keyValue->second;
			std::vector<int>::iterator pointIndex = indices.begin();
			for (pointIndex; (pointIndex != indices.end()); pointIndex++)
			{
				if (point == points[*pointIndex])
				{
					break;
				}
			}
			// If the point was found
			if (pointIndex != indices.end())
			{
				// Add the index to the lsit 
				emptyElementIndices.push_back(*pointIndex);				
				// Remove index pointing to the point in the bucket
				indices.erase(pointIndex);
				// If the amount of empty elements of the point array is
				// higher than a certain threshold, DEFRAGMENT the structure
				if (emptyElementIndices.size() >= maxEmptyElements)
				{
					defragment();
				}

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
		return (getPointIndex(point) >= 0);
	}

	PointList PyramidTree::pointsInRegion(const Region& region)
	{
		// TODO
		return PointList();
	}

	const PointList& PyramidTree::allPoints() const
	{
		return points;
	}

	const IndexList& PyramidTree::emptyIndices() const
	{
		return emptyElementIndices;
	}

	void PyramidTree::insertToStructure(const Point& point, bool searchKeyExists)
	{
		// Transform the original data to the encoded data sets.
		double sum = 0;

		for (int j = 0; j < numDimensions; j++) 
		{
			sum += point[j];
		}
		// Add raw point adn the sum of all its elements to the vectors
		points.push_back(point);
		pointSums.push_back(sum);

		int searchKey = hashPoint(point);
		int currentIndex = points.size() - 1;
		if (searchKeyExists)
		{
			// If there is no inserted data, but there is a search key
			std::vector<int> value = hashMap.find(searchKey)->second;
			value.push_back(currentIndex);
			hashMap[searchKey] = value;
		}
		else
		{
			// If there is no inserted data and there is no search key
			std::vector<int> myVector;
			myVector.reserve(10);
			myVector.push_back(currentIndex);
			hashMap[searchKey] = myVector;
		}
	}

	int PyramidTree::getPointIndex(const Point& point)
	{
		int finalIndex;
		double pSum = 0;

		for(int d = 0; d < numDimensions; d++) 
		{
			pSum += point[d];
		}

		int searchKey = hashPoint(point);
		OneDMap::const_iterator keyValue = hashMap.find(searchKey);
		if (keyValue != hashMap.end())
		{
			const std::vector<int>& output = keyValue->second;
			bool contain = false;

			for (int i = 0; i < output.size(); i++)
			{
				int index = output[i];
				double sum = pointSums[index];
				if (compare(sum, pSum) == 0)
				{
					Point foundPoint = points[index];
					if (point == foundPoint)
			  		{
			  			return index;
			  		}
				}
		  	}
			// if there is no inserted data, but there is search key
			if (!contain)
			{
				return -2;
			}
		}
		else
		{
			// if there is no inserted data
			return -1;
		}
	}	

	int PyramidTree::hashPoint(const Point& point)
	{
		int searchKey = 0 ;
		int value[numDimensions];
		for (int d = 0; d < numDimensions; d++ )
		{
			value[d] = static_cast<int>(
				static_cast<double>((point[d] - boundary[d].min) / (boundary[d].max - boundary[d].min))
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

	/* Fucntion used to sort integer values in DESCENDING ORDER. */
	bool descendingSorter(unsigned int i, unsigned int j)
	{
		return (i > j);	
	}

	void PyramidTree::defragment()
	{
		// Sort indices list in DESCENDING ORDER
		// Done so indices aren't changed by previously removed elements
		std::sort(emptyElementIndices.begin(), emptyElementIndices.end(), descendingSorter);
		// Remove all empty elements
		PointList::iterator pBegin = points.begin();
		RealList::iterator sBegin = pointSums.begin();
		for (IndexList::const_iterator index = emptyElementIndices.begin();
			(index != emptyElementIndices.end()); index++)
		{
			points.erase(pBegin + (*index));
			pointSums.erase(sBegin + (*index));
			updatePointIndices((*index));
		}
		
		emptyElementIndices.clear();
	}

	void PyramidTree::updatePointIndices(unsigned int removedIndex)
	{
		// TODO: comment
		for (OneDMap::iterator entry = hashMap.begin(); (entry != hashMap.end()); entry++)
		{
			for (IndexList::iterator index = entry->second.begin();
				(index != entry->second.end()); index++)
			{
				if (*index > removedIndex)
				{
					*index = *index - 1;
				}
			}
		}
	}

}