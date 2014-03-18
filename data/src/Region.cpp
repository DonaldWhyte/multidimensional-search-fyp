#include "Region.h"

namespace mdsearch
{

	Interval::Interval()
	{
		min = max = 0;
	}

	Interval::Interval(Real min, Real max)
	{
		this->min = min;
		this->max = max;
	}

	const bool Interval::operator==(const Interval& other) const
	{
		return (min == other.min && max == other.max);
	}

	const bool Interval::operator!=(const Interval& other) const
	{
		return !(*this == other);
	}

	Region::Region(int n, const Interval& initialInterval) : nDimensions(n)
	{
		intervals.reserve(nDimensions);
		for (int i = 0; (i < nDimensions); i++)
		{
			intervals.push_back(initialInterval);
		}
	}

	Region::Region(int n, const Interval* initialIntervals) : nDimensions(n)
	{
		intervals.reserve(nDimensions);
		for (int i = 0; (i < nDimensions); i++)
		{
			intervals.push_back(initialIntervals[i]);
		}
	}

	std::ostream& operator<<(std::ostream& out, const Interval& interval)
	{
		out << "[" << interval.min << "-" << interval.max << "]";
	}

	std::ostream& operator<<(std::ostream& out, const Region& region)
	{
		out << "(";
		for (unsigned int i = 0; (i < region.numDimensions() - 1); i++)
			out << region[i] << ", ";
		out << region[region.numDimensions() - 1];
		out << ")";
	}	

}