#include "SequentialScan.h"
#include "Utility.h"

namespace mdsearch
{

	void SequentialScan::loadPoints(const PointList& pointsToAdd)
	{
		// Pre-allocate necessary memory to add points
		points.reserve(points.size() + pointsToAdd.size());
		// Add points in given list to THE END of this structure's point list
		points.insert(points.end(), pointsToAdd.begin(), pointsToAdd.end());
		// Ensure there are no duplicates in the resulting array
		points.erase(util::Uniquify(points.begin(), points.end()), points.end());
	}

	const PointList& SequentialScan::allPoints() const
	{
		return points;
	}
	
	void SequentialScan::insert(const Point& p)
	{
		// TODO	
	}

	bool SequentialScan::remove(const Point& p)
	{
		// TODO	
	}

	bool SequentialScan::update(const Point& oldPoint, const Point& newPoint)
	{
		// TODO	
	}
	
	bool SequentialScan::pointExists(const Point& p)
	{
		// TODO	
	}

	PointList SequentialScan::pointsInRegion(const Region& r)
	{
		// TODO	
	}

}