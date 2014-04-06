#include "KDTree.h"

namespace mdsearch
{

		KDTree::KDTree(unsigned int numDimensions) : IndexStructure(numDimensions)
		{
			// TODO
		}

		KDTree::~KDTree()
		{
			delete root;
		}

		void KDTree::clear()
		{
			delete root;
			root = NULL;
		}

		bool KDTree::insert(const Point& p)
		{
			// TODO
			return false;
		}

		bool KDTree::remove(const Point& p)
		{
			// TODO
			return false;
		}

		bool KDTree::update(const Point& oldPoint, const Point& newPoint)
		{
			// TODO
			return false;
		}

		bool KDTree::pointExists(const Point& p)
		{
			// TODO
			return false;
		}

		PointList KDTree::pointsInRegion(const Region& region)
		{
			// TODO
			return PointList();
		}

}