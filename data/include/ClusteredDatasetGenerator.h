#ifndef MDSEARCH_CLUSTEREDDATASETGENERATOR_H
#define MDSEARCH_CLUSTEREDDATASETGENERATOR_H

#include "Point.h"
#include "Region.h"

namespace mdsearch
{

	class ClusterSpecification
	{

	public:
		ClusterSpecification(const Region& clusterRegion, unsigned int nPoints);

		const Region& region() const;
		unsigned int numPoints() const;

	private:
		Region clusterRegion;
		unsigned int nPoints;

	};

	class ClusteredDatasetGenerator
	{

	public:
		PointList generate(unsigned int numDimensions,
			const std::vector<ClusterSpecification>& clusters) const;

	};

}

#endif