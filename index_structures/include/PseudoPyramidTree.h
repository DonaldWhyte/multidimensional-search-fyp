#ifndef MDSEARCH_PSEUDOPYRAMIDTREE_H
#define MDSEARCH_PSEUDOPYRAMIDTREE_H

#include "PyramidTree.h"

namespace mdsearch
{

	class PseudoPyramidTree : public PyramidTree
	{

	public:
		PseudoPyramidTree(unsigned int numDimensions, const Region& boundary);

		virtual bool insert(const Point& point);

	private:
		virtual PTBucket* getContainingBucket(const Point& point);

	};

}

#endif