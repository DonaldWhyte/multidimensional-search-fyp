#ifndef MDSEARCH_IMINMAX_H
#define MDSEARCH_IMINMAX_H

#include "PyramidTree.h"

namespace mdsearch
{

	class IMinMax : public PyramidTree
	{

	public:
		IMinMax(unsigned int numDimensions, const Region& boundary, Real theta);

		virtual bool insert(const Point& point);

	private:
		virtual PTBucket* getContainingBucket(const Point& point);

		Real theta;

	};

}

#endif