#ifndef MDSEARCH_TEST_IMINMAX_H
#define MDSEARCH_TEST_IMINMAX_H

#include <gtest/gtest.h>
#include "Common.h"
#include "IMinMax.h"

namespace mdsearch { namespace tests
{

	static const unsigned int NUM_IMINMAX_DIMENSIONS = 3;
	static const Real IMINMAX_THETA = 1.0f;

	class IMinMaxTests : public ::testing::Test
	{

	protected:
		virtual void SetUp()
		{
			Interval initialBoundaryIntervals[3] = { Interval(0, 10), Interval(0, 10), Interval(0, 10) };
			initialBoundary = Region(3, initialBoundaryIntervals);			
		}
		
		Region initialBoundary;

	};

	TEST_F(IMinMaxTests, Construction)
	{
		IMinMax structure(NUM_IMINMAX_DIMENSIONS, initialBoundary, IMINMAX_THETA);
		ASSERT_EQ(NUM_IMINMAX_DIMENSIONS, structure.dimensionality());
		ASSERT_EQ(initialBoundary, structure.getBoundary());
	}

	TEST_F(IMinMaxTests, Clear)
	{
		IMinMax structure(NUM_IMINMAX_DIMENSIONS, initialBoundary, IMINMAX_THETA);
		IndexStructureTester tester;
		const PointList& testPoints = tester.getTestPoints();
		structure.loadPoints(testPoints);
		// Ensure all points loaded exist
		for (unsigned int i = 0; (i < testPoints.size()); i++)
			ASSERT_TRUE(structure.pointExists(testPoints[i]));
		// Clear the entire structure
		structure.clear();
		// Ensure all points are no longer stored in structure
		for (unsigned int i = 0; (i < testPoints.size()); i++)
			ASSERT_FALSE(structure.pointExists(testPoints[i]));
	}

	TEST_F(IMinMaxTests, InsertionAndRemoval)
	{
		IMinMax structure(NUM_IMINMAX_DIMENSIONS, initialBoundary, IMINMAX_THETA);
		IndexStructureTester tester;
		const PointList& testPoints = tester.getTestPoints();

		// Insert each point, ensuring that point and the points
		// previously inserted can be accessed
		for (unsigned int i = 0; (i < testPoints.size()); i++)
		{
			ASSERT_TRUE(structure.insert(testPoints[i]));
			for (unsigned int j = 0; (j <= i); j++)
				ASSERT_TRUE(structure.pointExists(testPoints[j]));
			for (unsigned int j = i + 1; (j < testPoints.size()); j++)
				ASSERT_FALSE(structure.pointExists(testPoints[j]));
		}

		// Now do the same for remove()
		for (unsigned int i = 0; (i < testPoints.size()); i++)
		{
			ASSERT_TRUE(structure.remove(testPoints[i]));
			for (unsigned int j = 0; (j <= i); j++)
				ASSERT_FALSE(structure.pointExists(testPoints[j]));
			for (unsigned int j = i + 1; (j < testPoints.size()); j++)
				ASSERT_TRUE(structure.pointExists(testPoints[j]));
		}
	}

	TEST_F(IMinMaxTests, Updating)
	{
		IMinMax structure(IndexStructureTester::NUM_TEST_DIMENSIONS, initialBoundary, IMINMAX_THETA);
		IndexStructureTester tester;
		tester.testUpdates(&structure);
	}

	TEST_F(IMinMaxTests, PointQueries)
	{
		IMinMax structure(IndexStructureTester::NUM_TEST_DIMENSIONS, initialBoundary, IMINMAX_THETA);
		IndexStructureTester tester;
		tester.testPointQueries(&structure);
	}

} }

#endif
