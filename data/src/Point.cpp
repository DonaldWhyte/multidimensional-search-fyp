#include "Point.h"

namespace mdsearch
{

	Point::Point(int n, Real initialValue) : nDimensions(n)
	{
		values.reserve(nDimensions);
		for (int i = 0; (i < nDimensions); i++)
			values.push_back(initialValue);
	}

	Point::Point(int n, const Real* initialValues) : nDimensions(n)
	{
		values.reserve(nDimensions);
		for (int i = 0; (i < nDimensions); i++)
			values.push_back(initialValues[i]);
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