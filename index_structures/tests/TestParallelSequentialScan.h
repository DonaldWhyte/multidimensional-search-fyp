#ifndef MDSEARCH_TEST_PARALLELSEQUENTIALSCAN_H
#define MDSEARCH_TEST_PARALLELSEQUENTIALSCAN_H

#include <gtest/gtest.h>
#include "Common.h"
#include "ParallelSequentialScan.h"

namespace mdsearch { namespace tests
{

	class ParallelSequentialScanTests : public ::testing::Test
	{

	protected:
		static const unsigned int NUM_DIMENSIONS = 3;
		static const unsigned int NUM_THREADS = 1;

	};

	TEST_F(ParallelSequentialScanTests, Construction)
	{
		ParallelSequentialScan structure(NUM_DIMENSIONS, NUM_THREADS);
		IndexStructureTester tester;
		const PointList& testPoints = tester.getTestPoints();
				
		ASSERT_EQ(0, structure.allPoints().size());

		structure.loadPoints(testPoints);
		ASSERT_EQ(testPoints, structure.allPoints());
		// Testing no duplicates are added
		structure.loadPoints(testPoints);
		ASSERT_EQ(testPoints, structure.allPoints());
	}

	TEST_F(ParallelSequentialScanTests, Clear)
	{
		ParallelSequentialScan structure(NUM_DIMENSIONS, NUM_THREADS);
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

	TEST_F(ParallelSequentialScanTests, Insertion)
	{
		ParallelSequentialScan structure(NUM_DIMENSIONS, NUM_THREADS);
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

	TEST_F(ParallelSequentialScanTests, Removal)
	{
		ParallelSequentialScan structure(NUM_DIMENSIONS, NUM_THREADS);
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
		// Point deletition where point exists
		EXPECT_TRUE(structure.remove(testPoints[1]));
		expectedPoints.erase(expectedPoints.begin() + 1);
		EXPECT_EQ(expectedPoints, structure.allPoints()); // ensure point has been removed!
	}

	TEST_F(ParallelSequentialScanTests, Updating)
	{
		ParallelSequentialScan structure(
			IndexStructureTester::NUM_TEST_DIMENSIONS,
			NUM_THREADS);
		IndexStructureTester tester;
		tester.testUpdates(&structure);
	}

	TEST_F(ParallelSequentialScanTests, PointQueries)
	{
		ParallelSequentialScan structure(
			IndexStructureTester::NUM_TEST_DIMENSIONS,
			NUM_THREADS);
		IndexStructureTester tester;
		tester.testPointQueries(&structure);
	}

} }

#endif