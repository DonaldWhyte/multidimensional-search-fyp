#ifndef MDSEARCH_REGION_H
#define MDSEARCH_REGION_H

#include <vector>
#include <cmath>
#include "Point.h"

namespace mdsearch
{

	// Classes forward declared so typedefs and << overloads can be declared
	class Interval;
	class Region;
	// Typedefs for convenience
	typedef std::vector<Interval> IntervalList;
	typedef std::vector<Region> RegionList;	
	// Used to easily print intervals and regions
	std::ostream& operator<<(std::ostream& out, const Interval& interval);
	std::ostream& operator<<(std::ostream& out, const Region& region);

	struct Interval
	{
		Interval();
		Interval(Real min, Real max);

		/* Return true if given interval overlaps with this one. */
		inline bool intersects(const Interval& other) const
		{
			return (other.min <= max && min <= other.max);
		}

		const bool operator==(const Interval& other) const;
		const bool operator!=(const Interval& other) const;

		Real min;
		Real max;
	};

	class Region
	{

	public:
		Region(int n = 1, const Interval& initialInterval = Interval());
		Region(int n, const Interval* initialIntervals);

		inline unsigned int numDimensions() const
		{
			return nDimensions;
		}

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

		/* Interval overlapping equation received from:
		 * http://www.rgrjr.com/emacs/overlap.html */
		inline bool intersects(const Region& r) const
		{
			for (int i = 0; (i < nDimensions); i++)
			{
				// If the two regions' intervals for a given dimension
				// DO NOT OVERLAP, then the regions as a whole don't intersect
				if (!intervals[i].intersects(r[i]))
					return false;
			}
			return true;
		}

		// Subdivides the region into a collection of sub-regions by splitting
		// the original region in half along each dimension. This returns
		// 2^(numDimensions) sub-regions.
		inline RegionList subdivide() const
		{
			// Compute all possible intervals (two for each dimension)
			IntervalList subIntervals;
			subIntervals.reserve(nDimensions * 2);
			for (unsigned int i = 0; (i < nDimensions); i++)
			{	
				Real halfValue;
				// CORNER CASE: Interval has a size of zero (min == max)
				if (intervals[i].min == intervals[i].max)
					halfValue = intervals[i].min;
				else
					halfValue = intervals[i].min + (intervals[i].max - intervals[i].min) / 2.0f;

				subIntervals.push_back(Interval(intervals[i].min, halfValue));
				subIntervals.push_back(Interval(halfValue, intervals[i].max));
			}
			// Compute total number of sub-regions
			unsigned int numSubRegions = pow(2, nDimensions);
			// Create sub-region objects
			RegionList subRegions;
			subRegions.reserve(numSubRegions);
			for (unsigned int i = 0; (i < numSubRegions); i++)
			{
				subRegions.push_back(Region(nDimensions));
			}			
			// Populate sub-regions with correct intervals
			for (unsigned int d = 0; (d < nDimensions); d++)
			{
				int repeatCount = pow(2, d);
				repeatCount = std::max(1, repeatCount);

				unsigned int index = 0;
				for (unsigned int r = 0; (r < subRegions.size()); r++)
				{
					subRegions[r][d] = subIntervals[(d * 2) + index];
					if (((r + 1) % repeatCount) == 0)
					{
						index++;
						if (index >= 2)
						{
							index = 0;
						}
					}
				}
			}			

			return subRegions;
		}

		inline const Interval& operator[](int index) const
		{
			return intervals[index];
		}

		inline Interval& operator[](int index)
		{
			return intervals[index];
		}

		inline bool operator==(const Region& other) const
		{
			if (nDimensions != other.numDimensions())
				return false;
			for (unsigned int i = 0; (i < nDimensions); i++)
				if (intervals[i] != other[i])
					return false;
			return true;
		}


	private:
		// An interval (min-max value) for each dimension
		IntervalList intervals;
		unsigned int nDimensions;

	};

}

#endif