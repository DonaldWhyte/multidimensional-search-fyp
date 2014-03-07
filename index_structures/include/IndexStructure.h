#ifndef MDSEARCH_INDEXSTRUCTURE_H
#define MDSEARCH_INDEXSTRUCTURE_H

#include "Point.h"
#include "Region.h"

namespace mdsearch
{

	// NOTE: This is for index structures where each UNIQUE point can
	// only be in the structure ONCE. There are NO DUPLICATES!
	class IndexStructure
	{

	public:
		IndexStructure(unsigned int numDimensions) : numDimensions(numDimensions)
		{
		}

		// Dynamic operations
		virtual void insert(const Point& p) = 0;
		virtual bool remove(const Point& p) = 0;
		virtual bool update(const Point& oldPoint, const Point& newPoint) = 0;
		// Queries
		virtual bool pointExists(const Point& p) = 0;
		virtual PointList pointsInRegion(const Region& r) = 0;

		inline unsigned int dimensionality() const
		{
			return numDimensions;
		}

	private:
		unsigned int numDimensions;

	};

}

#endif