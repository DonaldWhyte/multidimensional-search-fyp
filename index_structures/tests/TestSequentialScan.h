#ifndef MDSEARCH_TEST_SEQUENTIALSCAN_H
#define MDSEARCH_TEST_SEQUENTIALSCAN_H

#include <gtest/gtest.h>
#include "Common.h"
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
		SequentialScan structure(IndexStructureTester::NUM_TEST_DIMENSIONS);
		IndexStructureTester tester;
		tester.testUpdates(&structure);
	}

	TEST_F(SequentialScanTests, PointQueries)
	{
		SequentialScan structure(IndexStructureTester::NUM_TEST_DIMENSIONS);
		IndexStructureTester tester;
		tester.testPointQueries(&structure);
	}

	TEST_F(SequentialScanTests, RegionQueries)
	{
		SequentialScan structure(IndexStructureTester::NUM_TEST_DIMENSIONS);
		IndexStructureTester tester;
		tester.testRegionQueries(&structure);
	}


} }

#endif