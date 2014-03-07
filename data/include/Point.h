#ifndef MDSEARCH_POINT_H
#define MDSEARCH_POINT_H

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

}

#endif