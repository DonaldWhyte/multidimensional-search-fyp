#ifndef MDSEARCH_TEST_BDTREE_H
#define MDSEARCH_TEST_BDTREE_H

#include <gtest/gtest.h>
#include "BDTree.h"

namespace mdsearch { namespace tests {

	static const unsigned int NUM_BDTREE_DIMENSIONS = 3;

	class BDTreeTests : public ::testing::Test
	{

	protected:
		virtual void SetUp()
		{
			Interval initialBoundaryIntervals[3] = { Interval(0, 10), Interval(0, 10), Interval(0, 10) };
			initialBoundary = Region(3, initialBoundaryIntervals);
		}

		Region initialBoundary;

	};

	TEST_F(BDTreeTests, Construction)
	{
		BDTree structure(NUM_BDTREE_DIMENSIONS, initialBoundary);
		// TODO: check emptyness
		ASSERT_EQ(NUM_BDTREE_DIMENSIONS, structure.dimensionality());

		structure = BDTree(8, initialBoundary);
		// TODO: check emptyness
		ASSERT_EQ(8, structure.dimensionality());
	}

	TEST_F(BDTreeTests, Clear)
	{
		IndexStructureTester tester;
		const PointList& testPoints = tester.getTestPoints();

		BDTree structure(NUM_BDTREE_DIMENSIONS, initialBoundary);
		// Test clear with already empty structure
		ASSERT_FALSE(structure.pointExists(testPoints[0]));
		structure.clear();
		ASSERT_FALSE(structure.pointExists(testPoints[0]));
		// Test clear with filled structure
		structure.loadPoints(testPoints);
		ASSERT_TRUE(structure.pointExists(testPoints[0]));
		structure.clear();
		ASSERT_FALSE(structure.pointExists(testPoints[0]));
	}

	TEST_F(BDTreeTests, Insertion)
	{
		BDTree structure(NUM_BDTREE_DIMENSIONS, initialBoundary);
		IndexStructureTester tester;
		const PointList& testPoints = tester.getTestPoints();

		// Test point that IS NOT CONAINED in the region the octree covers
		Real outsidePointValues[3] = { 30, 30, 30 };
		Point outsidePoint(3, outsidePointValues);
		ASSERT_FALSE(structure.insert(outsidePoint));
		ASSERT_FALSE(structure.pointExists(outsidePoint));
		
		// Insert point and check it's direct child of octree
		ASSERT_TRUE(structure.insert(testPoints[0]));
		ASSERT_TRUE(structure.pointExists(testPoints[0]));
		// Insert same point to ensure it is NOT inserted
		ASSERT_FALSE(structure.insert(testPoints[0]));
		ASSERT_TRUE(structure.pointExists(testPoints[0]));
	}

	TEST_F(BDTreeTests, Removal)
	{
		BDTree structure(NUM_BDTREE_DIMENSIONS, initialBoundary);
		IndexStructureTester tester;
		const PointList& testPoints = tester.getTestPoints();

		// Point deletion with empty structure
		EXPECT_FALSE(structure.pointExists(testPoints[1]));
		EXPECT_FALSE(structure.remove(testPoints[1]));
		EXPECT_FALSE(structure.pointExists(testPoints[1]));

		structure.insert(testPoints[0]);
		structure.insert(testPoints[1]);
		structure.insert(testPoints[2]);;
		ASSERT_TRUE(structure.pointExists(testPoints[0]));
		ASSERT_TRUE(structure.pointExists(testPoints[1]));
		ASSERT_TRUE(structure.pointExists(testPoints[2]));

		// Point deletion where point is not in non-empty structure
		EXPECT_FALSE(structure.pointExists(testPoints[4]));
		EXPECT_FALSE(structure.remove(testPoints[4]));
		EXPECT_FALSE(structure.pointExists(testPoints[4]));
		// Point deletion where point exists
		EXPECT_TRUE(structure.remove(testPoints[1]));
		EXPECT_FALSE(structure.pointExists(testPoints[1]));
	}

	TEST_F(BDTreeTests, Updating)
	{
		BDTree structure(IndexStructureTester::NUM_TEST_DIMENSIONS, initialBoundary);
		IndexStructureTester tester;
		tester.testUpdates(&structure);
	}

	TEST_F(BDTreeTests, PointQueries)
	{
		BDTree structure(IndexStructureTester::NUM_TEST_DIMENSIONS, initialBoundary);
		IndexStructureTester tester;
		tester.testPointQueries(&structure);
	}

	/*TEST_F(BDTreeTests, RegionQueries)
	{
		BDTree structure(IndexStructureTester::NUM_TEST_DIMENSIONS, initialBoundary);
		IndexStructureTester tester;
		tester.testRegionQueries(&structure);
	}*/

} }

#endif