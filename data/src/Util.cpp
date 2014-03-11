#include "Util.h"
#include <cstdlib>
#include <fstream>

namespace mdsearch
{

	Real generateRandomNumber(Real minimum, Real maximum)
	{
		return minimum + static_cast<float>(rand()) / static_cast<float>(RAND_MAX / (maximum - minimum));
	}

	bool writePointsToFile(const std::string& filename, unsigned int numDimensions, const PointList& points)
	{
		if (numDimensions == 0)
			return false;

		std::ofstream file(filename.c_str());
		if (!file.is_open())
			return false;

		file << numDimensions << " " << points.size() << "\n";
		for (PointList::const_iterator it = points.begin(); (it != points.end()); it++)
		{
			const Point& p = *it;
			for (unsigned int d = 0; (d < numDimensions - 1); d++)
				file << p[d] << " ";
			file << p[numDimensions - 1] << "\n";
		}

		file.close();

		return true;
	}
	
}