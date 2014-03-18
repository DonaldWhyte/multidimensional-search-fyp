#ifndef MDSEARCH_RANDOMPOINTGENERATOR_H
#define MDSEARCH_RANDOMPOINTGENERATOR_H

#include "Point.h"
#include "Region.h"

namespace mdsearch
{

	class RandomPointGenerator
	{

	public:
		PointList generatePointsInRegion(unsigned int numDimensions,
			const Region& region, unsigned int numPoints) const;
		
	};

}

#endif