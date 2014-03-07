#ifndef MDSEARCH_POINT_H
#define MDSEARCH_POINT_H

#include <iostream>
#include <vector>
#include "DataTypes.h"

namespace mdsearch
{

	class Point
	{

	public:
		Point(int n, Real initialValue = 0);
		Point(int n, const Real* initialValues);

		inline bool operator==(const Point& other) const
		{
			// Ensure the two points have the same dimensionality
			if (nDimensions != other.numDimensions())
				return false;
			// Perform equality check element-by-element
			for (int i = 0; (i < nDimensions); i++)
				if (values[i] != other[i]) // TODO: float equality? how to check?
					return false;
			return true;
		}
		inline bool operator!=(const Point& other) const
		{
			return !(*this == other);
		}

		// Inline functions have been declared in the header file
		// to prevent linker issues
		inline Real operator[](int index) const
		{
			return values[index];
		}

		inline unsigned int numDimensions() const
		{
			return nDimensions;
		}


	private:
		RealList values;
		unsigned int nDimensions;

	};

	// Used to easily print a point
	std::ostream& operator<<(std::ostream& out, const Point& point);

	typedef std::vector<Point> PointList;

}

#endif