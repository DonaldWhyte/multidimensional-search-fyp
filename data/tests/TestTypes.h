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

		// Interval intersection
		EXPECT_TRUE(Interval(2, 2).intersects(Interval(2, 2)));
		EXPECT_TRUE(Interval(2, 2).intersects(Interval(2, 3)));
		EXPECT_TRUE(Interval(2, 3).intersects(Interval(2, 2)));
		EXPECT_TRUE(Interval(2, 3).intersects(Interval(1, 4)));
		EXPECT_TRUE(Interval(1, 4).intersects(Interval(2, 3)));
		EXPECT_FALSE(Interval(1, 1).intersects(Interval(2, 2)));
		EXPECT_FALSE(Interval(2, 8).intersects(Interval(17, 24)));
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
		Interval twoCIntervals[] = { Interval(60, 70), Interval(80, 90) };
		Region twoC(2, twoCIntervals);
		Interval twoDIntervals[] = { Interval(40, 100), Interval(85, 87) };
		Region twoD(2, twoDIntervals);
		EXPECT_EQ(twoA[0], Interval(0, 0)); EXPECT_EQ(twoA[1], Interval(0, 0));
		EXPECT_EQ(twoB[0], Interval(10, 20)); EXPECT_EQ(twoB[1], Interval(10, 20));
		EXPECT_EQ(twoC[0], Interval(60, 70)); EXPECT_EQ(twoC[1], Interval(80, 90));
		EXPECT_EQ(twoD[0], Interval(40, 100)); EXPECT_EQ(twoD[1], Interval(85, 87));
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

		// Region intersection
		EXPECT_TRUE(twoB.intersects(twoB));
		EXPECT_FALSE(twoB.intersects(twoC));
		EXPECT_TRUE(twoC.intersects(twoD));
		EXPECT_FALSE(twoD.intersects(twoB));

		// Finding longest dimension of region
		Interval longestTestRegionIntervals[] = {
			Interval(0, 10), Interval(0, 10), Interval(2, 4), 
			Interval(30, 41), Interval(0.535, 0.8)
		};
		Region longestTestRegion(5, longestTestRegionIntervals);
		EXPECT_EQ(-1, Region(0).longestDimension());
		EXPECT_EQ(0, oneA.longestDimension());
		EXPECT_EQ(0, oneB.longestDimension());
		EXPECT_EQ(0, twoA.longestDimension());
		EXPECT_EQ(0, twoB.longestDimension());
		EXPECT_EQ(0, twoC.longestDimension());
		EXPECT_EQ(0, twoD.longestDimension());
		EXPECT_EQ(3, longestTestRegion.longestDimension());
	}	

	TEST_F(RegionTests, Subdivison)
	{
		// Construct test regions
		Region zeroSizeOneDR = Region(1, Interval(1, 1));
		Region zeroSizeTwoDR = Region(2, Interval(1, 1));
		Region zeroSizeThreeDR = Region(3, Interval(1, 1));
		Region oneDR = Region(1, Interval(1, 3));
		Region twoDR = Region(2, Interval(1, 3));
		Region threeDR = Region(3, Interval(1, 3));
		Region fourDR = Region(4, Interval(1, 3));
		Interval nonUniformRegionIntervals[2] = { Interval(1, 11), Interval(0.5, 1) };
		Region nonUniformRegion = Region(2, nonUniformRegionIntervals);
		// Construct expected subregions for each region
		Interval twoDSubRegionsIntervals[][2] = {
			{ Interval(1, 2), Interval(1, 2) },
			{ Interval(2, 3), Interval(1, 2) },
			{ Interval(1, 2), Interval(2, 3) },
			{ Interval(2, 3), Interval(2, 3) }
		};
		Interval threeDSubRegionsIntervals[][3] = {
			{ Interval(1, 2), Interval(1, 2), Interval(1, 2) },
			{ Interval(2, 3), Interval(1, 2), Interval(1, 2) },
			{ Interval(1, 2), Interval(2, 3), Interval(1, 2) },
			{ Interval(2, 3), Interval(2, 3), Interval(1, 2) },
			{ Interval(1, 2), Interval(1, 2), Interval(2, 3) },
			{ Interval(2, 3), Interval(1, 2), Interval(2, 3) },
			{ Interval(1, 2), Interval(2, 3), Interval(2, 3) },
			{ Interval(2, 3), Interval(2, 3), Interval(2, 3) }
		};
		Interval fourDSubRegionsIntervals[][4] = {
			{ Interval(1, 2), Interval(1, 2), Interval(1, 2), Interval(1, 2) },
			{ Interval(2, 3), Interval(1, 2), Interval(1, 2), Interval(1, 2) },
			{ Interval(2, 3), Interval(2, 3), Interval(1, 2), Interval(1, 2) },
			{ Interval(2, 3), Interval(2, 3), Interval(2, 3), Interval(2, 3) },
			{ Interval(1, 2), Interval(2, 3), Interval(1, 2), Interval(1, 2) },
			{ Interval(2, 3), Interval(2, 3), Interval(2, 3), Interval(1, 2) },
			{ Interval(1, 2), Interval(2, 3), Interval(2, 3), Interval(2, 3) },
			{ Interval(1, 2), Interval(1, 2), Interval(2, 3), Interval(1, 2) },
			{ Interval(1, 2), Interval(1, 2), Interval(2, 3), Interval(2, 3) },
			{ Interval(2, 3), Interval(1, 2), Interval(2, 3), Interval(1, 2) },
			{ Interval(2, 3), Interval(1, 2), Interval(2, 3), Interval(2, 3) },
			{ Interval(2, 3), Interval(2, 3), Interval(1, 2), Interval(2, 3) },
			{ Interval(1, 2), Interval(2, 3), Interval(1, 2), Interval(2, 3) },
			{ Interval(1, 2), Interval(2, 3), Interval(2, 3), Interval(1, 2) },
			{ Interval(2, 3), Interval(1, 2), Interval(1, 2), Interval(2, 3) },
			{ Interval(1, 2), Interval(1, 2), Interval(1, 2), Interval(2, 3) }
		};
		Interval nonUniformSubRegionsIntervals[][2] = {
			{ Interval(1, 6), Interval(0.5, 0.75) },
			{ Interval(6, 11), Interval(0.5, 0.75) },
			{ Interval(1, 6), Interval(0.75, 1) },
			{ Interval(6, 11), Interval(0.75, 1) }
		};

		RegionList zeroSizeOneDSubRegions;
		for (unsigned int i = 0; (i < 2); i++)
			zeroSizeOneDSubRegions.push_back( Region(1, Interval(1, 1)) );
		RegionList zeroSizeTwoDSubRegions;
		for (unsigned int i = 0; (i < 4); i++)
			zeroSizeTwoDSubRegions.push_back( Region(2, Interval(1, 1)) );
		RegionList zeroSizeThreeDSubRegions;
		for (unsigned int i = 0; (i < 8); i++)
			zeroSizeThreeDSubRegions.push_back( Region(3, Interval(1, 1)) );
		RegionList oneDSubRegions;
		oneDSubRegions.push_back( Region(1, Interval(1, 2)) );
		oneDSubRegions.push_back( Region(1, Interval(2, 3)) );
		RegionList twoDSubRegions;
		for (unsigned int i = 0; (i < 4); i++)
			twoDSubRegions.push_back( Region(2, twoDSubRegionsIntervals[i]) );
		RegionList threeDSubRegions;
		for (unsigned int i = 0; (i < 8); i++)
			threeDSubRegions.push_back( Region(3, threeDSubRegionsIntervals[i]) );
		RegionList fourDSubRegions;
		for (unsigned int i = 0; (i < 16); i++)
			fourDSubRegions.push_back( Region(4, fourDSubRegionsIntervals[i]) );
		RegionList nonUniformSubRegions;
		for (unsigned int i = 0; (i < 4); i++)
			nonUniformSubRegions.push_back( Region(2, nonUniformSubRegionsIntervals[i]) );


		// Test with one-dimensional region of zero size
		EXPECT_EQ(zeroSizeOneDSubRegions, zeroSizeOneDR.subdivide());
		// Test with one-dimensional region
		EXPECT_EQ(oneDSubRegions, oneDR.subdivide());
		// Test with two-dimensional region of zero size
		EXPECT_EQ(zeroSizeTwoDSubRegions, zeroSizeTwoDR.subdivide());
		// Test with two-dimensional region
		EXPECT_EQ(twoDSubRegions, twoDR.subdivide());
		// Test with three-dimensional region of zero size
		EXPECT_EQ(zeroSizeThreeDSubRegions, zeroSizeThreeDR.subdivide());
		// Test with three-dimensional region
		EXPECT_EQ(threeDSubRegions, threeDR.subdivide());
		// Test four-dimension region
		RegionList actualSubRegions = fourDR.subdivide();
		// Manually check for existing of actual region in EXPECTED regions
		// This way, the order of the expected region list does not matter
		for (RegionList::const_iterator actualRegion = actualSubRegions.begin();
			(actualRegion != actualSubRegions.end()); actualRegion++)
		{
			bool exists = false;
			for (RegionList::const_iterator expectedRegion = fourDSubRegions.begin();
				(expectedRegion != fourDSubRegions.end()); expectedRegion++)			
			{
				if ((*expectedRegion) == (*actualRegion))
				{
					exists = true;
					break;
				}
			}
			ASSERT_TRUE(exists);
		}

		// Test with non-uniform region
		EXPECT_EQ(nonUniformSubRegions, nonUniformRegion.subdivide());
	}

	TEST_F(RegionTests, MinimumBoundingBox)
	{
		Real pointValues[][2] = { { 2, 2 }, { 8, -4 } };
		Point points[] = { Point(2, pointValues[0]), Point(2, pointValues[1]) };
		Interval regionIntervals[][2] = {
			{ Interval(2, 2), Interval(2, 2) },
			{ Interval(1, 3), Interval(1, 3) },
			{ Interval(4, 5), Interval(4, 6) },
			{ Interval(-10, -5), Interval(-15, -2.5) }
		};
		Region regions[] = {
			Region(2, regionIntervals[0]),
			Region(2, regionIntervals[1]),
			Region(2, regionIntervals[2]),
			Region(2, regionIntervals[3])
		};
		Interval expectedRegionIntervals[][2] = {
			{ Interval(2, 8), Interval(-4, 2) },
			{ Interval(2, 5), Interval(2, 6) },
			{ Interval(-10, 2), Interval(-15, 2) }
		};
		Region expectedRegions[] = {
			Region(2, expectedRegionIntervals[0]),
			Region(2, expectedRegionIntervals[1]),
			Region(2, expectedRegionIntervals[2])
		};

		// Test with zero-sized region and point in same place
		Region minimum = Region::minimumBoundingBox(regions[0], points[0]);
		EXPECT_EQ(regions[0], minimum);
		// Test with zero-sized region and point
		minimum = Region::minimumBoundingBox(regions[0], points[1]);
		EXPECT_EQ(expectedRegions[0], minimum);
		// Test with point in region
		minimum = Region::minimumBoundingBox(regions[1], points[0]);
		EXPECT_EQ(regions[1], minimum);
		// Test with disjoint region and point
		minimum = Region::minimumBoundingBox(regions[2], points[0]);
		EXPECT_EQ(expectedRegions[1], minimum);
		minimum = Region::minimumBoundingBox(regions[3], points[0]);
		EXPECT_EQ(expectedRegions[2], minimum);

		// Test with two points in same place
		minimum = Region::minimumBoundingBox(points[0], points[0]);
		EXPECT_EQ(regions[0], minimum);
		// Test with two points in different place
		minimum = Region::minimumBoundingBox(points[0], points[1]);
		EXPECT_EQ(expectedRegions[0], minimum);
	}

	TEST_F(RegionTests, Split)
	{
		Interval expectedRegionIntervals[][2] = {
			{ Interval(1, 10), Interval(1, 1) },
			{ Interval(1, 10), Interval(1, 10) },

			{ Interval(1, 10), Interval(1, 10) },
			{ Interval(1, 10), Interval(10, 10) },

			{ Interval(1, 10), Interval(1, 5.5) },
			{ Interval(1, 10), Interval(5.5, 10) },

			{ Interval(1, 10), Interval(1, 9) },
			{ Interval(1, 10), Interval(9, 10) }
		};
		Region expectedRegions[] = {
			Region(2, expectedRegionIntervals[0]),
			Region(2, expectedRegionIntervals[1]),
			Region(2, expectedRegionIntervals[2]),
			Region(2, expectedRegionIntervals[3]),
			Region(2, expectedRegionIntervals[4]),
			Region(2, expectedRegionIntervals[5]),
			Region(2, expectedRegionIntervals[6]),
			Region(2, expectedRegionIntervals[7])
		};

		Region originalRegion = Region(2, Interval(1, 10));
		Region lowRegion, highRegion;
		// Test splitting with pivot values outside region
		EXPECT_FALSE(originalRegion.split(1, 0.9, lowRegion, highRegion));
		EXPECT_FALSE(originalRegion.split(1, 10.1, lowRegion, highRegion));
		// Test splitting on lowest value
		EXPECT_TRUE(originalRegion.split(1, 1, lowRegion, highRegion));
		EXPECT_EQ(expectedRegions[0], lowRegion);
		EXPECT_EQ(expectedRegions[1], highRegion);
		// Test splitting on highest value
		EXPECT_TRUE(originalRegion.split(1, 10, lowRegion, highRegion));
		EXPECT_EQ(expectedRegions[2], lowRegion);
		EXPECT_EQ(expectedRegions[3], highRegion);
		// Test splitting on middle value
		EXPECT_TRUE(originalRegion.split(1, 5.5, lowRegion, highRegion));
		EXPECT_EQ(expectedRegions[4], lowRegion);
		EXPECT_EQ(expectedRegions[5], highRegion);
		// Test splitting on another value
		EXPECT_TRUE(originalRegion.split(1, 9, lowRegion, highRegion));
		EXPECT_EQ(expectedRegions[6], lowRegion);
		EXPECT_EQ(expectedRegions[7], highRegion);
	}

	TEST_F(RegionTests, ContainsRegion)
	{
		Interval testRegionIntervals[][2] = {
			{ Interval(1, 1), Interval(1, 1) },
			{ Interval(2, 2), Interval(2, 2) },
			{ Interval(1, 10), Interval(1, 10) },
			{ Interval(2, 6), Interval(2, 6) },
			{ Interval(4, 7), Interval(4, 7) },
			{ Interval(7, 8), Interval(7, 8) }
		};
		RegionList testRegions;
		for (unsigned int i = 0; (i < 6); i++)
			testRegions.push_back(Region(2, testRegionIntervals[i]));

		// Test two zero-sized regions in same place
		EXPECT_TRUE(testRegions[0].contains(testRegions[0]));
		// Test two different zero-sized regions
		EXPECT_FALSE(testRegions[0].contains(testRegions[1]));
		// Test identical regions
		EXPECT_TRUE(testRegions[2].contains(testRegions[2]));
		// Test one region nested in the other
		EXPECT_TRUE(testRegions[2].contains(testRegions[3]));
		// Test region TOO BIG to be in other region, but intersects with all of it (should fail)
		EXPECT_FALSE(testRegions[3].contains(testRegions[2]));
		// Test overlapping regions
		EXPECT_FALSE(testRegions[3].contains(testRegions[4]));
		// Test disjoint regions
		EXPECT_FALSE(testRegions[3].contains(testRegions[5]));
	}

	TEST_F(RegionTests, LongestDimension)
	{
		Region oneD(1, Interval(4, 8));
		Region twoDUniform(2, Interval(4, 8));
		Interval intervals[2] = { Interval(1, 5), Interval(-4, 7) };
		Region twoD(2, intervals);

		// Test 1D region
		EXPECT_EQ(0, oneD.longestDimension());
		EXPECT_EQ(4, oneD.longestLength());
		// Test 2D uniform region
		EXPECT_EQ(0, twoDUniform.longestDimension());
		EXPECT_EQ(4, twoDUniform.longestLength());
		// Test 2D non-uniform region
		EXPECT_EQ(1, twoD.longestDimension());
		EXPECT_EQ(11, twoD.longestLength());
	}



} }

#endif