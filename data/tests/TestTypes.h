#ifndef MDSEARCH_TEST_DATATYPES_H
#define MDSEARCH_TEST_DATATYPES_H

#include <gtest/gtest.h>
#include "Point.h"
#include "Region.h"

namespace mdsearch { namespace tests
{

	class DataTypeTests : public ::testing::Test
	{

	protected:
		DataTypeTests() { }
		virtual ~DataTypeTests() { }

	};

	TEST_F(DataTypeTests, Point)
	{
		// Test with one and two dimensions
		Point<1> oneA;
		Point<1> oneB(4);
		Real value = 5;
		Point<1> oneC(&value);
		EXPECT_EQ(oneA[0], 0);
		EXPECT_EQ(oneB[0], 4);
		EXPECT_EQ(oneC[0], 5);
		Point<2> twoA;
		Point<2> twoB(3);
		Real values[] = { 2, 4 };
		Point<2> twoC(values);
		EXPECT_EQ(twoA[0], 0); EXPECT_EQ(twoA[1], 0);
		EXPECT_EQ(twoB[0], 3); EXPECT_EQ(twoB[1], 3);
		EXPECT_EQ(twoC[0], 2); EXPECT_EQ(twoC[1], 4);
	}

	TEST_F(DataTypeTests, Interval)
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

	TEST_F(DataTypeTests, Region)
	{
		// Test with one and two dimensions
		Region<1> oneA;
		Region<1> oneB( Interval(10, 20) );
		Interval someInterval(30, 40);
		Region<1> oneC(&someInterval);
		EXPECT_EQ(oneA[0], Interval(0, 0));
		EXPECT_EQ(oneB[0], Interval(10, 20));
		EXPECT_EQ(oneC[0], Interval(30, 40));
		Region<2> twoA;
		Region<2> twoB( Interval(10, 20) );
		Interval values[] = { Interval(60, 70), Interval(80, 90) };
		Region<2> twoC(values);
		EXPECT_EQ(twoA[0], Interval(0, 0)); EXPECT_EQ(twoA[1], Interval(0, 0));
		EXPECT_EQ(twoB[0], Interval(10, 20)); EXPECT_EQ(twoB[1], Interval(10, 20));
		EXPECT_EQ(twoC[0], Interval(60, 70)); EXPECT_EQ(twoC[1], Interval(80, 90));

		// TODO: test point-inside-region
	}	

} }

#endif