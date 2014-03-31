#ifndef MDSEARCH_SPLAYPYRAMIDTREE_H
#define MDSEARCH_SPLAYPYRAMIDTREE_H

#include <vector>
#include "IndexStructure.h"
#include "SplayTree.h"
#include "PyramidTree.h"

namespace mdsearch
{

	class SplayPyramidTree : public IndexStructure
	{

	public:
		SplayPyramidTree(unsigned int numDimensions, const Region& boundary);

		/* Clear all points currently stored in the structure. */
		virtual void clear();

		/* Insert point into the tree.
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

		/* Return spatial region this Pyramid covers. */
		const Region& getBoundary() const;

	private:
		static const unsigned int MAX_BUCKET_NUMBER = 300000;

		void insertToStructure(const Point& point, PTBucket* bucket = NULL);
		PTBucket* getContainingBucket(const Point& point);
		int getPointIndexInBucket(const Point& point, const PTBucket* bucket) const;

		typedef SplayTree<int, PTBucket> SplayTreeType;
		SplayTreeType splayTree;

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