#ifndef MDSEARCH_TEST_SPLAYPSEUDOPYRAMIDTREE_H
#define MDSEARCH_TEST_SPLAYPSEUDOPYRAMIDTREE_H

#include <gtest/gtest.h>
#include "Common.h"
#include "SplayPseudoPyramidTree.h"

namespace mdsearch { namespace tests
{

	static const unsigned int NUM_SPLAYPYRAMIDTREE_DIMENSIONS = 3;

	class SplayPseudoPyramidTreeTests : public ::testing::Test
	{

	protected:
		virtual void SetUp()
		{
			Interval initialBoundaryIntervals[3] = { Interval(0, 10), Interval(0, 10), Interval(0, 10) };
			initialBoundary = Region(3, initialBoundaryIntervals);			
		}

		Region initialBoundary;

	};

	TEST_F(SplayPseudoPyramidTreeTests, Construction)
	{
		SplayPseudoPyramidTree structure(NUM_SPLAYPYRAMIDTREE_DIMENSIONS, initialBoundary);
		ASSERT_EQ(NUM_SPLAYPYRAMIDTREE_DIMENSIONS, structure.dimensionality());
		ASSERT_EQ(initialBoundary, structure.getBoundary());
	}

	TEST_F(SplayPseudoPyramidTreeTests, Clear)
	{
		SplayPseudoPyramidTree structure(NUM_SPLAYPYRAMIDTREE_DIMENSIONS, initialBoundary);
		IndexStructureTester tester;
		const PointList& testPoints = tester.getTestPoints();
		structure.loadPoints(testPoints);
		// Ensure all points loaded exist
		for (unsigned int i = 0; (i < testPoints.size()); i++)
			ASSERT_TRUE(structure.pointExists(testPoints[i]));
		// Clear the entire structure
		structure.clear();
		// Ensure all points are no longer store din structure
		for (unsigned int i = 0; (i < testPoints.size()); i++)
			ASSERT_FALSE(structure.pointExists(testPoints[i]));
	}

	TEST_F(SplayPseudoPyramidTreeTests, InsertionAndRemoval)
	{
		SplayPseudoPyramidTree structure(NUM_SPLAYPYRAMIDTREE_DIMENSIONS, initialBoundary);
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

	TEST_F(SplayPseudoPyramidTreeTests, Updating)
	{
		SplayPseudoPyramidTree structure(IndexStructureTester::NUM_TEST_DIMENSIONS, initialBoundary);
		IndexStructureTester tester;
		tester.testUpdates(&structure);
	}

	TEST_F(SplayPseudoPyramidTreeTests, PointQueries)
	{
		SplayPseudoPyramidTree structure(IndexStructureTester::NUM_TEST_DIMENSIONS, initialBoundary);
		IndexStructureTester tester;
		tester.testPointQueries(&structure);
	}

} }

#endif
