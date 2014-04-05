#ifndef MDSEARCH_SEQUENTIALSCAN_H
#define MDSEARCH_SEQUENTIALSCAN_H

#include "IndexStructure.h"

namespace mdsearch
{

	class SequentialScan : public IndexStructure
	{

	public:
		SequentialScan(int numDimensions);

		// Bulk load points into the structure
		void loadPoints(const PointList& points);
		// Return all store points as a raw list
		const PointList& allPoints() const;

		void clear();

		// Dynamic operations
		bool insert(const Point& p);
		bool remove(const Point& p);
		bool update(const Point& oldPoint, const Point& newPoint);
		// Queries
		bool pointExists(const Point& p);

	protected:
		PointList points;

	};

}

#endif