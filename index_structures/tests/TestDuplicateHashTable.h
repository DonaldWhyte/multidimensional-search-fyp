#ifndef MDSEARCH_TEST_DUPLICATEHASHTABLE_H
#define MDSEARCH_TEST_DUPLICATEHASHTABLE_H

#include <gtest/gtest.h>
#include "DuplicateHashTable.h"

namespace mdsearch { namespace tests {

	static const unsigned int NUM_DHT_DIMENSIONS = 3;

	class DuplicateHashTableTests : public ::testing::Test
	{

	};

	TEST_F(DuplicateHashTableTests, Construction)
	{
		DuplicateHashTable structure(NUM_DHT_DIMENSIONS);
		ASSERT_EQ(NUM_DHT_DIMENSIONS, structure.dimensionality());
	}

	TEST_F(DuplicateHashTableTests, Clear)
	{
		DuplicateHashTable structure(NUM_DHT_DIMENSIONS);
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

	TEST_F(DuplicateHashTableTests, InsertionAndRemoval)
	{
		DuplicateHashTable structure(NUM_DHT_DIMENSIONS);
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

	TEST_F(DuplicateHashTableTests, Updating)
	{
		DuplicateHashTable structure(IndexStructureTester::NUM_TEST_DIMENSIONS);
		IndexStructureTester tester;
		tester.testUpdates(&structure);
	}

	TEST_F(DuplicateHashTableTests, PointQueries)
	{
		DuplicateHashTable structure(IndexStructureTester::NUM_TEST_DIMENSIONS);
		IndexStructureTester tester;
		tester.testPointQueries(&structure);
	}


} }

#endif