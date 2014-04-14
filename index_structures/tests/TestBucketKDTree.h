#ifndef MDSEARCH_TEST_BUCKETKDTREE_H
#define MDSEARCH_TEST_BUCKETKDTREE_H

#include <gtest/gtest.h>
#include "Common.h"
#include "BucketKDTree.h"

namespace mdsearch { namespace tests
{

	static const unsigned int NUM_BUCKETKDTREE_DIMENSIONS = 3;

	class BucketBucketKDTreeTests : public ::testing::Test
	{

	};

	TEST_F(BucketBucketKDTreeTests, Construction)
	{
		BucketKDTree structure(NUM_BUCKETKDTREE_DIMENSIONS);
		ASSERT_EQ(NUM_PYRAMIDTREE_DIMENSIONS, structure.dimensionality());
	}

	TEST_F(BucketBucketKDTreeTests, Clear)
	{
		BucketKDTree structure(NUM_BUCKETKDTREE_DIMENSIONS);
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

	TEST_F(BucketBucketKDTreeTests, InsertionAndRemoval)
	{
		BucketKDTree structure(NUM_BUCKETKDTREE_DIMENSIONS);
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

	TEST_F(BucketBucketKDTreeTests, Updating)
	{
		BucketKDTree structure(IndexStructureTester::NUM_TEST_DIMENSIONS);
		IndexStructureTester tester;
		tester.testUpdates(&structure);
	}

	TEST_F(BucketBucketKDTreeTests, PointQueries)
	{
		BucketKDTree structure(IndexStructureTester::NUM_TEST_DIMENSIONS);
		IndexStructureTester tester;
		tester.testPointQueries(&structure);
	}

} }

#endif
