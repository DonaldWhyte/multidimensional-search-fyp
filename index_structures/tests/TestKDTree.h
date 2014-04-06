#ifndef MDSEARCH_TEST_KDTREE_H
#define MDSEARCH_TEST_KDTREE_H

#include <gtest/gtest.h>
#include "Common.h"
#include "KDTree.h"

namespace mdsearch { namespace tests
{

	static const unsigned int NUM_KDTREE_DIMENSIONS = 3;

	class KDTreeTests : public ::testing::Test
	{

	};

	TEST_F(KDTreeTests, Construction)
	{
		KDTree structure(NUM_KDTREE_DIMENSIONS);
		ASSERT_EQ(NUM_PYRAMIDTREE_DIMENSIONS, structure.dimensionality());
	}

	TEST_F(KDTreeTests, Clear)
	{
		KDTree structure(NUM_KDTREE_DIMENSIONS);
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

	TEST_F(KDTreeTests, InsertionAndRemoval)
	{
		KDTree structure(NUM_KDTREE_DIMENSIONS);
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

	TEST_F(KDTreeTests, Updating)
	{
		KDTree structure(IndexStructureTester::NUM_TEST_DIMENSIONS);
		IndexStructureTester tester;
		tester.testUpdates(&structure);
	}

	TEST_F(KDTreeTests, PointQueries)
	{
		KDTree structure(IndexStructureTester::NUM_TEST_DIMENSIONS);
		IndexStructureTester tester;
		tester.testPointQueries(&structure);
	}

} }

#endif
