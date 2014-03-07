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

}