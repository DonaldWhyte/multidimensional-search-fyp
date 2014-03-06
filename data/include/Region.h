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

		const bool operator==(const Interval& other) const
		{
			return (min == other.min && max == other.max);
		}

		const bool operator!=(const Interval& other) const
		{
			return !(*this == other);
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
				intervals[i] = Interval();
			}
		}

		Region(const Interval& initialInterval)
		{
			for (int i = 0; (i < N); i++)
			{
				intervals[i] = initialInterval;
			}
		}

		Region(Interval* dimensionIntervals)
		{
			for (int i = 0; (i < N); i++)
			{
				intervals[i] = dimensionIntervals[i];
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

		inline const Interval& operator[](int index) const
		{
			return intervals[index];
		}

		inline Interval& operator[](int index)
		{
			return intervals[index];
		}

	private:
		// An interval (min-max value) for each dimension
		Interval intervals[N];

	};

}

#endif