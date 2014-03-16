#ifndef MDSEARCH_TEST_PYRAMIDTREE_H
#define MDSEARCH_TEST_PYRAMIDTREE_H

#include <gtest/gtest.h>
#include "Common.h"
#include "PyramidTree.h"

namespace mdsearch { namespace tests
{

	class PyramidTreeTests : public ::testing::Test
	{

	protected:
		virtual void SetUp()
		{
			Interval initialBoundaryIntervals[3] = { Interval(0, 10), Interval(0, 10), Interval(0, 10) };
			initialBoundary = Region(3, initialBoundaryIntervals);			
		}

		static const unsigned int NUM_DIMENSIONS = 3;
		Region initialBoundary;

	};

	TEST_F(PyramidTreeTests, Construction)
	{
		PyramidTree structure(NUM_DIMENSIONS, initialBoundary);
		IndexStructureTester tester;
		const PointList& testPoints = tester.getTestPoints();		

		ASSERT_EQ(0, structure.allPoints().size());

		structure.loadPoints(testPoints);
		ASSERT_EQ(testPoints, structure.allPoints());
		// Testing no duplicates are added
		structure.loadPoints(testPoints);
		ASSERT_EQ(testPoints, structure.allPoints());
	}

	TEST_F(PyramidTreeTests, Clear)
	{
		PyramidTree structure(NUM_DIMENSIONS, initialBoundary);
		IndexStructureTester tester;
		const PointList& testPoints = tester.getTestPoints();

		ASSERT_EQ(PointList(), structure.allPoints());
		// Testing clear with already empty structure
		structure.clear();
		ASSERT_EQ(PointList(), structure.allPoints());
		// Testing c
		structure.loadPoints(testPoints);
		ASSERT_EQ(testPoints, structure.allPoints());
		structure.clear();
		ASSERT_EQ(PointList(), structure.allPoints());
	}

	TEST_F(PyramidTreeTests, Insertion)
	{
		PyramidTree structure(NUM_DIMENSIONS, initialBoundary);
		IndexStructureTester tester;
		const PointList& testPoints = tester.getTestPoints();

		// Insert same point twice -- SHOULD ONLY BE INSERTED ONCE
		// SINCE WE ARE NOT STORING DUPLICATES!
		ASSERT_TRUE( structure.insert(testPoints[3]) );
		ASSERT_EQ(1, structure.allPoints().size());
		ASSERT_EQ(testPoints[3], structure.allPoints()[0]);
		ASSERT_FALSE( structure.insert(testPoints[3]) );
		ASSERT_EQ(1, structure.allPoints().size());
		ASSERT_EQ(testPoints[3], structure.allPoints()[0]);
		// Insert two different points and check both points exist, in correct order
		PointList expectedPoints;
		expectedPoints.push_back(testPoints[3]);
		expectedPoints.push_back(testPoints[1]);
		expectedPoints.push_back(testPoints[0]);
		structure.insert(testPoints[1]);
		structure.insert(testPoints[0]);
		ASSERT_EQ(expectedPoints, structure.allPoints());
	}

	TEST_F(PyramidTreeTests, Removal)
	{
		PyramidTree structure(NUM_DIMENSIONS, initialBoundary);
		IndexStructureTester tester;
		const PointList& testPoints = tester.getTestPoints();

		// Point deletion with empty structure
		EXPECT_FALSE(structure.remove(testPoints[1]));

		structure.insert(testPoints[0]);
		structure.insert(testPoints[1]);
		structure.insert(testPoints[2]);
		PointList expectedPoints;
		expectedPoints.push_back(testPoints[0]);
		expectedPoints.push_back(testPoints[1]);
		expectedPoints.push_back(testPoints[2]);
		ASSERT_EQ(expectedPoints, structure.allPoints());

		// Point deletion where point is not in non-empty structure
		EXPECT_FALSE(structure.remove(testPoints[4]));
		// Point deletion where point exists
		EXPECT_TRUE(structure.pointExists(testPoints[1]));
		EXPECT_TRUE(structure.remove(testPoints[1]));
		EXPECT_FALSE(structure.pointExists(testPoints[1])); // ensure point has been removed!
	}

	TEST_F(PyramidTreeTests, Updating)
	{
		PyramidTree structure(IndexStructureTester::NUM_TEST_DIMENSIONS, initialBoundary);
		IndexStructureTester tester;
		tester.testUpdates(&structure);
	}

	TEST_F(PyramidTreeTests, PointQueries)
	{
		PyramidTree structure(IndexStructureTester::NUM_TEST_DIMENSIONS, initialBoundary);
		IndexStructureTester tester;
		tester.testPointQueries(&structure);
	}

	TEST_F(PyramidTreeTests, RegionQueries)
	{
		PyramidTree structure(IndexStructureTester::NUM_TEST_DIMENSIONS, initialBoundary);
		IndexStructureTester tester;
		tester.testRegionQueries(&structure);
	}

	TEST_F(PyramidTreeTests, Defragment)
	{
		// Make max empty elements 2 for testing purposes
		PyramidTree structure(IndexStructureTester::NUM_TEST_DIMENSIONS, initialBoundary, 2);
		IndexStructureTester tester;
		const PointList& testPoints = tester.getTestPoints();
		// Insert some points into the structure
		structure.insert(testPoints[0]);
		structure.insert(testPoints[1]);
		structure.insert(testPoints[2]);
		structure.insert(testPoints[3]);
		ASSERT_EQ(4, structure.allPoints().size());
		ASSERT_TRUE(structure.pointExists(testPoints[0]));
		ASSERT_TRUE(structure.pointExists(testPoints[1]));
		ASSERT_TRUE(structure.pointExists(testPoints[2]));
		ASSERT_TRUE(structure.pointExists(testPoints[3]));		
		// Remove one point and ensure nothing is deleted from point list
		structure.remove(testPoints[0]);
		ASSERT_EQ(4, structure.allPoints().size());
		ASSERT_EQ(1, structure.emptyIndices().size());
		ASSERT_EQ(0, structure.emptyIndices()[0]);
		// Remove another point. Empty indices should now be empty and the two
		// points should be removed (WITH THE ORIGINAL ORDER OF INSERTION INTACT!)
		structure.remove(testPoints[2]);
		ASSERT_EQ(2, structure.allPoints().size());
		ASSERT_EQ(testPoints[1], structure.allPoints()[0]);
		ASSERT_EQ(testPoints[3], structure.allPoints()[1]);
		ASSERT_EQ(0, structure.emptyIndices().size());
		// Test point queries work correctly after defragmentation
		EXPECT_FALSE(structure.pointExists(testPoints[0]));
		EXPECT_TRUE(structure.pointExists(testPoints[1]));
		EXPECT_FALSE(structure.pointExists(testPoints[2]));
		EXPECT_TRUE(structure.pointExists(testPoints[3]));
	}

} }

#endif