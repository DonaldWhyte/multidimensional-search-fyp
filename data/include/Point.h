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

		// Inline functions have been declared in the header file
		// to prevent linker issues
		// Equality operators
		inline bool operator==(const Point& other) const
		{
			// Ensure the two points have the same dimensionality
			if (nDimensions != other.nDimensions)
			{
				return false;
			}
			else
			{
				// Perform equality check element-by-element
				for (int i = 0; (i < nDimensions); i++)
					if (values[i] != other.values[i]) // TODO: float equality? how to check?
						return false;
				return true;
			}
		}

		inline bool operator!=(const Point& other) const
		{
			return !(*this == other);
		}

		inline Real& operator[](int index) { return values[index]; }
		inline Real operator[](int index) const { return values[index]; }
		inline unsigned int numDimensions() const { return nDimensions; }

		/* Return sum of all elements/coordinates of this point. */
		inline Real sum() const
		{
			Real s = 0;
			for (int i = 0; (i < nDimensions); i++)
				s += values[i];
			return s;
		}

	private:
		RealList values;
		unsigned int nDimensions;

	};

	std::ostream& operator<<(std::ostream& out, const Point& point);
	
	typedef std::vector<Point> PointList;

}

#endif