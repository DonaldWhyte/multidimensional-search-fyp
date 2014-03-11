#ifndef MDSEARCH_SKEWEDDATASETGENERATOR_H
#define MDSEARCH_SKEWEDDATASETGENERATOR_H

#include "Point.h"

namespace mdsearch
{

	enum SkewDirection
	{
		SKEW_DIRECTION_LEFT = 0,
		SKEW_DIRECTION_RIGHT
	};

	class SkewedDatasetGenerator
	{

	public:
		PointList generate(unsigned int numDimensions, const Point& min,
			const Point& max, Real skewSeverity, SkewDirection skewDirection) const;

	};

}

#endif