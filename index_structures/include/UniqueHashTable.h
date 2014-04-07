#ifndef MDSEARCH_UNIQUEHASHTABLE_H
#define MDSEARCH_UNIQUEHASHTABLE_H

#include "IndexStructure.h"
#include <boost/unordered_map.hpp>

namespace mdsearch
{

	class UniqueHashTable : public IndexStructure
	{

	public:
		UniqueHashTable(unsigned int numDimensions);

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

	private:
		typedef boost::unordered_map<std::size_t, Point> PointMap;
		PointMap hashMap;		

	};

}

#endif