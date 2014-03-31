#ifndef MDSEARCH_PYRAMIDTREE_H
#define MDSEARCH_PYRAMIDTREE_H

#include <vector>
#include <boost/unordered_map.hpp>
#include "IndexStructure.h"

namespace mdsearch
{

	std::vector<int> computeInitialMedianPoint(int maxBucketNumber, int numDimensions);

	/* Structure used for Pyramid tree buckets. */
	struct PTBucket
	{
		PointList points;
		RealList pointSums;
	};	

	class PyramidTree : public IndexStructure
	{

	public:
		PyramidTree(unsigned int numDimensions, const Region& boundary);

		/* Clear all points currently stored in the structure. */
		virtual void clear();

		/* Insert point into the Pyramid Tree.
		 * Returns true if the point was inserted successfully and
		 * false if the point is already stored in the structure. */
		virtual bool insert(const Point& point);
		/* Remove point from the tree.
		 * Returns true if the point was removed successfully and
		 * false if the point was not being stored. */
		virtual bool remove(const Point& point);
		/* Update the value of an existing point.
		 * Returns true if the point was updated and false if the
		 * old point is not stored in the structure. */
		virtual bool update(const Point& oldPoint, const Point& newPoint);
		/* Return true if the given point is being stored in the structure. */
		virtual bool pointExists(const Point& point);
		/* Return all points stored in this structure which are contained
		 * within the given spatial region. */
		virtual PointList pointsInRegion(const Region& region);

		/* Return spatial region the tree covers. */
		const Region& getBoundary() const;

		/* Return total number of points currently stored in the structure. */
		unsigned int numPointsStored() const;
		/* Return average number of points stored in a bucket. */
		Real averagePointsPerBucket() const;
		/* Return string reprensetation of Pyrmaid tree. */
		std::string toString() const;

	private:
		static const unsigned int MAX_BUCKET_NUMBER = 300000;

		void insertToStructure(const Point& point, PTBucket* bucket = NULL);
		PTBucket* getContainingBucket(const Point& point);
		int getPointIndexInBucket(const Point& point, const PTBucket* bucket) const;

		// Unordered_map for storing the points
		// Key = hashed 1D representation of point,
		// Vsalue = list of points in BUCKET
		// Each key can contain MULTIPLE indices
		typedef boost::unordered_map<int, PTBucket> OneDMap;
		OneDMap hashMap;

		// Entire region of space the Pyramid tree covers
		// (points outside this region are ignored)
		Region boundary;
		// Interval between buckets
		unsigned int bucketInterval;
		// Median values of the data set.
		std::vector<int> medianPoint;

	};

}

#endif