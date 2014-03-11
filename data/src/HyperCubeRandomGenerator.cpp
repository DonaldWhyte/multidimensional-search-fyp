#include "HyperCubeRandomGenerator.h"
#include "Util.h"

namespace mdsearch
{


	PointList HyperCubeRandomGenerator::generate(unsigned int numDimensions,
		const Point& min, const Point& max, unsigned int numPoints) const
	{
		RealList temp(numDimensions);

		PointList points;
		points.reserve(numPoints);
		for (unsigned int i = 0; (i < numPoints); i++)
		{
			for (unsigned int d = 0; (d < numDimensions); d++)
			{
				temp[d] = generateRandomNumber(min[d], max[d]);
			}
			points.push_back(Point(numDimensions, &temp[0]));
		}

		return points;
	}

}