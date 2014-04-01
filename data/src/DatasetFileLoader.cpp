#include "DatasetFileLoader.h"

#include <fstream>
#include <sstream>
#include <boost/lexical_cast.hpp>

namespace mdsearch
{

	PointList DatasetFileLoader::load(const std::string& filename)
	{
		PointList points;

		// Open specified file and just return empty list of points if
		// the file does not exist
		std::ifstream file(filename.c_str());
		if (!file.is_open())
			return points;

		// Read header information
		std::string numDimensionsStr;
		file >> numDimensionsStr;
		std::string numPointsStr;
		file >> numPointsStr;
		// Convert strings to integers
		int numDimensions = 0;
		int numPoints = 0;
		try
		{
			numDimensions = boost::lexical_cast<int>(numDimensionsStr);
			numPoints = boost::lexical_cast<int>(numPointsStr);
		}
		catch (boost::bad_lexical_cast& ex) // not integers -- invalid file!!
		{
			return points;
		}
		// Only continue reading points if the points have at least
		// one dimension and there is at least one point in the dataset
		if (numDimensions < 1 || numPoints < 1)
			return points;

		// Pre-allocate memory to store all the specified points
		points.reserve(numPoints);
		// Treat the rest of lines as a point each
		std::vector<Real> temp(numDimensions); // temporary stores point's values
		std::stringstream ss;
		for (unsigned int i = 0; (i < numPoints); i++)
		{
			for (unsigned int j = 0; (j < numDimensions); j++)
			{
				file >> temp[j];
			}
			points.push_back(Point(numDimensions, &temp[0]));

			// If we have reached the end of the file, STOP and don't try
			// reading any more points
			if (file.eof())
				break;
		}

		return points;
	}

}