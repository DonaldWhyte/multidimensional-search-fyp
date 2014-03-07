#ifndef MDSEARCH_UNIFORMDATASETGENERATOR_H
#define MDSEARCH_UNIFORMDATASETGENERATOR_H

#include "Point.h"

namespace mdsearch
{

	class UniformDatasetGenerator
	{

	public:
		PointList generate(unsigned int numDimensions,
			Real min, Real max, unsigned int pointsPerDimension) const;

	private:
		std::vector<Real> generateUniformSequence(Real min, Real max, unsigned int n) const;

	};

}

#endif