#ifndef MDSEARCH_UTIL_H
#define MDSEARCH_UTIL_H

#include "DataTypes.h"
#include "Point.h"
#include <cmath>
#include <algorithm>

namespace mdsearch
{

	/* Generate random real number r such that minimum <= r <= maximum. */
	Real generateRandomNumber(Real minimum, Real maximum);

	/* Remove element at given index from vector, using erase-remove
	 * idiom to prevent $(n) move operation. 
	 *
	 * NOTE: This deletes an element, but does NOT preserve the
	 * vector's order.
	 *
	 * NOTE: This does NOT perform a check to ensure the given index
	 * is within the bounds of the vector -- this be done by the calling
	 * code. */
	template <typename T>
	inline void removeElementAtIndex(std::vector<T>& vec, unsigned int index)
	{
		std::iter_swap(vec.begin() + index, vec.end() - 1);
		vec.erase(vec.end() - 1);
	}
	/* Same as removeElementAtIndex, but uses an iterator to represent
	 * the element that should be removed. */
	template <typename T>
	inline void removeElementAtIterator(std::vector<T>& vec, typename std::vector<T>::iterator it)
	{
		std::iter_swap(it, vec.end() - 1);
		vec.erase(vec.end() - 1);
	}	

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