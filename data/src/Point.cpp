#include "Point.h"

namespace mdsearch
{

	Point::Point(int n, Real initialValue) : nDimensions(n)
	{
		#ifdef MDSEARCH_USE_SSE_POINT_COMPARISON
			int leftover = (nDimensions % 4);
			int numbersToPad = (leftover) ? 4 - leftover : 0;
			values.reserve(nDimensions + numbersToPad);
			for (int i = 0; (i < nDimensions); i++)
				values.push_back(initialValue);
			for (int i = 0; (i < numbersToPad); i++)
				values.push_back(1.0f);
		#else
			values.reserve(nDimensions);
			for (int i = 0; (i < nDimensions); i++)
				values.push_back(initialValue);
		#endif
	}

	Point::Point(int n, const Real* initialValues) : nDimensions(n)
	{
		#ifdef MDSEARCH_USE_SSE_POINT_COMPARISON
			int leftover = (nDimensions % 4);
			int numbersToPad = (leftover) ? 4 - leftover : 0;
			values.reserve(nDimensions + numbersToPad);
			for (int i = 0; (i < nDimensions); i++)
				values.push_back(initialValues[i]);
			for (int i = 0; (i < numbersToPad); i++)
				values.push_back(1.0f);
		#else
			values.reserve(nDimensions);
			for (int i = 0; (i < nDimensions); i++)
				values.push_back(initialValues[i]);
		#endif
	}

	std::ostream& operator<<(std::ostream& out, const Point& point)
	{
		if (point.numDimensions() == 0)
		{
			out << "(-)";
		}
		else
		{
			out << "(";
			for (int i = 0; (i < point.numDimensions() - 1); i++)
				out << point[i] << ",";
			out << point[point.numDimensions() - 1] << ")";
		}
		return out;
	}
	
}