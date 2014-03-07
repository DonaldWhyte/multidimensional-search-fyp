#ifndef MDSEARCH_SEQUENTIALSCAN_H
#define MDSEARCH_SEQUENTIALSCAN_H

#include "IndexStructure.h"

namespace mdsearch
{

	class SequentialScan : public IndexStructure
	{

	public:
		SequentialScan(int numDimensions) : IndexStructure(numDimensions)
		{

		}

		// Bulk load points into the sturcture
		void loadPoints(const PointList& points);

		// Dynamic operations
		void insert(const Point& p);
		bool remove(const Point& p);
		bool update(const Point& p);
		// Queries
		bool pointExists(const Point& p);
		PointList pointsInRegion(const Region& r);

	private:
		PointList points;

	};

}

#endif