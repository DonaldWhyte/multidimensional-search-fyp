#include "IndexPseudoPyramidTree.h"
#include "Hashing.h"
#include "Util.h"

namespace mdsearch
{

	IndexPseudoPyramidTree::IndexPseudoPyramidTree(unsigned int nDimensions, const Region& treeBoundary,
		int maxEmptyElements, CleanupProcedure cleanupProcedure) :
		IndexStructure(nDimensions),
		maxEmptyElements(maxEmptyElements), cleanupProcedure(cleanupProcedure),
		boundary(treeBoundary), minPoint(nDimensions), maxPoint(nDimensions)
	{
		// Compute the interval between buckets 
		bucketInterval = static_cast<Real>(
			MAX_BUCKET_NUMBER / (numDimensions * 2)
		);
		bucketInterval = floor(bucketInterval);
		
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

		// Compute scale factors
		Real m = static_cast<Real>(1.0 / numDimensions);
		int div = ceil(pow(MAX_BUCKET_NUMBER, m));
		scaleFactors = std::vector<int>(numDimensions, div);
		// Pre-compute the cumlative products of the scale factors to
		// speed up hashing function
		cumulativeSFProducts.resize(numDimensions);
		cumulativeSFProducts[0] = 1;
		for (unsigned int d = 1; (d < numDimensions); d++)
		{
			cumulativeSFProducts[d] = scaleFactors[d - 1] * cumulativeSFProducts[d - 1];
		}
	}

	void IndexPseudoPyramidTree::clear()
	{
		// NOTE: Using assigment not clear() to ensure memory is de-allocated
		// (through destructors of containers)
		points = PointList();
		pointSums = RealList();
		hashMap = OneDMap();
		emptyElementIndices = IndexList();
	}

	bool IndexPseudoPyramidTree::insert(const Point& point)
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

	bool IndexPseudoPyramidTree::remove(const Point& point)
	{
		// Find bucket the point would belong to
		int searchKey = computePseudoPyramidValue(numDimensions, point,
			minPoint, maxPoint, scaleFactors, cumulativeSFProducts);
		OneDMap::iterator keyValue = hashMap.find(searchKey);
		// Bucket has been found, point MIGHT be stored in structure
		if (keyValue != hashMap.end())
		{
			// Search through points in bucket to see if it contains the given point
			IndexList& indices = keyValue->second;
			IndexList::iterator pointIt = indices.begin();
			for (pointIt; (pointIt != indices.end()); pointIt++)
			{
				if (point == points[*pointIt])
				{
					break;
				}
			}
			// If the point was found
			if (pointIt != indices.end())
			{
				// Add the index to the lsit 
				emptyElementIndices.push_back(*pointIt);				
				// Remove index pointing to the point in the bucket
				removeElementAtIterator(indices, pointIt);
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

	bool IndexPseudoPyramidTree::update(const Point& oldPoint, const Point& newPoint)
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

	bool IndexPseudoPyramidTree::pointExists(const Point& point)
	{
		return (getPointIndex(point) >= 0);
	}

	const PointList& IndexPseudoPyramidTree::allPoints() const
	{
		return points;
	}

	const IndexList& IndexPseudoPyramidTree::emptyIndices() const
	{
		return emptyElementIndices;
	}

	const Region& IndexPseudoPyramidTree::getBoundary() const
	{
		return boundary;
	}

	void IndexPseudoPyramidTree::insertToStructure(const Point& point, bool searchKeyExists)
	{
		// Add raw point adn the sum of all its elements to the vectors
		points.push_back(point);
		pointSums.push_back(point.sum());

		int searchKey = computePseudoPyramidValue(numDimensions, point,
			minPoint, maxPoint, scaleFactors, cumulativeSFProducts);
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

	int IndexPseudoPyramidTree::getPointIndex(const Point& point)
	{
		// First check if this point's bucket exists
		int searchKey = computePseudoPyramidValue(numDimensions, point,
			minPoint, maxPoint, scaleFactors, cumulativeSFProducts);
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

	void IndexPseudoPyramidTree::defragment()
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
			points.erase(points.begin() + *index); // can't use erase-remove idiom because order matters!
			pointSums.erase(pointSums.begin() + *index);
			updatePointIndices(*index);
		}

		emptyElementIndices.clear();
	}

	void IndexPseudoPyramidTree::updatePointIndices(unsigned int removedIndex)
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

	void IndexPseudoPyramidTree::rebuild()
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