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
			initialBoundary = Region(3, Interval(0, 100));
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
		static const unsigned int NUM_TEST_POINTS = 6;
		Real testPointValues[][3] = {
			{9, 2, 0}, {3, 6, 6},
			{20, 20, 20}, {7, 3, 2}, {4, 3, 2},
			{ 10000, 10000, 10000 }
		};
		PointList testPoints;
		for (unsigned int i = 0; (i < NUM_TEST_POINTS); i++)
			testPoints.push_back(Point(3, testPointValues[i]));

		static const unsigned int NUM_TEST_REGIONS = 2;
		Interval testRegionIntervals[][3] = {
			// First insertion
			{ Interval(3, 9), Interval(2, 6), Interval(0, 6) }, // inner box
			// Second insertion
			{ Interval(2, 9), Interval(2, 7), Interval(0, 6) }, // inner box
		};
		RegionList testRegions;
		for (unsigned int i = 0; (i < NUM_TEST_REGIONS); i++)
			testRegions.push_back(Region(3, testRegionIntervals[i]));


		SplayQuadtree::Node* root = NULL;
		SplayQuadtree::LeafNode* leaf = NULL;
		SplayQuadtree::ShrinkSplitNode* shrinkSplit = NULL;

		SplayQuadtree structure(NUM_SPLAYQUADTREE_DIMENSIONS, initialBoundary);
		ASSERT_EQ(SplayQuadtree::EMPTY_LEAF_NODE, structure.rootNode()->type);
		ASSERT_EQ(initialBoundary, structure.rootNode()->outerBox);

		// Test inserting point into empty structure
		ASSERT_TRUE(structure.insert(testPoints[0]));
		root = structure.rootNode();
		ASSERT_EQ(SplayQuadtree::FILLED_LEAF_NODE, root->type);
		leaf = dynamic_cast<SplayQuadtree::LeafNode*>(root);
		ASSERT_FALSE(leaf->containsInnerBox);
		EXPECT_EQ(testPoints[0], leaf->point);		

		// Test inserting point into single leaf node (w/ point)
		// (MAIN TEST OF THE SHRINK-SPLIT PROCEDURE)	
		ASSERT_TRUE(structure.insert(testPoints[1]));
		root  = structure.rootNode();
		ASSERT_EQ(SplayQuadtree::SHRINKSPLIT_NODE, root->type);
		shrinkSplit = dynamic_cast<SplayQuadtree::ShrinkSplitNode*>(root);
		EXPECT_EQ(initialBoundary, shrinkSplit->outerBox);
		ASSERT_EQ(SplayQuadtree::FILLED_LEAF_NODE, shrinkSplit->leftChild->type);
		leaf = dynamic_cast<SplayQuadtree::LeafNode*>(shrinkSplit->leftChild);
		EXPECT_FALSE(leaf->containsInnerBox);
		EXPECT_EQ(testPoints[0], leaf->point); // existing contents should be in left child
		EXPECT_TRUE(leaf->outerBox.contains(testPoints[0]));
		EXPECT_FALSE(leaf->outerBox.contains(testPoints[1]));
		ASSERT_EQ(SplayQuadtree::FILLED_LEAF_NODE, shrinkSplit->rightChild->type);
		leaf = dynamic_cast<SplayQuadtree::LeafNode*>(shrinkSplit->rightChild);
		EXPECT_TRUE(leaf->outerBox.contains(leaf->point));
		EXPECT_FALSE(leaf->containsInnerBox);
		EXPECT_EQ(testPoints[1], leaf->point); //  new point should be in right child
		EXPECT_TRUE(leaf->outerBox.contains(testPoints[1]));
		EXPECT_FALSE(leaf->outerBox.contains(testPoints[0]));
		ASSERT_EQ(SplayQuadtree::FILLED_LEAF_NODE, shrinkSplit->outerChild->type);
		leaf = dynamic_cast<SplayQuadtree::LeafNode*>(shrinkSplit->outerChild);
		EXPECT_EQ(initialBoundary, leaf->outerBox);
		EXPECT_TRUE(leaf->containsInnerBox);
		EXPECT_TRUE(leaf->outerBox.contains(leaf->innerBox));
		EXPECT_TRUE(leaf->innerBox.contains(shrinkSplit->leftChild->outerBox));
		EXPECT_TRUE(leaf->innerBox.contains(shrinkSplit->rightChild->outerBox));

		// Test inserting point where it is contained in outer child (w/ inner box) of root
		ASSERT_TRUE(structure.insert(testPoints[2]));
		// Ensuring outer child is now a shrink-split node
		root = structure.rootNode();
		ASSERT_EQ(SplayQuadtree::SHRINKSPLIT_NODE, root->type);
		shrinkSplit = dynamic_cast<SplayQuadtree::ShrinkSplitNode*>(root);
		ASSERT_EQ(SplayQuadtree::SHRINKSPLIT_NODE, shrinkSplit->outerChild->type);
		shrinkSplit = dynamic_cast<SplayQuadtree::ShrinkSplitNode*>(shrinkSplit->outerChild);
		EXPECT_EQ(initialBoundary, shrinkSplit->outerBox);
		ASSERT_EQ(SplayQuadtree::FILLED_LEAF_NODE, shrinkSplit->leftChild->type);
		leaf = dynamic_cast<SplayQuadtree::LeafNode*>(shrinkSplit->leftChild);
		EXPECT_TRUE(leaf->containsInnerBox);
		EXPECT_TRUE(leaf->outerBox.contains(leaf->innerBox));
		ASSERT_EQ(SplayQuadtree::FILLED_LEAF_NODE, shrinkSplit->rightChild->type);
		leaf = dynamic_cast<SplayQuadtree::LeafNode*>(shrinkSplit->rightChild);
		EXPECT_FALSE(leaf->containsInnerBox);
		EXPECT_TRUE(leaf->outerBox.contains(testPoints[2]));
		EXPECT_EQ(testPoints[2], leaf->point); //  new point should be in right child
		ASSERT_EQ(SplayQuadtree::FILLED_LEAF_NODE, shrinkSplit->outerChild->type);
		leaf = dynamic_cast<SplayQuadtree::LeafNode*>(shrinkSplit->outerChild);
		EXPECT_EQ(initialBoundary, leaf->outerBox);
		EXPECT_TRUE(leaf->containsInnerBox);
		EXPECT_TRUE(leaf->innerBox.contains(shrinkSplit->leftChild->outerBox));
		EXPECT_TRUE(leaf->innerBox.contains(shrinkSplit->rightChild->outerBox));

		// Test inserting point where it is contained in left child of root
		ASSERT_TRUE(structure.insert(testPoints[3]));
		// Test inserting point where it is contained in right child of root
		ASSERT_TRUE(structure.insert(testPoints[4]));
		// Ensure that all three children of the root node are shrink-split nodes
		root = structure.rootNode();
		ASSERT_EQ(SplayQuadtree::SHRINKSPLIT_NODE, root->type);
		shrinkSplit = dynamic_cast<SplayQuadtree::ShrinkSplitNode*>(root);
		EXPECT_EQ(SplayQuadtree::SHRINKSPLIT_NODE, shrinkSplit->leftChild->type);
		EXPECT_EQ(SplayQuadtree::SHRINKSPLIT_NODE, shrinkSplit->rightChild->type);
		EXPECT_EQ(SplayQuadtree::SHRINKSPLIT_NODE, shrinkSplit->outerChild->type);

		// Test inserting point out of whole structure's boundaries
		ASSERT_FALSE(structure.insert(testPoints[5]));
	}

	TEST_F(SplayQuadtreeTests, PointQueries)
	{
		SplayQuadtree structure(NUM_SPLAYQUADTREE_DIMENSIONS, initialBoundary);
		IndexStructureTester tester;
		tester.testPointQueries(&structure);
	}

	TEST_F(SplayQuadtreeTests, Promotion)
	{
		SplayQuadtree structure(NUM_SPLAYQUADTREE_DIMENSIONS, initialBoundary);
		IndexStructureTester tester;

		SplayQuadtree::Node* root = NULL;
		SplayQuadtree::ShrinkSplitNode* shrinkSplit = NULL;
		SplayQuadtree::LeafNode* leaf = NULL;

		// Test root node
		structure.clear();
		structure.loadPoints(tester.getTestPoints());
		shrinkSplit = dynamic_cast<SplayQuadtree::ShrinkSplitNode*>(structure.rootNode());
		ASSERT_FALSE( structure.promote(shrinkSplit) ); // root node cannot be promoted any more

		// Test promoting right child (where left sibling DOES NOT HAVE AN INNER BOX)
		// NOTE: Equivalent as left node promotion, just with a swap
		SplayQuadtree::Node* oldParentLeftChild = shrinkSplit->leftChild;
		SplayQuadtree::ShrinkSplitNode* oldParentOuterChild = dynamic_cast<SplayQuadtree::ShrinkSplitNode*>(shrinkSplit->outerChild);
		SplayQuadtree::Node* oldGrandparent = shrinkSplit->parent;
		SplayQuadtree::ShrinkSplitNode* oldRightChild = dynamic_cast<SplayQuadtree::ShrinkSplitNode*>(shrinkSplit->rightChild);
		SplayQuadtree::Node* oldRightChildChildren[] = {
			oldRightChild->leftChild, oldRightChild->rightChild, oldRightChild->outerChild
		};
		ASSERT_TRUE( structure.promote(oldRightChild) );
		EXPECT_EQ(oldGrandparent, oldRightChild->parent); // promoted node's parent is its old grandparent
		EXPECT_EQ(shrinkSplit, oldRightChild->outerChild);
		EXPECT_EQ(oldRightChildChildren[0], oldRightChild->leftChild); // left and right children of promoted node should stay the same
		EXPECT_EQ(oldRightChildChildren[1], oldRightChild->rightChild);
		EXPECT_EQ(oldRightChild, shrinkSplit->parent);
		EXPECT_EQ(oldRightChildChildren[2], shrinkSplit->leftChild);
		EXPECT_EQ(oldParentLeftChild, shrinkSplit->rightChild); // old left is now right
		EXPECT_EQ(oldParentOuterChild, shrinkSplit->outerChild); // outer child remains same

		// Test promoting right child (where left sibling DOES HAVE AN INNER BOX)
		// This is not a valid promotion, so false should be returned
		structure.clear();
		structure.loadPoints(tester.getTestPoints());
		root = structure.rootNode();
		ASSERT_EQ(SplayQuadtree::SHRINKSPLIT_NODE, root->type);
		shrinkSplit = dynamic_cast<SplayQuadtree::ShrinkSplitNode*>(root);
		ASSERT_EQ(SplayQuadtree::FILLED_LEAF_NODE, shrinkSplit->leftChild->type);
		leaf = dynamic_cast<SplayQuadtree::LeafNode*>(shrinkSplit->leftChild);
		leaf->containsInnerBox = true; // make it seem like the left child has an inner box
		ASSERT_FALSE( structure.promote(dynamic_cast<SplayQuadtree::ShrinkSplitNode*>(shrinkSplit->rightChild)) );

		// Test promoting outer child
		structure.clear();
		structure.loadPoints(tester.getTestPoints());
		root = structure.rootNode();
		ASSERT_EQ(SplayQuadtree::SHRINKSPLIT_NODE, root->type);
		shrinkSplit = dynamic_cast<SplayQuadtree::ShrinkSplitNode*>(root);

		oldGrandparent = shrinkSplit->parent;
		oldParentLeftChild = shrinkSplit->leftChild;
		SplayQuadtree::Node* oldParentRightChild = shrinkSplit->rightChild;
		oldParentOuterChild = dynamic_cast<SplayQuadtree::ShrinkSplitNode*>(shrinkSplit->outerChild);
		SplayQuadtree::Node* oldPromotedLeftChild = oldParentOuterChild->leftChild;
		SplayQuadtree::Node* oldPromotedRightChild = oldParentOuterChild->rightChild;
		SplayQuadtree::Node* oldPromotedOuterChild = oldParentOuterChild->outerChild;

		ASSERT_TRUE(structure.promote( dynamic_cast<SplayQuadtree::ShrinkSplitNode*>(shrinkSplit->outerChild)) );
		EXPECT_EQ(oldGrandparent, oldParentOuterChild->parent); // promoted node's parent is its old grandparent
		EXPECT_EQ(shrinkSplit, oldParentOuterChild->leftChild); // left child of promoted becomes its old parent
		EXPECT_EQ(oldPromotedRightChild, oldParentOuterChild->rightChild); // right and outer children of promoted node should stay the same
		EXPECT_EQ(oldPromotedOuterChild, oldParentOuterChild->outerChild);
		EXPECT_EQ(oldParentOuterChild, shrinkSplit->parent);
		EXPECT_EQ(oldParentLeftChild, shrinkSplit->leftChild); // left and right children same
		EXPECT_EQ(oldParentRightChild, shrinkSplit->rightChild);
		EXPECT_EQ(oldPromotedLeftChild, shrinkSplit->outerChild); // left child of promoted becomes outer node
	}

	TEST_F(SplayQuadtreeTests, BasicSplay)
	{
		SplayQuadtree structure(NUM_SPLAYQUADTREE_DIMENSIONS, initialBoundary);
		IndexStructureTester tester;

		SplayQuadtree::Node* root = NULL;
		SplayQuadtree::ShrinkSplitNode* shrinkSplit = NULL;
		SplayQuadtree::ShrinkSplitNode* nodeToSplay = NULL;
		SplayQuadtree::LeafNode* leaf = NULL;

		// Test root node
		structure.clear();
		structure.loadPoints(tester.getTestPoints());		
		shrinkSplit = dynamic_cast<SplayQuadtree::ShrinkSplitNode*>(structure.rootNode());
		ASSERT_TRUE(structure.basicSplay(shrinkSplit));

		// Test zig (w/ right child chosen) -- EQUIVALENT TO A RIGHT CHILD PROMOTE
		structure.clear();
		structure.loadPoints(tester.getTestPoints());
		shrinkSplit = dynamic_cast<SplayQuadtree::ShrinkSplitNode*>(structure.rootNode());
		nodeToSplay = dynamic_cast<SplayQuadtree::ShrinkSplitNode*>(shrinkSplit->rightChild);
		SplayQuadtree::ShrinkSplitNode* oldParent = shrinkSplit;
		SplayQuadtree::Node* oldParentChildren[] = {
			oldParent->leftChild, oldParent->rightChild, oldParent->outerChild
		};
		SplayQuadtree::Node* nodeToSplayOldChildren[] = {
			nodeToSplay->leftChild, nodeToSplay->rightChild, nodeToSplay->outerChild
		};

		ASSERT_TRUE(structure.basicSplay(nodeToSplay));
		ASSERT_EQ(NULL, nodeToSplay->parent);
		ASSERT_EQ(nodeToSplayOldChildren[0], nodeToSplay->leftChild);
		ASSERT_EQ(nodeToSplayOldChildren[1], nodeToSplay->rightChild);
		ASSERT_EQ(oldParent, nodeToSplay->outerChild);
		ASSERT_EQ(nodeToSplay, oldParent->parent);
		ASSERT_EQ(nodeToSplayOldChildren[2], oldParent->leftChild);
		ASSERT_EQ(oldParentChildren[0], oldParent->rightChild);
		ASSERT_EQ(oldParentChildren[2], oldParent->outerChild);

		// Test zig-zag (OL)
		structure.clear();
		structure.loadPoints(tester.getTestPoints());
		// Insert points to make LO situation
		Real values[][3] = { { 7, 5, 5 }, { 9, 6, 6 } };
		ASSERT_TRUE( structure.insert(Point(3, values[0])) );
		ASSERT_TRUE( structure.insert(Point(3, values[1])) );
		// Get node to splay
		shrinkSplit = dynamic_cast<SplayQuadtree::ShrinkSplitNode*>(structure.rootNode());
		SplayQuadtree::ShrinkSplitNode* parent = dynamic_cast<SplayQuadtree::ShrinkSplitNode*>(shrinkSplit->leftChild);
		nodeToSplay = dynamic_cast<SplayQuadtree::ShrinkSplitNode*>(parent->outerChild);
		nodeToSplayOldChildren[0] = nodeToSplay->leftChild;
		nodeToSplayOldChildren[1] = nodeToSplay->rightChild;
		nodeToSplayOldChildren[2] = nodeToSplay->outerChild;
		oldParentChildren[0] = parent->leftChild;
		oldParentChildren[1] = parent->rightChild;
		oldParentChildren[2] = parent->outerChild;
		SplayQuadtree::Node* oldGrandparentChildren[] = {
			shrinkSplit->leftChild, shrinkSplit->rightChild, shrinkSplit->outerChild
		};

		ASSERT_TRUE(structure.basicSplay(nodeToSplay));
		ASSERT_EQ(NULL, nodeToSplay->parent);
		ASSERT_EQ(parent, nodeToSplay->leftChild);
		ASSERT_EQ(nodeToSplayOldChildren[1], nodeToSplay->rightChild);
		ASSERT_EQ(shrinkSplit, nodeToSplay->outerChild);
		ASSERT_EQ(nodeToSplay, parent->parent);
		ASSERT_EQ(oldParentChildren[0], parent->leftChild);
		ASSERT_EQ(oldParentChildren[1], parent->rightChild);
		ASSERT_EQ(nodeToSplayOldChildren[0], parent->outerChild);
		ASSERT_EQ(nodeToSplay, shrinkSplit->parent);
		ASSERT_EQ(nodeToSplayOldChildren[2], shrinkSplit->leftChild);
		ASSERT_EQ(oldGrandparentChildren[1], shrinkSplit->rightChild);
		ASSERT_EQ(oldGrandparentChildren[2], shrinkSplit->outerChild);

		// Test zig-zig (OO)
		structure.clear();
		structure.loadPoints(tester.getTestPoints());
		shrinkSplit = dynamic_cast<SplayQuadtree::ShrinkSplitNode*>(structure.rootNode());
		parent = dynamic_cast<SplayQuadtree::ShrinkSplitNode*>(shrinkSplit->outerChild);
		nodeToSplay = dynamic_cast<SplayQuadtree::ShrinkSplitNode*>(parent->outerChild);
		nodeToSplayOldChildren[0] = nodeToSplay->leftChild;
		nodeToSplayOldChildren[1] = nodeToSplay->rightChild;
		nodeToSplayOldChildren[2] = nodeToSplay->outerChild;
		oldParentChildren[0] = parent->leftChild;
		oldParentChildren[1] = parent->rightChild;
		oldParentChildren[2] = parent->outerChild;
		oldGrandparentChildren[0] = shrinkSplit->leftChild;
		oldGrandparentChildren[1] = shrinkSplit->rightChild;
		oldGrandparentChildren[2] = shrinkSplit->outerChild;

		ASSERT_TRUE(structure.basicSplay(nodeToSplay));
		ASSERT_EQ(NULL, nodeToSplay->parent);
		ASSERT_EQ(parent, nodeToSplay->leftChild);
		ASSERT_EQ(nodeToSplayOldChildren[1], nodeToSplay->rightChild);
		ASSERT_EQ(nodeToSplayOldChildren[2], nodeToSplay->outerChild);
		ASSERT_EQ(nodeToSplay, parent->parent);
		ASSERT_EQ(shrinkSplit, parent->leftChild);
		ASSERT_EQ(oldParentChildren[1], parent->rightChild);
		ASSERT_EQ(nodeToSplayOldChildren[0], parent->outerChild);
		ASSERT_EQ(parent, shrinkSplit->parent);
		ASSERT_EQ(oldGrandparentChildren[0], shrinkSplit->leftChild);
		ASSERT_EQ(oldGrandparentChildren[1], shrinkSplit->rightChild);
		ASSERT_EQ(oldParentChildren[0], shrinkSplit->outerChild);
	}

	TEST_F(SplayQuadtreeTests, Splay)
	{
		SplayQuadtree structure(NUM_SPLAYQUADTREE_DIMENSIONS, initialBoundary);
		IndexStructureTester tester;
		const PointList& testPoints = tester.getTestPoints();
		structure.loadPoints(testPoints);

		for (unsigned int i = 0; (i < testPoints.size()); i++)
		{
			SplayQuadtree::Node* node = structure.findNodeStoredIn(testPoints[i]);	
			if (node == NULL)
			{
				node = structure.findContainingNode(testPoints[i]);
				std::cout << testPoints[i] << " ";
				if (node)
					std::cout << node->outerBox << std::endl;
				else
					std::cout << "NULL" << std::endl;
				std::cout << std::endl;
				std::cout << structure.toString() << std::endl;
				ASSERT_FALSE(true);
			}

			ASSERT_EQ(SplayQuadtree::SHRINKSPLIT_NODE, node->parent->type);
			SplayQuadtree::ShrinkSplitNode* nonLeaf =
				dynamic_cast<SplayQuadtree::ShrinkSplitNode*>(node->parent);
			// Splay parent of leaf node and ensure it's now the root
			ASSERT_TRUE(structure.splay(nonLeaf));
			ASSERT_EQ(NULL, nonLeaf->parent);
			ASSERT_EQ(structure.rootNode(), nonLeaf);
			// Ensure all points can still be retrieved
			/*for (unsigned int j = 0; (j < testPoints.size()); j++)
			{
				structure.pointExists(testPoints[j]);
			}*/
		}
	}

} }

#endif