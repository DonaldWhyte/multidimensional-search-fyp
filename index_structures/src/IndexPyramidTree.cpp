#include "IndexPyramidTree.h"
#include "Hashing.h"
#include "Util.h"

namespace mdsearch
{

	IndexPyramidTree::IndexPyramidTree(unsigned int nDimensions, const Region& treeBoundary,
		int maxEmptyElements, CleanupProcedure cleanupProcedure) :
		IndexStructure(nDimensions),
		maxEmptyElements(maxEmptyElements), cleanupProcedure(cleanupProcedure),
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

	void IndexPyramidTree::clear()
	{
		// NOTE: Using assigment not clear() to ensure memory is de-allocated
		// (through destructors of containers)
		points = PointList();
		pointSums = RealList();
		hashMap = OneDMap();
		emptyElementIndices = IndexList();
	}

	bool IndexPyramidTree::insert(const Point& point)
	{
		// TODO: add boundary check w/ point here???
		
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

	bool IndexPyramidTree::remove(const Point& point)
	{
		// Find bucket the point would belong to
		#ifdef MDSEARCH_USE_SSE_HASHING
			int searchKey = hashPointSSE(numDimensions, point, minPoint, maxPoint, medianPoint);
		#else
			int searchKey = hashPoint(numDimensions, point, minPoint, maxPoint, medianPoint);
		#endif
		OneDMap::iterator keyValue = hashMap.find(searchKey);
		// Bucket has been found, point MIGHT be stored in structure
		if (keyValue != hashMap.end())
		{
			// Search through points in bucket to see if it contains the given point
			IndexList& indices = keyValue->second;
			IndexList::iterator pointIndex = indices.begin();
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
				// higher than a certain threshold, clean up unused points
				if (maxEmptyElements != -1 && // NOTE: -1 means the list should NOT be cleaned
					emptyElementIndices.size() >= maxEmptyElements)
				{
					if (cleanupProcedure == CLEANUP_PROC_REBUILD)
					{
						rebuild();
					}
					else
					{
						defragment();
					}
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

	bool IndexPyramidTree::update(const Point& oldPoint, const Point& newPoint)
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

	bool IndexPyramidTree::pointExists(const Point& point)
	{
		return (getPointIndex(point) >= 0);
	}

	PointList IndexPyramidTree::pointsInRegion(const Region& region)
	{
		// TODO
		return PointList();
	}

	const PointList& IndexPyramidTree::allPoints() const
	{
		return points;
	}

	const IndexList& IndexPyramidTree::emptyIndices() const
	{
		return emptyElementIndices;
	}

	const Region& IndexPyramidTree::getBoundary() const
	{
		return boundary;
	}

	void IndexPyramidTree::insertToStructure(const Point& point, bool searchKeyExists)
	{
		// Add raw point adn the sum of all its elements to the vectors
		points.push_back(point);
		pointSums.push_back(point.sum());

		#ifdef MDSEARCH_USE_SSE_HASHING
			int searchKey = hashPointSSE(numDimensions, point, minPoint, maxPoint, medianPoint);
		#else
			int searchKey = hashPoint(numDimensions, point, minPoint, maxPoint, medianPoint);
		#endif
		int currentIndex = points.size() - 1;
		if (searchKeyExists)
		{
			// If there is no inserted data, but there is a search key,
			// be sure to insert the current index (latest inserted point
			// index) into that point's bucket (this one)
			IndexList& indices = hashMap.find(searchKey)->second; // storing REFERFENCE so changes are made
			indices.push_back(currentIndex);
		}
		else
		{
			// If there is no inserted data and there is no search key
			IndexList indices;
			indices.reserve(10);
			indices.push_back(currentIndex);
			hashMap[searchKey] = indices;
		}
	}

	int IndexPyramidTree::getPointIndex(const Point& point)
	{
		// First check if this point's bucket exists
		#ifdef MDSEARCH_USE_SSE_HASHING
			int searchKey = hashPointSSE(numDimensions, point, minPoint, maxPoint, medianPoint);
		#else
			int searchKey = hashPoint(numDimensions, point, minPoint, maxPoint, medianPoint);
		#endif
		OneDMap::const_iterator keyValue = hashMap.find(searchKey);
		if (keyValue != hashMap.end())
		{
			// Compute the sum of the point's elements
			Real pSum = point.sum();
			// Look through each of the bucket's points
			const IndexList& indices = keyValue->second;
			for (IndexList::const_iterator index = indices.begin();
				(index != indices.end()); index++)
			{
				// First check that the sum of the points match!
				// We can avoid performing many point equality checks by doing this
				Real sum = pointSums[*index];
				if (compare(sum, pSum) == 0)
				{
					const Point& foundPoint = points[*index];
					if (point == foundPoint)
			  		{
			  			return *index;
			  		}
				}
		  	}
			// Return -2, as the point does not exist but its bucket does
			return -2;
		}
		else
		{
			// If both the point AND ITS BUCKET do not exist
			return -1;
		}
	}	

	/* Function used to sort integer values in DESCENDING ORDER. */
	bool descendingSorter(unsigned int i, unsigned int j)
	{
		return (i > j);	
	}

	void IndexPyramidTree::defragment()
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

	void IndexPyramidTree::updatePointIndices(unsigned int removedIndex)
	{
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

	void IndexPyramidTree::rebuild()
	{
		// Store old points to re-insert into structure
		PointList oldPoints = points;
		// Clear all data from structure
		points.clear();
		pointSums.clear();
		hashMap.clear();
		// Insert non-marked pooints into structure incrementally
		for (unsigned int i = 0; (i < oldPoints.size()); i++)
		{
			// Only insert point if its old index has NOT been marked as empty
			if (std::find(emptyElementIndices.begin(), emptyElementIndices.end(), i) == emptyElementIndices.end())
				insert(oldPoints[i]);
		}
		// Now clear the marked elements list
		emptyElementIndices.clear();
	}

}