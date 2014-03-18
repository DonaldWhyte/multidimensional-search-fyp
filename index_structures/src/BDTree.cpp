#include "BDTree.h"

namespace mdsearch
{

	BDTree::BDTree(unsigned int numDimensions, const Region& boundary) :
		IndexStructure(numDimensions), boundary(boundary)
	{
		
	}

	void BDTree::clear()
	{
		// TODO
	}

	bool BDTree::insert(const Point& p)
	{
		// TODO
		return false;
	}

	bool BDTree::remove(const Point& p)
	{
		// TODO
		return false;
	}
	bool BDTree::update(const Point& oldPoint, const Point& newPoint)
	{
		// TODO
		return false;
	}

	bool BDTree::pointExists(const Point& p)
	{
		// TODO
		return false;
	}

	PointList BDTree::pointsInRegion(const Region& region)
	{
		// TODO
		return PointList();
	}

}