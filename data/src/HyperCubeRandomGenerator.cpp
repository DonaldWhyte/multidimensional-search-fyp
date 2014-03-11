#include "HyperCubeRandomGenerator.h"
#include <cstdlib>

namespace mdsearch
{

	/* Generate random real number r such that minimum <= r <= maximum. */
	Real generateRandomNumber(Real minimum, Real maximum)
	{
		return minimum + static_cast<float>(rand()) / static_cast<float>(RAND_MAX / (maximum - minimum));
	}

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