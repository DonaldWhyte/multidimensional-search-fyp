#include "SkewedDatasetGenerator.h"
#include "Util.h"
#include <cmath>

namespace mdsearch
{

	PointList SkewedDatasetGenerator::generate(unsigned int numDimensions,
		const Point& min, const Point& max, unsigned int numPoints,
		Real skewSeverity, SkewDirection skewDirection) const
	{
		// Reserve enough memory for all the points
		PointList points;
		points.reserve(numPoints);
		// Temporarily stores generated point values
		RealList temp(numDimensions);

		if (skewDirection == SKEW_DIRECTION_LEFT)
		{
			for (unsigned int i = 0; (i < numPoints); i++)
			{
				for (unsigned int d = 0; (d < numDimensions); d++)
				{
					// Generate random number and raise to a power to skew value
					Real value = pow(generateRandomNumber(0, 1), skewSeverity);
					// Scale the value to match the spatial boundaries
					value *= generateRandomNumber(0, max[d] - min[d]); // use range as upperbound due to lower bound imposed next
					// Apply lower bound and store final value
					temp[d] = min[d] + value;
				}
				points.push_back(Point(numDimensions, &temp[0]));
			}
		}
		else if (skewDirection == SKEW_DIRECTION_RIGHT)
		{
			for (unsigned int i = 0; (i < numPoints); i++)
			{
				for (unsigned int d = 0; (d < numDimensions); d++)
				{
					// Reverse skew caused by exponent
					Real value = (1.0f - pow(generateRandomNumber(0, 1), skewSeverity));
					// Scale the value to match the spatial boundaries, but skew
					// the likelihood of larger values by modifying the lower bound
					// on the random scale factor
					Real range = max[d] - min[d];
					Real skewedLowerBound = range - (range * (1.0f / skewSeverity));
					value *= generateRandomNumber(skewedLowerBound, range);
					// Apply lower bound and store final value
					temp[d] = min[d] + value;				
				}
				points.push_back(Point(numDimensions, &temp[0]));
			}
		}

		return points;
	}

}