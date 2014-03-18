#ifndef MDSEARCH_BDTREE_H
#define MDSEARCH_BDTREE_H

#include "IndexStructure.h"

namespace mdsearch
{

	class BDTree : public IndexStructure
	{

	public:
		BDTree(unsigned int numDimensions, const Region& boundary);

		virtual void clear();

		virtual bool insert(const Point& p);
		virtual bool remove(const Point& p);
		virtual bool update(const Point& oldPoint, const Point& newPoint);

		virtual bool pointExists(const Point& p);
		virtual PointList pointsInRegion(const Region& region);

	private:
		struct BDTreeNode
		{
			Region boundary;
			Region otherBoundary;
			bool differenceInBoundaries;

			BDTreeNode* left;
			BDTreeNode* right;
			BDTreeNode* outer;

			Point point;
		};

		/* Perform a Splay quadtree promote(x) operation on the given node.
		 * Return true if the node was successfully promoted and false
		 * if it wasn't (due to it already being the root). */
		bool promote(BDTreeNode* node);

		BDTreeNode* root;

	};

}

#endif