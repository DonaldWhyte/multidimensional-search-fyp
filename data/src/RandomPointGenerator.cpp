#include "RandomPointGenerator.h"
#include "Util.h"

namespace mdsearch
{

	PointList RandomPointGenerator::generatePointsInRegion(
		unsigned int numDimensions, const Region& region,
		unsigned int numPoints) const
	{
		RealList temp(numDimensions);

		PointList points;
		points.reserve(numPoints);
		for (unsigned int i = 0; (i < numPoints); i++)
		{
			for (unsigned int d = 0; (d < numDimensions); d++)
			{
				temp[d] = generateRandomNumber(region[d].min, region[d].max);
			}
			points.push_back(Point(numDimensions, &temp[0]));
		}

		return points;
	}

}