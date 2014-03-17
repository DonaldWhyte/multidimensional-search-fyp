#ifndef MDSEARCH_UTIL_H
#define MDSEARCH_UTIL_H

#include "DataTypes.h"
#include "Point.h"
#include <cmath>

namespace mdsearch
{

	/* Generate random real number r such that minimum <= r <= maximum. */
	Real generateRandomNumber(Real minimum, Real maximum);

	/* Write generated points to a text file.
	 * Format:
	 * d n
	 * p_1[0] p_1[1] ... p_1[d - 1]
	 * p_1[0] p_2[1] ... p_2[d - 1]
	 * ...
	 * p_n[0] p_n[1] ... p_n[d - 1]
	 * where d = number of dimensions, n = number of points
	 * and p_i[j] is the value of the jth coordinate of point i.
	 * 
	 * Returns true if file was written successfully and false otherwise.
	*/
	bool writePointsToFile(const std::string& filename,
		unsigned int numDimensions, const PointList& points);

	/* Defined error tolrerance for floating point comparisons. */
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