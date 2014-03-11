#ifndef MDSEARCH_HYPERCUBERANDOMGENERATOR_H
#define MDSEARCH_HYPERCUBERANDOMGENERATOR_H

#include "Point.h"

namespace mdsearch
{

	class HyperCubeRandomGenerator
	{

	public:
		PointList generate(unsigned int numDimensions, const Point& min,
			const Point& max, unsigned int numPoints) const;

	};

}

#endif