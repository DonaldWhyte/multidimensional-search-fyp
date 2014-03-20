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
		EXPECT_EQ(SplayQuadtree::EMPTY_LEAF_NODE, structure.rootNode()->type);
		EXPECT_EQ(initialBoundary, structure.rootNode()->outerBox);
		// Test clearing empty structure
		structure.clear();
		EXPECT_EQ(SplayQuadtree::EMPTY_LEAF_NODE, structure.rootNode()->type);
		EXPECT_EQ(initialBoundary, structure.rootNode()->outerBox);
		// Testing clearing filled structure
		ASSERT_TRUE( structure.insert(Point(NUM_SPLAYQUADTREE_DIMENSIONS, 1)) );
		EXPECT_FALSE( structure.rootNode()->type == SplayQuadtree::EMPTY_LEAF_NODE );
		structure.clear();
		EXPECT_EQ(SplayQuadtree::EMPTY_LEAF_NODE, structure.rootNode()->type);
		EXPECT_EQ(initialBoundary, structure.rootNode()->outerBox);			
	}

	TEST_F(SplayQuadtreeTests, Insertion)
	{
		ASSERT_FALSE(true);
		/*
		IndexStructureTester tester;
		const PointList& testPoints = tester.getTestPoints();

		SplayQuadtree::Node* root = NULL;
		SplayQuadtree::LeafNode* leaf = NULL;
		SplayQuadtree::ShrinkSplitNode* shrinkSplit = NULL;

		SplayQuadtree structure(NUM_SPLAYQUADTREE_DIMENSIONS, initialBoundary);
		ASSERT_EQ(SplayQuadtree::EMPTY_LEAF_NODE, structure.rootNode()->type);
		ASSERT_EQ(initialBoundary, structure.rootNode()->outerBox);
		// Test inserting point into empty structure
		ASSERT_TRUE(structure.insert(testPoints[0]));
		root  = structure.rootNode();
		ASSERT_EQ(SplayQuadtree::FILLED_LEAF_NODE, root->type);
		leaf = dynamic_cast<SplayQuadtree::LeafNode*>(root);
		ASSERT_FALSE(leaf->containsInnerBox);
		EXPECT_EQ(testPoints[0], leaf->point);		
		// Test inserting point into single leaf node		
		ASSERT_TRUE(structure.insert(testPoints[1]));
		// Test inserting point where it is contained in left child of root
		// TODO
		// Test inserting point where it is contained in outer child of root
		// TODO
		// Test inserting point out of whole structure's boundaries
		Point outsidePoint(NUM_SPLAYQUADTREE_DIMENSIONS, 10000);
		ASSERT_FALSE(structure.insert(outsidePoint));*/
	}

} }

#endif