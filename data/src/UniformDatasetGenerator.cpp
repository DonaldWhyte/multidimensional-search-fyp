#include <cmath>
#include "UniformDatasetGenerator.h"

namespace mdsearch
{

	PointList UniformDatasetGenerator::generate(unsigned int numDimensions,
		Real min, Real max, unsigned int pointsPerDimension) const
	{
		if (numDimensions == 0)
			return PointList();

		// Pre-allocate memory for all points in the dataset
		PointList points;
		unsigned int numPoints = pow(pointsPerDimension, numDimensions);
		points.reserve(numPoints);
		for (unsigned int i = 0; (i < numPoints); i++)
			points.push_back(Point(numDimensions, 0.0f));
		// Compute sequence of all possible values of any coordinate
		std::vector<Real> valueSequence = generateUniformSequence(min, max, pointsPerDimension);

		for (unsigned int d = 0; (d < numDimensions); d++)
		{
			unsigned int index = 0;
			int repeatCount = pow(pointsPerDimension, d);
			repeatCount = std::max(1, repeatCount);
			for (unsigned int p = 0; (p < points.size()); p++)
			{
				points[p][d] = valueSequence[index];
				if (((p + 1) % repeatCount) == 0)
				{
					index++;
					if (index >= valueSequence.size())
						index = 0;
				}
			}
		}

		return points;
	}

	std::vector<Real> UniformDatasetGenerator::generateUniformSequence(
		Real min, Real max, unsigned int n) const
	{
		std::vector<Real> sequence;

		// Swap min and max if min > max
		if (min > max)
		{
			Real tmp = min;
			min = max;
			max = tmp;
		}
		if (n == 0)
		{
			return sequence;
		}
		else if (n == 1)
		{
			Real middleValue = min + (max - min) / 2;
			sequence.push_back(middleValue);
		}
		else if (n == 2)
		{
			sequence.push_back(min);
			sequence.push_back(max);
		}
		else
		{	
			if (min == max)
			{
				for (unsigned int i = 0; (i < n); i++)
					sequence.push_back(min);
			}
			else
			{
				Real spacing = (max - min) / (n - 1);
				Real tmp = 0;
				for (unsigned int i = 0; (i < n); i++)
					sequence.push_back( min + (spacing * i) );
			}
		}
		return sequence;
	}

}