#ifndef MDSEARCH_PYRAMIDTREE_H
#define MDSEARCH_PYRAMIDTREE_H

#include <vector>
#include <boost/unordered_map.hpp>
#include "IndexStructure.h"
#include "Util.h"

namespace mdsearch
{

	/* Hash function used for the underlying 1D hash map data structure. */
	struct ihash : std::unary_function<int, size_t>
	{
		size_t operator()(int const& x) const
		{
			return x;
		}
	};

	class PyramidTree : public IndexStructure
	{

	public:
		PyramidTree(unsigned int numDimensions, const Region& boundary,
			unsigned int maxEmptyElements = DEFAULT_MAX_EMPTY_ELEMENTS);

		virtual void clear();

		virtual bool insert(const Point& point);
		virtual bool remove(const Point& point);
		virtual bool update(const Point& oldPoint, const Point& newPoint);
		virtual bool pointExists(const Point& point);
		virtual PointList pointsInRegion(const Region& region);

		/* NOTE: May contain points recently deleted. Use emptyIndices()
		 * to find out which indices of the point list contain deleted pints. */
		const PointList& allPoints() const;
		const IndexList& emptyIndices() const;
		const Region& getBoundary() const;

	private:
		static const unsigned int MAX_BUCKET_NUMBER = 300000;
		static const unsigned int DEFAULT_MAX_EMPTY_ELEMENTS = 100;

		void insertToStructure(const Point& point, bool searchKeyExists);
		int getPointIndex(const Point& point);
		int hashPoint(const Point& myPoint);
		/* Defragments internal point arrays by deleting all unused points. */
		void defragment();
		/* Given the index of the last removed point in the 'points' array,
		 * update the remaining index entries in the hash table. */
		void updatePointIndices(unsigned int removedIndex);

		// Contains all the full d-dimensional points stored in the Pyramid tree
		PointList points;
		// Contains the SUM of all elements for each stored point
		// That is, pointSums[i] stores the sum of all elements of points[i]
		RealList pointSums;
		// Unordered_map for storing the key values of 1-dimensional interpolated point.
		// Key = hashed Point, value = list of indices of potential points
		// Each key can contain MULTIPLE indices
		typedef boost::unordered_map<Real, IndexList, ihash> OneDMap;
		OneDMap hashMap;   
		// Stores the indices which are empty in the linear points list
		// As a point is removed from the structure, the index is removed but
		// the point is not (as removal from the array is an O(n) operation).
		// This keeps track of which elements of the point array ARE NOT
		// being used
		IndexList emptyElementIndices;
		// Maximum amount of empty elements allowed before the list is defragmented
		unsigned int maxEmptyElements;

		// Entire region of space the Pyramid tree covers
		// (points outside this region are ignored)
		Region boundary;
		// Interval between buckets
		unsigned int bucketInterval;
		// Median values of the data set.
		Point medianPoint;

	};

}

#endif