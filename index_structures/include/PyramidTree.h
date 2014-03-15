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
		PyramidTree(unsigned int numDimensions, const Region& boundary);

		virtual void clear();

		virtual bool insert(const Point& point);
		virtual bool remove(const Point& point);
		virtual bool update(const Point& oldPoint, const Point& newPoint);
		virtual bool pointExists(const Point& point);
		virtual PointList pointsInRegion(const Region& region);

		const PointList& allPoints() const;

	private:
		static const unsigned int MAX_BUCKET_NUMBER = 300000;

		void insertToStructure(const Point& point, bool searchKeyExists);
		int getPointIndex(const Point& point);
		int HashValue(const Point& myPoint);

		// Contains all the full d-dimensional points stored in the Pyramid tree
		PointList points;
		// Contains the SUM of all elements for each stored point
		// That is, pointSums[i] stores the sum of all elements of points[i]
		RealList pointSums;
		// Unordered_map for storing the key values of 1-dimensional interpolated point.
		// Key = hashed Point, value = list of indices of potential points
		// Each key can contain MULTIPLE indicies
		typedef boost::unordered_map<Real, std::vector<int>, ihash> OneDMap;
		OneDMap Map;    

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