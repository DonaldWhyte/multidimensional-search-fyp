#ifndef MDSEARCH_TEST_SPLAYQUADTREE_H
#define MDSEARCH_TEST_SPLAYQUADTREE_H

#include <gtest/gtest.h>
#include "SplayQuadtree.h"

namespace mdsearch { namespace tests {

	static const unsigned int NUM_SPLAYQUADTREE_DIMENSIONS = 3;

	class SplayQuadtreeTests : public ::testing::Test
	{

	protected:
		virtual void SetUp()
		{
			Interval initialBoundaryIntervals[3] = { Interval(0, 10), Interval(0, 10), Interval(0, 10) };
			initialBoundary = Region(3, initialBoundaryIntervals);
		}

		Region initialBoundary;

	};

	TEST_F(SplayQuadtreeTests, Construction)
	{
		SplayQuadtree structure(NUM_SPLAYQUADTREE_DIMENSIONS, initialBoundary);
		EXPECT_EQ(SplayQuadtree::EMPTY_NODE, structure.rootNode()->type);
		EXPECT_EQ(initialBoundary, structure.rootNode()->outerBox);
		// Test clearing empty structure
		structure.clear();
		EXPECT_EQ(SplayQuadtree::EMPTY_NODE, structure.rootNode()->type);
		EXPECT_EQ(initialBoundary, structure.rootNode()->outerBox);
		// Testing clearing filled structure
		ASSERT_TRUE( structure.insert(Point(NUM_SPLAYQUADTREE_DIMENSIONS, 1)) );
		EXPECT_FALSE( structure.rootNode()->type == SplayQuadtree::EMPTY_NODE );
		structure.clear();
		EXPECT_EQ(SplayQuadtree::EMPTY_NODE, structure.rootNode()->type);
		EXPECT_EQ(initialBoundary, structure.rootNode()->outerBox);			
	}

} }

#endif