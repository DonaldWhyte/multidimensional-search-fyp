#include "SequentialScan.h"
#include <algorithm>

namespace mdsearch
{

	SequentialScan::SequentialScan(int numDimensions) : IndexStructure(numDimensions)
	{
	}

	void SequentialScan::loadPoints(const PointList& pointsToAdd)
	{
		// Pre-allocate necessary memory to add points
		points.reserve(points.size() + pointsToAdd.size());
		// Now insert each point incrementally
		// (done so duplicate checking code can be performed)
		for (PointList::const_iterator it = pointsToAdd.begin();
			(it != pointsToAdd.end()); it++)
		{
			insert(*it);
		}
	}

	const PointList& SequentialScan::allPoints() const
	{
		return points;
	}

	void SequentialScan::clear()
	{
		// NOTE: Not using clear so all heap memory is also de-allocated
		points = PointList();
	}
	
	bool SequentialScan::insert(const Point& p)
	{
		// Only insert point if it does not already exist in the structure
		if (!pointExists(p))
		{
			points.push_back(p);
			return true;
		}
		else
		{
			return false;
		}
	}

	bool SequentialScan::remove(const Point& p)
	{
		size_t oldSize = points.size();
		points.erase( std::remove(points.begin(), points.end(), p), points.end() );
		// If the size of the vector has changed, then a point was removed!
		// Therefore, true is returned
		return (oldSize != points.size());
	}

	bool SequentialScan::update(const Point& oldPoint, const Point& newPoint)
	{
		// Find point with given value
		PointList::iterator currPoint = points.begin();
		for (currPoint; (currPoint != points.end()); currPoint++)
		{
			if (*currPoint == oldPoint) // we've found the desired point!
			{
				// Corner case: check points are not the same (do nothing if so)
				if (oldPoint == newPoint)
					return true;
				// Check if the new point already exists. If so, just erase the old point
				if (pointExists(newPoint))
					points.erase(currPoint);
				// Chnage the point if the new one does NOT EXIST
				else
					*currPoint = newPoint;
				// Point was found, so we just return true
				return true;
			}
		}
		// Never found point to update!
		return false;
	}
	
	bool SequentialScan::pointExists(const Point& p)
	{
		for (PointList::const_iterator it = points.begin();
			(it != points.end()); it++)
		{
			if (p == *it)
				return true;
		}
		return false;
	}

	PointList SequentialScan::pointsInRegion(const Region& region)
	{
		PointList foundPoints;
		for (PointList::const_iterator it = points.begin();
			(it != points.end()); it++)
		{
			if (region.contains(*it))
				foundPoints.push_back(*it);
		}
		return foundPoints;
	}

}