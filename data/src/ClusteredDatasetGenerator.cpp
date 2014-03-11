#include "ClusteredDatasetGenerator.h"
#include "RandomPointGenerator.h"

namespace mdsearch
{

	ClusterSpecification::ClusterSpecification(const Region& clusterRegion,
		unsigned int nPoints) : clusterRegion(clusterRegion), nPoints(nPoints)
	{
	}

	const Region& ClusterSpecification::region() const
	{
		return clusterRegion;
	}

	unsigned int ClusterSpecification::numPoints() const
	{
		return nPoints;
	}

	PointList ClusteredDatasetGenerator::generate(unsigned int numDimensions,
		const std::vector<ClusterSpecification>& clusters) const
	{
		PointList points;

		RandomPointGenerator rpGenerator;
		for (std::vector<ClusterSpecification>::const_iterator it = clusters.begin();
			(it != clusters.end()); it++)
		{
			PointList clusterPoints = rpGenerator.generatePointsInRegion(
				numDimensions, it->region(), it->numPoints());
			points.insert(points.end(), clusterPoints.begin(), clusterPoints.end());
		}

		return points;
	}

}