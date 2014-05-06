#ifndef MDSEARCH_DATASET_H
#define MDSEARCH_DATASET_H

#include "types.h"
#include <boost/lexical_cast.hpp>
#include <fstream>

namespace mdsearch
{

	template<int D>
	class Dataset
	{

	public:
		typedef std::vector< Point<D> > PointList;

		/* Add all given points to this dataset. */
		void load(const PointList& newPoints)
		{
			// Pre-allocate memory in one sys call
			points.reserve(points.size() + newPoints.size());
			// Append given points to end of current point list
			points.insert(points.end(), newPoints.begin(), newPoints.end());
		}

		/* Add all points in file to this dataset. */
		void load(const std::string& filename)
		{
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
			points.reserve(points.size() + numPoints);
			// Treat the rest of lines as points
			Real temp[D]; // temporary stores point's values
			std::stringstream ss;
			for (unsigned int i = 0; (i < numPoints); i++)
			{
				for (unsigned int j = 0; (j < numDimensions); j++)
				{
					file >> temp[j];
				}
				points.push_back(Point<D>(temp));

				// If we have reached the end of the file, STOP and don't try
				// reading any more points
				if (file.eof())
					break;
			}

			return points;
		}

		Boundary<D> computeBoundary() const
		{
			Boundary<D> boundary = { 0 };

			if (!points.empty())
			{
				// Use first point for dimensionality and initial boundary
				const Point<D>& firstPoint = points[0];
				for (unsigned int d = 0; (d < D); d++)
				{
					boundary.minVal(d) = firstPoint[d];
					boundary.maxVal(d) = firstPoint[d];
				}
				// Now search through rest of points in dataset to find
				// minimum and maximum values for each dimension
				for (typename PointList::const_iterator p = points.begin() + 1; (p != points.end()); p++)
				{
					for (unsigned int d = 0; (d < D); d++)
					{
						if ((*p)[d] < boundary.minVal(d))
							boundary.minVal(d) = (*p)[d];
						else if ((*p)[d] > boundary.maxVal(d))
							boundary.maxVal(d) = (*p)[d];
					}
				}
			}

			return boundary;
		}

		const PointList& getPoints() const
		{
			return points;
		}

	private:
		PointList points;

	};

}

#endif