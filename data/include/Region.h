#ifndef MDSEARCH_REGION_H
#define MDSEARCH_REGION_H

#include "Point.h"

namespace mdsearch
{

	struct Interval
	{
		Interval()
		{
			min = max = 0;			
		}
		Interval(Real min, Real max)
		{
			this->min = min;
			this->max = max;
		}

		Real min;
		Real max;
	};

	template<int N>
	class Region
	{

	public:
		Region()
		{
			for (int i = 0; (i < N); i++)
			{
				intervals = Interval();
			}
		}
		Region(Interval* dimensionIntervals)
		{
			for (int i = 0; (i < N); i++)
			{
				intervals = dimensionIntervals[i];
			}
		}

		inline bool inside(const Point<N>& p)
		{
			for (int i = 0; (i < N); i++)
			{
				if (p < intervals[i].min || p > intervals[i].max)
					return false;
			}
			return true;
		}

	private:
		// An interval (min-max value) for each dimension
		Interval intervals[N];

	};

}

#endif