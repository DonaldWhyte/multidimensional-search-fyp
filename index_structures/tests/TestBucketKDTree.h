#ifndef MDSEARCH_TEST_BUCKETKDTREE_H
#define MDSEARCH_TEST_BUCKETKDTREE_H

#include <gtest/gtest.h>
#include "Common.h"
#include "BucketKDTree.h"

namespace mdsearch { namespace tests
{

	static const unsigned int NUM_BUCKETKDTREE_DIMENSIONS = 3;

	class BucketKDTreeTests : public ::testing::Test
	{

	protected:
		static const int MAX_POINTS_UNTIL_SPLIT = 4;
		static const int MIN_POINTS_UNTIL_MERGE = 4;

	};

	TEST_F(BucketKDTreeTests, Construction)
	{
		BucketKDTree structure(NUM_BUCKETKDTREE_DIMENSIONS,
			MAX_POINTS_UNTIL_SPLIT, MIN_POINTS_UNTIL_MERGE);
		ASSERT_EQ(NUM_PYRAMIDTREE_DIMENSIONS, structure.dimensionality());
	}

	TEST_F(BucketKDTreeTests, Clear)
	{
		BucketKDTree structure(NUM_BUCKETKDTREE_DIMENSIONS,
			MAX_POINTS_UNTIL_SPLIT, MIN_POINTS_UNTIL_MERGE);
		IndexStructureTester tester;
		const PointList& testPoints = tester.getTestPoints();
		structure.loadPoints(testPoints);
		// Ensure all points loaded exist
		for (unsigned int i = 0; (i < testPoints.size()); i++)
			ASSERT_TRUE(structure.pointExists(testPoints[i]));
		// Clear the entire structure
		structure.clear();
		// Ensure all points are no longer stored in structure
		for (unsigned int i = 0; (i < testPoints.size()); i++)
			ASSERT_FALSE(structure.pointExists(testPoints[i]));
	}

	TEST_F(BucketKDTreeTests, InsertionAndRemoval)
	{
		BucketKDTree structure(NUM_BUCKETKDTREE_DIMENSIONS,
			MAX_POINTS_UNTIL_SPLIT, MIN_POINTS_UNTIL_MERGE);
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

	TEST_F(BucketKDTreeTests, Updating)
	{
		BucketKDTree structure(IndexStructureTester::NUM_TEST_DIMENSIONS,
			MAX_POINTS_UNTIL_SPLIT, MIN_POINTS_UNTIL_MERGE);
		IndexStructureTester tester;
		tester.testUpdates(&structure);
	}

	TEST_F(BucketKDTreeTests, PointQueries)
	{
		BucketKDTree structure(IndexStructureTester::NUM_TEST_DIMENSIONS,
			MAX_POINTS_UNTIL_SPLIT, MIN_POINTS_UNTIL_MERGE);
		IndexStructureTester tester;
		tester.testPointQueries(&structure);
	}

} }

#endif
