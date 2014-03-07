#ifndef MDSEARCH_TEST_DATATYPES_H
#define MDSEARCH_TEST_DATATYPES_H

#include <gtest/gtest.h>
#include "Point.h"
#include "Region.h"

namespace mdsearch { namespace tests
{

	class PointTests : public ::testing::Test
	{


	};

	TEST_F(PointTests, Point)
	{
		// Test with one and two dimensions
		Point oneA(1);
		Point oneB(1, 4);
		Real someValue = 8;
		Point oneC(1, someValue);
		EXPECT_EQ(oneA[0], 0);
		EXPECT_EQ(oneB[0], 4);
		EXPECT_EQ(oneC[0], 8);
		Point twoA(2);
		Point twoB(2, 3);
		Real someValues[] = { 2, 4 };
		Point twoC(2, someValues);
		EXPECT_EQ(twoA[0], 0); EXPECT_EQ(twoA[1], 0);
		EXPECT_EQ(twoB[0], 3); EXPECT_EQ(twoB[1], 3);
		EXPECT_EQ(twoC[0], 2); EXPECT_EQ(twoC[1], 4);
	}

	class RegionTests : public ::testing::Test
	{

	};

	TEST_F(RegionTests, Interval)
	{
		Interval a;
		Interval b(3, 5);
		EXPECT_EQ(a.min, 0); EXPECT_EQ(a.max, 0);
		EXPECT_EQ(b.min, 3); EXPECT_EQ(b.max, 5);
		// Test equality operators
		EXPECT_FALSE(a == b);
		EXPECT_TRUE(b == b);
		EXPECT_TRUE(b == Interval(3, 5));
		EXPECT_TRUE(a != b);
		EXPECT_FALSE(b != b);
		EXPECT_FALSE(b != Interval(3, 5));
	}

	TEST_F(RegionTests, Region)
	{
		// Test with one and two dimensions
		Region oneA(1);
		Region oneB(1, Interval(10, 20));
		EXPECT_EQ(oneA[0], Interval(0, 0));
		EXPECT_EQ(oneB[0], Interval(10, 20));
		Region twoA(2);
		Region twoB(2, Interval(10, 20));
		Interval someIntervals[] = { Interval(60, 70), Interval(80, 90) };
		Region twoC(2, someIntervals);
		EXPECT_EQ(twoA[0], Interval(0, 0)); EXPECT_EQ(twoA[1], Interval(0, 0));
		EXPECT_EQ(twoB[0], Interval(10, 20)); EXPECT_EQ(twoB[1], Interval(10, 20));
		EXPECT_EQ(twoC[0], Interval(60, 70)); EXPECT_EQ(twoC[1], Interval(80, 90));
		// Test point-inside-region test
		Real testPointsValues[][2] = {
			{12, 12}, {19, 12}, {8, 12},
			{12, 8}, {8, 8}, {0, 0},
			{1, 1}, {-1, -1}
		};

		EXPECT_TRUE( twoB.contains(Point(2, testPointsValues[0])) );
		EXPECT_TRUE( twoB.contains(Point(2, testPointsValues[1])) );
		EXPECT_FALSE( twoB.contains(Point(2, testPointsValues[2])) );
		EXPECT_FALSE( twoB.contains(Point(2, testPointsValues[3])) );
		EXPECT_FALSE( twoB.contains(Point(2, testPointsValues[4])) );
		// Test with a region that has zero size
		EXPECT_TRUE( twoA.contains(Point(2, testPointsValues[5])) );
		EXPECT_FALSE( twoA.contains(Point(2, testPointsValues[6])) );
		EXPECT_FALSE( twoA.contains(Point(2, testPointsValues[7])) );
	}	

} }

#endif