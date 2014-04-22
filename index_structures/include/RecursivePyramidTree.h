#ifndef MDSEARCH_RECURSIVEPYRAMIDTREE_H
#define MDSEARCH_RECURSIVEPYRAMIDTREE_H

#include <vector>
#include <boost/unordered_map.hpp>
#include "IndexStructure.h"

namespace mdsearch
{

	class RecursivePyramidTree; // forward declaration

	/* Structure used for Pyramid tree buckets. */
	struct RPTBucket
	{
		RecursivePyramidTree* subTree;
		unsigned int removedDimension;

		PointList points;
		RealList pointSums;

		RPTBucket() : subTree(NULL), removedDimension(0)
		{
		}
	};

	class RecursivePyramidTree : public IndexStructure
	{

	public:
		RecursivePyramidTree(unsigned int numDimensions, const Region& boundary);

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

		/* Return spatial region the tree covers. */
		const Region& getBoundary() const;

		/* Return total number of points currently stored in the structure. */
		unsigned int numPointsStored() const;
		/* Return average number of points stored in a bucket. */
		Real averagePointsPerBucket() const;
		/* Return satdnard deviation of points in buckets. */
		Real stdevPointsPerBucket() const;
		/* Return mimimum and maximum number of points stored in a single bucket. */
		unsigned int minPointsPerBucket() const;
		unsigned int maxPointsPerBucket() const;
		/* Return string reprensetation of Pyrmaid tree. */
		std::string toString() const;

	protected:
		static const unsigned int MAX_BUCKET_NUMBER = 300000; // TODO: need?

		virtual RPTBucket* getContainingBucket(const Point& point);
		virtual int getPointIndexInBucket(const Point& point, const RPTBucket* bucket) const;

		// Unordered_map for storing the points
		// Key = hashed 1D representation of point,
		// Vsalue = list of points in BUCKET
		// Each key can contain MULTIPLE indices
		typedef boost::unordered_map<Real, RPTBucket> OneDMap;
		OneDMap hashMap;

		// Entire region of space the Pyramid tree covers
		// (points outside this region are ignored)
		Region boundary;
		Point minPoint;
		Point maxPoint;
		// Interval between buckets
		unsigned int bucketInterval;
		// Maximum size of a single bucket
		unsigned int maxBucketSize;
		// Median values of the data set.
		Point medianPoint;

	};

}

#endif