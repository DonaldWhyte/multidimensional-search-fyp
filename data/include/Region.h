#ifndef MDSEARCH_REGION_H
#define MDSEARCH_REGION_H

#include <vector>
#include "Point.h"

namespace mdsearch
{

	struct Interval
	{
		Interval();
		Interval(Real min, Real max);

		const bool operator==(const Interval& other) const;
		const bool operator!=(const Interval& other) const;

		Real min;
		Real max;
	};

	typedef std::vector<Interval> IntervalList;

	class Region
	{

	public:
		Region(int n = 1, const Interval& initialInterval = Interval());
		Region(int n, const Interval* initialIntervals);

		// Inline functions have been declared in the header file
		// to prevent linker issues
		inline bool contains(const Point& p) const
		{
			for (int i = 0; (i < nDimensions); i++)
			{
				if (p[i] < intervals[i].min || p[i] > intervals[i].max)
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

		inline unsigned int numDimensions() const
		{
			return nDimensions;
		}


	private:
		// An interval (min-max value) for each dimension
		IntervalList intervals;
		unsigned int nDimensions;

	};

	typedef std::vector<Region> RegionList;

}

#endif