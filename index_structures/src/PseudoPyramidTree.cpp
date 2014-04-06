#include "PseudoPyramidTree.h"
#include "Hashing.h"

namespace mdsearch
{

	PseudoPyramidTree::PseudoPyramidTree(unsigned int numDimensions, const Region& boundary)
		: PyramidTree(numDimensions, boundary)
	{
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

	bool PseudoPyramidTree::insert(const Point& point)
	{
		// Retrieve containing bucket by hashing point into key
		int searchKey = computePseudoPyramidValue(numDimensions, point,
			minPoint, maxPoint, scaleFactors, cumulativeSFProducts);
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

	PTBucket* PseudoPyramidTree::getContainingBucket(const Point& point)
	{
		// Hash point into one-dimensional key
		int searchKey = computePseudoPyramidValue(numDimensions, point,
			minPoint, maxPoint, scaleFactors, cumulativeSFProducts);
		// Search underlying structure to find point's bucket
		OneDMap::iterator it = hashMap.find(searchKey);
		if (it != hashMap.end())
			return &(it->second); // pointer to bucket
		else
			return NULL;
	}

}