/******************************************************************************

mdsearch - Lightweight C++ library implementing a collection of
           multi-dimensional search structures

File:        types.hpp
Description: TODO

*******************************************************************************

The MIT License (MIT)

Copyright (c) 2014 Donald Whyte

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

******************************************************************************/

#ifndef MDSEARCH_TYPES_H
#define MDSEARCH_TYPES_H

#include <iostream>
#include <vector>
#include <cmath>

namespace mdsearch
{

	typedef float Real;
	typedef std::vector<Real> RealList;

	typedef long HashType;

	template <int D>
	struct Point
	{

		Point()
		{
		}

		Point(const Real* initialValues)
		{
			memcpy(values, initialValues, sizeof(Real) * D);
		}

		inline bool operator==(const Point& other) const
		{
			for (unsigned int d = 0; (d < D); d++)
				if (compare(values[d], other.values[d]) != 0) 
					return false;
			return true;
		}

		inline bool operator!=(const Point& other) const
		{
			return !(*this == other);
		}

		inline Real operator[](int index) const
		{
			return values[index];
		}

		inline Real& operator[](int index)
		{
			return values[index];
		}

		inline Real sum() const
		{
			Real s = 0;
			for (unsigned int d = 0; (d < D); d++)
				s += values[d];
			return s;	
		}

		Real values[D];

	};

	template<int D>
	struct Boundary
	{

		Real values[D * 2];

		inline Real minVal(int d) const { return values[d*2]; }
		inline Real maxVal(int d) const { return values[d*2+1]; }
		inline Real& minVal(int d) { return values[d*2]; }
		inline Real& maxVal(int d) { return values[d*2+1]; }

	};

	template<int D>
	std::ostream& operator<<(std::ostream& out, const Point<D>& point)
	{
		out << "(";
		for (int i = 0; (i < D - 1); i++)
			out << point[i] << ",";
		out << point[D - 1] << ")";
		return out;
	}

	/* Defined error tolerance for floating point comparisons. */
	static const Real EPSILON = 1.0e-7;
	/* Compare two Real values subject to an error tolerance.
	 * Return -1, 0, 1 respectively if t is less than,
	 * approximately equal to, or greater than, base. */
	inline int compare(Real t, Real base)
	{
		if (std::fabs(t - base) < EPSILON)
			return 0;
		if (t < base)
			return -1;
		return 1;
	}		

}

#endif