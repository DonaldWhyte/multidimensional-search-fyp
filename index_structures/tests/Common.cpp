#include "Common.h"
#include <gtest/gtest.h>

namespace mdsearch { namespace tests
{

	IndexStructureTester::IndexStructureTester()
	{
		// Construct test dataset
		Real testPointValues[NUM_TEST_POINTS][NUM_TEST_DIMENSIONS] = {
			{9, 2, 0}, {3, 6, 6}, {4, 0, 3}, {6, 5, 8}, {6, 7, 6},
			{1, 3, 0}, {6, 9, 0}, {2, 4, 7}, {4, 5, 9}, {5, 6, 0}
		};
		testPoints.reserve(NUM_TEST_POINTS);
		for (unsigned int i = 0; (i < NUM_TEST_POINTS); i++)
			testPoints.push_back( Point(NUM_TEST_DIMENSIONS, testPointValues[i]) );
	}

	const PointList& IndexStructureTester::getTestPoints() const
	{
		return testPoints;
	}

	void IndexStructureTester::testUpdates(IndexStructure* structure)
	{
		structure->loadPoints(testPoints);

		// Test with point not stored in structure
		Point doesNotExist(NUM_TEST_DIMENSIONS, -10);
		ASSERT_FALSE(structure->pointExists(doesNotExist));
		EXPECT_FALSE(structure->update(doesNotExist, testPoints[0]));
		// Test with point stored in structure, but where the new
		// point IS ALREADY IN THE STRUCTURE
		// (should delete point but not add a new one)
		ASSERT_TRUE(structure->pointExists(testPoints[0]));
		ASSERT_TRUE(structure->pointExists(testPoints[1]));
		EXPECT_TRUE(structure->update(testPoints[0], testPoints[1]));
		EXPECT_FALSE(structure->pointExists(testPoints[0]));
		// Test with point stored in structure where new point is
		// NOT stored in structure
		Point newPoint(NUM_TEST_DIMENSIONS, 5.5);
		ASSERT_TRUE(structure->pointExists(testPoints[1]));
		ASSERT_FALSE(structure->pointExists(newPoint));
		EXPECT_TRUE(structure->update(testPoints[1], newPoint));
		EXPECT_FALSE(structure->pointExists(testPoints[1]));
		EXPECT_TRUE(structure->pointExists(newPoint));		
	}

	void IndexStructureTester::testPointQueries(IndexStructure* structure)
	{
		structure->loadPoints(testPoints);

		// Test points that don't exist in structure
		EXPECT_FALSE( structure->pointExists(Point(1, 0.0f)) );
		EXPECT_FALSE( structure->pointExists(Point(3, 0.0f)) );
		EXPECT_FALSE( structure->pointExists(Point(10, 5.6f)) );
		// Test points that do exist
		for (unsigned int i = 0; (i < testPoints.size()); i++)
			EXPECT_TRUE( structure->pointExists(testPoints[i]) );
	}

	void IndexStructureTester::testRegionQueries(IndexStructure* structure)
	{
		structure->loadPoints(testPoints);

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
		EXPECT_TRUE( vectorsContainSameElements(PointList(), structure->pointsInRegion(zeroSizedRegion1)) );
		// Test with zero-size region that IS A POINT
		EXPECT_TRUE( vectorsContainSameElements(zeroSizedRegion2Points, structure->pointsInRegion(zeroSizedRegion2)) );
		// Test with region that contains no points
		EXPECT_TRUE( vectorsContainSameElements(PointList(), structure->pointsInRegion(emptyRegion)) );
		// Test with region that contains one point
		EXPECT_TRUE( vectorsContainSameElements(singlePointRegionPoints, structure->pointsInRegion(singlePointRegion)) );
		// Test with region that contains multiple points
		EXPECT_TRUE( vectorsContainSameElements(multiplePointsRegionPoints, structure->pointsInRegion(multiplePointsRegion)) );
		// Test with region that contains all points
		EXPECT_TRUE( vectorsContainSameElements(testPoints, structure->pointsInRegion(allPointsRegion)) );
	}

} }