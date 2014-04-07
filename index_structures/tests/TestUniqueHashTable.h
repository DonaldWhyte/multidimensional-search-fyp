#ifndef MDSEARCH_TEST_UNIQUEHASHTABLE_H
#define MDSEARCH_TEST_UNIQUEHASHTABLE_H

#include <gtest/gtest.h>
#include "UniqueHashTable.h"

namespace mdsearch { namespace tests {

	static const unsigned int NUM_UHT_DIMENSIONS = 3;

	class UniqueHashTableTests : public ::testing::Test
	{

	};

	TEST_F(UniqueHashTableTests, Construction)
	{
		UniqueHashTable structure(NUM_UHT_DIMENSIONS);
		ASSERT_EQ(NUM_UHT_DIMENSIONS, structure.dimensionality());
	}

	TEST_F(UniqueHashTableTests, Clear)
	{
		UniqueHashTable structure(NUM_UHT_DIMENSIONS);
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

	TEST_F(UniqueHashTableTests, InsertionAndRemoval)
	{
		UniqueHashTable structure(NUM_UHT_DIMENSIONS);
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

	TEST_F(UniqueHashTableTests, Updating)
	{
		UniqueHashTable structure(IndexStructureTester::NUM_TEST_DIMENSIONS);
		IndexStructureTester tester;
		tester.testUpdates(&structure);
	}

	TEST_F(UniqueHashTableTests, PointQueries)
	{
		UniqueHashTable structure(IndexStructureTester::NUM_TEST_DIMENSIONS);
		IndexStructureTester tester;
		tester.testPointQueries(&structure);
	}


} }

#endif