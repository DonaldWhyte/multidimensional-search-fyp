#ifndef MDSEARCH_TEST_SEQUENTIALSCAN_H
#define MDSEARCH_TEST_SEQUENTIALSCAN_H

#include <gtest/gtest.h>
#include "SequentialScan.h"

namespace mdsearch { namespace tests
{

	class SequentialScanTests : public ::testing::Test
	{

	protected:
		virtual void SetUp()
		{
			// Construct test dataset
			Real testPointValues[NUM_TEST_POINTS][NUM_DIMENSIONS] = {
				{9, 2, 0}, {3, 6, 6}, {4, 0, 3}, {6, 5, 8}, {6, 7, 6},
				{1, 3, 0}, {6, 9, 0}, {2, 4, 7}, {4, 5, 9}, {5, 6, 0}
			};
			testPoints.reserve(NUM_TEST_POINTS);
			for (unsigned int i = 0; (i < NUM_TEST_POINTS); i++)
				testPoints.push_back( Point(NUM_DIMENSIONS, testPointValues[i]) );
		}

		virtual void TearDown()
		{
			// Clear test dataset
			testPoints.clear();
		}

		// Constants about test data
		static const unsigned int NUM_TEST_POINTS = 10;
		static const unsigned int NUM_DIMENSIONS = 3;
		PointList testPoints;

	};

	TEST_F(SequentialScanTests, Construction)
	{
		SequentialScan structure(NUM_DIMENSIONS);
		ASSERT_EQ(0, structure.allPoints().size());

		structure.loadPoints(testPoints);
		ASSERT_EQ(testPoints, structure.allPoints());
		// Testing no duplicates are added
		structure.loadPoints(testPoints);
		ASSERT_EQ(testPoints, structure.allPoints());
	}

	TEST_F(SequentialScanTests, Insertion)
	{
		SequentialScan structure(NUM_DIMENSIONS);

		// Insert same point twice -- SHOULD ONLY BE INSERTED ONCE
		// SINCE WE ARE NOT STORING DUPLICATES!
		structure.insert(testPoints[3]);
		ASSERT_EQ(1, structure.allPoints().size());
		ASSERT_EQ(testPoints[3], structure.allPoints()[0]);
		structure.insert(testPoints[3]);
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

	TEST_F(SequentialScanTests, Removal)
	{
		SequentialScan structure(NUM_DIMENSIONS);

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
		// Point deletition where point exists
		EXPECT_TRUE(structure.remove(testPoints[1]));
		expectedPoints.erase(expectedPoints.begin() + 1);
		EXPECT_EQ(expectedPoints, structure.allPoints()); // ensure point has been removed!
	}

	TEST_F(SequentialScanTests, Updating)
	{
		SequentialScan structure(NUM_DIMENSIONS);
		structure.loadPoints(testPoints);

		// Test with points that don't exist in structure
		EXPECT_FALSE( structure.pointExists(Point(1, 0.0f)) );
		EXPECT_FALSE( structure.pointExists(Point(3, 0.0f)) );
		EXPECT_FALSE( structure.pointExists(Point(10, 5.6f)) );
	}

	TEST_F(SequentialScanTests, PointQueries)
	{
		SequentialScan structure(NUM_DIMENSIONS);
		structure.loadPoints(testPoints);

		// Test points that don't exist in structure
		EXPECT_FALSE( structure.pointExists(Point(1, 0.0f)) );
		EXPECT_FALSE( structure.pointExists(Point(3, 0.0f)) );
		EXPECT_FALSE( structure.pointExists(Point(10, 5.6f)) );
		// Test points that do exist
		EXPECT_TRUE( structure.pointExists(testPoints[0]) );
		EXPECT_TRUE( structure.pointExists(testPoints[4]) );
		EXPECT_TRUE( structure.pointExists(testPoints[9]) );
	}

	TEST_F(SequentialScanTests, RegionQueries)
	{
		SequentialScan structure(NUM_DIMENSIONS);
		structure.loadPoints(testPoints);

		// Construct test regions
		Interval zeroSizedRegion1Intervals[3] = { Interval(0, 0), Interval(0, 0), Interval(0, 0) };
		Interval zeroSizedRegion2Intervals[3] = { Interval(4, 4), Interval(0, 0), Interval(3, 3) };
		Interval emptyRegionIntervals[3] = { Interval(-10, -5), Interval(-10, -5), Interval(-10, -5) };
		Interval singlePointRegionIntervals[3] = { Interval(3.5, 5), Interval(-1, 1), Interval(-7, 3.1) };
		Interval multiplePointsRegionIntervals[3] = { Interval(0, 10), Interval(2, 6), Interval(2, 7) };
		Interval allPointsRegionIntervals[3] = { Interval(0, 10), Interval(0, 10), Interval(0, 10) };

		Region zeroSizedRegion1(3, zeroSizedRegion1Intervals);
		Region zeroSizedRegion2(3, zeroSizedRegion2Intervals);
		Region emptyRegion(3, emptyRegionIntervals);
		Region singlePointRegion(3, singlePointRegionIntervals);
		Region multiplePointsRegion(3, multiplePointsRegionIntervals);
		Region allPointsRegion(3, allPointsRegionIntervals);

		// Construct lists of expected points in test regions
		PointList zeroSizedRegion2Points;
		zeroSizedRegion2Points.push_back(testPoints[2]);
		PointList singlePointRegionPoints;
		singlePointRegionPoints.push_back(testPoints[2]);
		PointList multiplePointsRegionPoints;
		multiplePointsRegionPoints.push_back(testPoints[1]);
		multiplePointsRegionPoints.push_back(testPoints[7]);

		// Test with zero-size region that is NOT a point
		EXPECT_EQ(PointList(), structure.pointsInRegion(zeroSizedRegion1));
		// Test with zero-size region that IS A POINT
		EXPECT_EQ(zeroSizedRegion2Points, structure.pointsInRegion(zeroSizedRegion2));
		// Test with region that contains no points
		EXPECT_EQ(PointList(), structure.pointsInRegion(emptyRegion));
		// Test with region that contains one point
		EXPECT_EQ(singlePointRegionPoints, structure.pointsInRegion(singlePointRegion));
		// Test with region that contains multiple points
		EXPECT_EQ(multiplePointsRegionPoints, structure.pointsInRegion(multiplePointsRegion));
		// Test with region that contains all points
		EXPECT_EQ(testPoints, structure.pointsInRegion(allPointsRegion));
	}


} }

#endif