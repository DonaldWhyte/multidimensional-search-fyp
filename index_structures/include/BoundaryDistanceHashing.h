#ifndef MDSEARCH_BOUNDARYDISTANCEHASHING_H
#define MDSEARCH_BOUNDARYDISTANCEHASHING_H

#include "PyramidTree.h"

namespace mdsearch
{

	class BoundaryDistanceHashing : public PyramidTree
	{

	public:
		BoundaryDistanceHashing(unsigned int numDimensions, const Region& boundary);

		virtual bool insert(const Point& point);

	private:
		virtual PTBucket* getContainingBucket(const Point& point);

	};

}

#endif