#ifndef MDSEARCH_POINTFACTORY_H
#define MDSEARCH_POINTFACTORY_H

#include "DataTypes.h"
#include <iostream>

namespace mdsearch
{

	/* Point which does not allocate, or manage, the point values 
	 * assigned to it. */
	class WeakPoint
	{

	public:
		/* Pointer containing all n elements must be given.
		 * This data will not be copied, so the data being pointed
		 * to miust not be deleted during the weak point's lifetime. */
		WeakPoint(int n, Real* initialValues);

		// Equality operators
		inline bool operator==(const WeakPoint& other) const
		{
			// Ensure the two points have the same dimensionality
			if (nDimensions != other.nDimensions)
			{
				return false;
			}
			else
			{
				// Perform equality check element-by-element
				for (int i = 0; (i < nDimensions); i++)
					if (values[i] != other.values[i]) // TODO: float equality? how to check?
						return false;
				return true;
			}
		}

		inline bool operator!=(const WeakPoint& other) const
		{
			return !(*this == other);
		}

		inline Real& operator[](int index) { return values[index]; }
		inline Real operator[](int index) const { return values[index]; }
		inline unsigned int numDimensions() const { return nDimensions; }

		inline Real sum() const
		{
			Real s = 0;
			for (int i = 0; (i < nDimensions); i++)
				s += values[i];
			return s;
		}

	private:
		Real* values;
		unsigned int nDimensions;

	};

	std::ostream& operator<<(std::ostream& out, const WeakPoint& point);
	typedef std::vector<WeakPoint> WeakPointList;

	/* Produces points whose values are stored in a large contiguous memory pool. */
	class PointFactory
	{

	public:
		/* Initialise factory pool with given size. */
		PointFactory(unsigned int memoryPoolSize);

		/* Create point with given dimensionality and values.
		 * 'values' should be a pointer to 'numDimensions' points.
		 * NOTE: The given values are COPIED to the factory's underlying
		 * memory pool. */
		WeakPoint createPoint(unsigned int numDimensions, const Real* values);
		/* Clear all the values currently stored in the factory.
		 * NOTE: This does not allocate new memory and the pool still has
		 * the same size. */
		void clear();

	private:
		RealList memoryPool;
		// Keep track of how many real numbers are currently being stored in the pool
		unsigned int numbersAllocated;
		// Total size of memory pool (how many real numbers can be stored)
		unsigned int poolSize;

	};

}

#endif