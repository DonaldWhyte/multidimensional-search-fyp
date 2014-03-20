#include "SplayQuadtree.h"

namespace mdsearch
{

	SplayQuadtree::SplayQuadtree(unsigned int numDimensions, const Region& initialBoundary)
		: IndexStructure(numDimensions), boundary(initialBoundary), root(NULL)
	{
		clear();
	}

	SplayQuadtree::~SplayQuadtree()
	{
		delete root;
	}

	void SplayQuadtree::clear()
	{
		delete root;
		root = new Node(EMPTY_NODE, boundary);
	}

	bool SplayQuadtree::insert(const Point& p)
	{
		return false;
	}

	bool SplayQuadtree::remove(const Point& p)
	{
		return false;
	}

	bool SplayQuadtree::update(const Point& oldPoint, const Point& newPoint)
	{
		return false;
	}

	bool SplayQuadtree::pointExists(const Point& p)
	{
		return false;
	}

	PointList SplayQuadtree::pointsInRegion(const Region& region)
	{
		return PointList();
	}

	SplayQuadtree::Node* SplayQuadtree::rootNode()
	{
		return root;
	}

}