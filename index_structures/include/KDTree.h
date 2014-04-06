#ifndef MDSEARCH_KDTREE_H
#define MDSEARCH_KDTREE_H

#include "IndexStructure.h"

namespace mdsearch
{

	struct KDNode
	{
		// TODO
	};

	class KDTree : public IndexStructure
	{


	public:
		KDTree(unsigned int numDimensions);
		virtual ~KDTree();

		virtual void clear();
		virtual bool insert(const Point& p);
		virtual bool remove(const Point& p);
		virtual bool update(const Point& oldPoint, const Point& newPoint);
		virtual bool pointExists(const Point& p);
		virtual PointList pointsInRegion(const Region& region);

	private:
		KDNode* root;

	};

}

#endif