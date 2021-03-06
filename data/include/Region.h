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

		/* Return minimum bounding region that contains all points. */
		inline static Region minimumBoundingBox(const PointList& points)
		{
			if (points.empty())
			{
				return Region(0);
			}
			else
			{
				// Get first point and use to initialise region
				Region minBox(points[0].numDimensions());
				for (unsigned int d = 0; (d < points[0].numDimensions()); d++)
				{
					minBox[d].min = minBox[d].max = points[0][d];
				}
				// Compute minimum and maximum values for each dimension
				// using the remainign points
				for (PointList::const_iterator it = points.begin() + 1; (it != points.end()); it++)
				{
					for (unsigned int d = 0; (d < it->numDimensions()); d++)
					{
						if ((*it)[d] < minBox[d].min)
							minBox[d].min = (*it)[d];
						else if ((*it)[d] > minBox[d].max)
							minBox[d].max = (*it)[d];
					}
				}
				return minBox;
			}
		}
		inline static Region minimumBoundingBox(const PointList& allPoints, const IndexList& indices)
		{
			if (indices.empty())
			{
				return Region(0);
			}
			else
			{
				// Get first point and use to initialise region
				int firstIndex = indices[0];
				int numDimensions = allPoints[firstIndex].numDimensions();
				Region minBox(numDimensions);
				for (unsigned int d = 0; (d < numDimensions); d++)
				{
					minBox[d].min = minBox[d].max = allPoints[firstIndex][d];
				}
				// Compute minimum and maximum values for each dimension
				// using the remainign points
				for (IndexList::const_iterator it = indices.begin() + 1; (it != indices.end()); it++)
				{
					for (unsigned int d = 0; (d < numDimensions); d++)
					{
						if (allPoints[*it][d] < minBox[d].min)
							minBox[d].min = allPoints[*it][d];
						else if (allPoints[*it][d] > minBox[d].max)
							minBox[d].max = allPoints[*it][d];
					}
				}
				return minBox;
			}
		}

		/* Return minimum bounding region that contains the given region and point. */
		inline static Region minimumBoundingBox(const Region& r, const Point& p)
		{
			// Find min and max values for each dimension of point and region
			Region minBoundBox(r.numDimensions(), Interval(0, 0));
			for (unsigned int d = 0; (d < r.numDimensions()); d++)
			{
				// NOTE: Assumes max >= min for all dimension intervals of region
				minBoundBox[d].min = std::min(r[d].min, p[d]);
				minBoundBox[d].max = std::max(r[d].max, p[d]);
			}
			return minBoundBox;
		}
		/* Return minimum bounding region that contains the given two points. */
		inline static Region minimumBoundingBox(const Point& a, const Point& b)
		{
			// Find min and max values for each dimension of two points
			Region minBoundBox(a.numDimensions(), Interval(0, 0));
			for (unsigned int d = 0; (d < a.numDimensions()); d++)
			{
				if (a[d] < b[d])
				{
					minBoundBox[d].min = a[d];
					minBoundBox[d].max = b[d];
				}
				else // new point < existing
				{
					minBoundBox[d].min = b[d];
					minBoundBox[d].max = a[d];
				}
			}
			return minBoundBox;
		}

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

		/* Return true if given region is contained within this one. */
		inline bool contains(const Region& r) const
		{
			for (int i = 0; (i < nDimensions); i++)
			{
				if (!(r[i].min >= intervals[i].min && r[i].min <= intervals[i].max)
					|| !(r[i].max >= intervals[i].min && r[i].max <= intervals[i].max))
				{
					return false;
				}
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

		/* Return index of the region's longest dimension (side). */
		inline int longestDimension() const
		{
			Real maxLength = -1; // guaranteed to be lower than any dimension
			int maxLengthIndex = -1;
			for (unsigned int d = 0; (d < nDimensions); d++)
			{
				Real length = (intervals[d].max - intervals[d].min);
				if (length > maxLength)
				{
					maxLength = length;
					maxLengthIndex = d;
				}
			}
			return maxLengthIndex;
		}
		/* Return length of longest dimension. */
		inline Real longestLength() const
		{
			Real maxLength = -1;
			for (unsigned int d = 0; (d < nDimensions); d++)
			{
				Real length = intervals[d].max - intervals[d].min;
				if (length > maxLength)
					maxLength = length;
			}
			return maxLength;
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