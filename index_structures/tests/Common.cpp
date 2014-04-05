#include "Common.h"
#include <gtest/gtest.h>

namespace mdsearch { namespace tests
{

	IndexStructureTester::IndexStructureTester()
	{
		// Construct test dataset
		Real testPointValues[NUM_TEST_POINTS][NUM_TEST_DIMENSIONS] = {
			{9, 2, 0}, {3, 6, 6}, {4, 0, 3}, {6, 5, 8}, {6, 7, 6},
			{1, 3, 0}, {6, 9, 0}, {2, 4, 7}, {4, 5, 9}, {5, 6, 0}
		};
		testPoints.reserve(NUM_TEST_POINTS);
		for (unsigned int i = 0; (i < NUM_TEST_POINTS); i++)
			testPoints.push_back( Point(NUM_TEST_DIMENSIONS, testPointValues[i]) );
	}

	const PointList& IndexStructureTester::getTestPoints() const
	{
		return testPoints;
	}

	void IndexStructureTester::testUpdates(IndexStructure* structure)
	{
		structure->loadPoints(testPoints);

		// Test with point not stored in structure
		Point doesNotExist(NUM_TEST_DIMENSIONS, -10);
		ASSERT_FALSE(structure->pointExists(doesNotExist));
		EXPECT_FALSE(structure->update(doesNotExist, testPoints[0]));
		// Test with point stored in structure, but where the new
		// point IS ALREADY IN THE STRUCTURE
		// (should delete point but not add a new one)
		ASSERT_TRUE(structure->pointExists(testPoints[0]));
		ASSERT_TRUE(structure->pointExists(testPoints[1]));
		EXPECT_TRUE(structure->update(testPoints[0], testPoints[1]));
		EXPECT_FALSE(structure->pointExists(testPoints[0]));
		// Test with point stored in structure where new point is
		// NOT stored in structure
		Point newPoint(NUM_TEST_DIMENSIONS, 5.5);
		ASSERT_TRUE(structure->pointExists(testPoints[1]));
		ASSERT_FALSE(structure->pointExists(newPoint));
		EXPECT_TRUE(structure->update(testPoints[1], newPoint));
		EXPECT_FALSE(structure->pointExists(testPoints[1]));
		EXPECT_TRUE(structure->pointExists(newPoint));		
	}

	void IndexStructureTester::testPointQueries(IndexStructure* structure)
	{
		structure->loadPoints(testPoints);

		// Test points that don't exist in structure
		EXPECT_FALSE( structure->pointExists(Point(structure->dimensionality(), 0.0f)) );
		EXPECT_FALSE( structure->pointExists(Point(structure->dimensionality(), 0.0f)) );
		EXPECT_FALSE( structure->pointExists(Point(structure->dimensionality(), 5.6f)) );
		// Test points that do exist
		for (unsigned int i = 0; (i < testPoints.size()); i++)
		{
			EXPECT_TRUE(structure->pointExists(testPoints[i]));
		}
	}

} }