#ifndef MDSEARCH_POINT_H
#define MDSEARCH_POINT_H

#include "DataTypes.h"

namespace mdsearch
{

	template<int N>
	class Point
	{

	public:
		Point(Real initialValue = 0)
		{
			for (int i = 0; (i < N); i++)
				values[i] = initialValue;
		}

		Point(Real* initialValues)
		{
			for (int i = 0; (i < N); i++)
				values[i] = initialValues[i];
		}

		inline Real operator[](int index)
		{
			return values[index];
		}

	private:
		Real values[N];

	};

}

#endif