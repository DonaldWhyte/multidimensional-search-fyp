#include "PointFactory.h"

namespace mdsearch
{

	WeakPoint::WeakPoint(int n, Real* initialValues) :
		nDimensions(n), values(initialValues)
	{
	}

	std::ostream& operator<<(std::ostream& out, const WeakPoint& point)
	{
		out << "(";
		for (int i = 0; (i < point.numDimensions() - 1); i++)
			out << point[i] << ",";
		out << point[point.numDimensions() - 1] << ")";
	}

	PointFactory::PointFactory(unsigned int memoryPoolSize) :
		numbersAllocated(0), poolSize(memoryPoolSize)
	{
		 memoryPool.resize(poolSize);
	}

	WeakPoint PointFactory::createPoint(unsigned int numDimensions,
		const Real* values)
	{
		// Check there's enough space in the pool
		// If not, return a zero-dimensional point
		// TODO: handle this more gracefully!
		if ((numbersAllocated + numDimensions) > poolSize)
		{
			return WeakPoint(0, NULL);
		}
		else
		{
			// Copy given values to the memory pool
			for (unsigned int i = 0; (i < numDimensions); i++)
				memoryPool[numbersAllocated + i] = values[i];
			WeakPoint point(numDimensions, &memoryPool[numbersAllocated]);
			numbersAllocated += numDimensions;
			return point;
		}
	}

	void PointFactory::clear()
	{
		std::fill(memoryPool.begin(), memoryPool.end(), 0);
		numbersAllocated = 0;
	}

}