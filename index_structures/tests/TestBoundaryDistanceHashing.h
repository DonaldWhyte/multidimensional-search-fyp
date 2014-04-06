#ifndef MDSEARCH_TEST_BOUNDARYDISTANCEHASHING_H
#define MDSEARCH_TEST_BOUNDARYDISTANCEHASHING_H

#include <gtest/gtest.h>
#include "Common.h"
#include "BoundaryDistanceHashing.h"

namespace mdsearch { namespace tests
{

	static const unsigned int NUM_BDH_DIMENSIONS = 3;

	class BoundaryDistanceHashingTests : public ::testing::Test
	{

	protected:
		virtual void SetUp()
		{
			Interval initialBoundaryIntervals[3] = { Interval(0, 10), Interval(0, 10), Interval(0, 10) };
			initialBoundary = Region(3, initialBoundaryIntervals);			
		}
		
		Region initialBoundary;

	};

	TEST_F(BoundaryDistanceHashingTests, Construction)
	{
		BoundaryDistanceHashing structure(NUM_BDH_DIMENSIONS, initialBoundary);
		ASSERT_EQ(NUM_BDH_DIMENSIONS, structure.dimensionality());
		ASSERT_EQ(initialBoundary, structure.getBoundary());
	}

	TEST_F(BoundaryDistanceHashingTests, Clear)
	{
		BoundaryDistanceHashing structure(NUM_BDH_DIMENSIONS, initialBoundary);
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

	TEST_F(BoundaryDistanceHashingTests, InsertionAndRemoval)
	{
		BoundaryDistanceHashing structure(NUM_BDH_DIMENSIONS, initialBoundary);
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

	TEST_F(BoundaryDistanceHashingTests, Updating)
	{
		BoundaryDistanceHashing structure(IndexStructureTester::NUM_TEST_DIMENSIONS, initialBoundary);
		IndexStructureTester tester;
		tester.testUpdates(&structure);
	}

	TEST_F(BoundaryDistanceHashingTests, PointQueries)
	{
		BoundaryDistanceHashing structure(IndexStructureTester::NUM_TEST_DIMENSIONS, initialBoundary);
		IndexStructureTester tester;
		tester.testPointQueries(&structure);
	}

} }

#endif
