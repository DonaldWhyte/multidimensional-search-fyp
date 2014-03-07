#ifndef MDSEARCH_INDEXSTRUCTURE_H
#define MDSEARCH_INDEXSTRUCTURE_H

#include "Point.h"
#include "Region.h"

namespace mdsearch
{

	class IndexStructure
	{

	public:
		IndexStructure(unsigned int numDimensions) : numDimensions(numDimensions)
		{
		}

		// Dynamic operations
		virtual void insert(const Point& p) const = 0;
		virtual bool remove(const Point& p) const = 0;
		virtual bool update(const Point& p) const = 0;
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