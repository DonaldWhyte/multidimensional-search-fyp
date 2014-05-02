#ifndef MDSEARCH_DUPLICATEHASHTABLE_H
#define MDSEARCH_DUPLICATEHASHTABLE_H

#include "IndexStructure.h"
#include <boost/unordered_map.hpp>

namespace mdsearch
{

	class DuplicateHashTable : public IndexStructure
	{

	public:
		DuplicateHashTable(unsigned int numDimensions);

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

		std::string toString() const;

		/* Return total number of points currently stored in the structure. */
		unsigned int numPointsStored() const;
		/* Return total number of buckets in structure. */
		unsigned int numBuckets() const;
		/* Return average number of points stored in a bucket. */
		Real averagePointsPerBucket() const;
		/* Return satdnard deviation of points in buckets. */
		Real stdevPointsPerBucket() const;
		/* Return mimimum and maximum number of points stored in a single bucket. */
		unsigned int minPointsPerBucket() const;
		unsigned int maxPointsPerBucket() const;

	private:
		int getPointIndexInBucket(const Point& point, const PointList& bucket) const;

		typedef boost::unordered_map<std::size_t, PointList> PointMap;
		PointMap hashMap;		

	};

}

#endif