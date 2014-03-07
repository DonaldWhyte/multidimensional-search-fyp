#ifndef MDSEARCH_POINT_H
#define MDSEARCH_POINT_H

#include <iostream>
#include <vector>
#include "DataTypes.h"

namespace mdsearch
{

	/* Return maximum of given array of real numbers. */
	inline Real MaximumValue(const RealList& values)
	{
		if (values.size() == 0) return 0;
		Real max = values[0];
		for (unsigned int i = 0; (i < values.size()); i++)
			if (values[i] > max)
				max = values[i];
		return max;
	}

	class Point
	{

	public:
		Point(int n, Real initialValue = 0);
		Point(int n, const Real* initialValues);

		// Equality operators
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
		// Comparison
		// NOTE: Assumes points have the same number of dimnesions! It may
		// crash if this is not the case. TODO: prevent this efficiently!
		inline bool operator<(const Point& other) const
		{
			Real thisMax = MaximumValue(values);
			Real otherMax = MaximumValue(other.values);
			return (thisMax < otherMax);
			/*for (unsigned int i = 0; (i < nDimensions); i++)
				if (values[i] < other.values[i])
					return true;
			return false;*/
		}

		// Inline functions have been declared in the header file
		// to prevent linker issues
		inline Real& operator[](int index) { return values[index]; }
		inline Real operator[](int index) const { return values[index]; }
		inline unsigned int numDimensions() const { return nDimensions; }


	private:
		RealList values;
		unsigned int nDimensions;

	};

	// Used to easily print a point
	std::ostream& operator<<(std::ostream& out, const Point& point);

	typedef std::vector<Point> PointList;

}

#endif