#ifndef MDSEARCH_TEST_SPLAYTREE_H
#define MDSEARCH_TEST_SPLAYTREE_H

#include <gtest/gtest.h>
#include "DataTypes.h"
#include "SplayTree.h"

namespace mdsearch { namespace tests
{

	typedef SplayTree<int, Real> SplayTreeType;

	class SplayTreeTests : public ::testing::Test
	{

	};

	TEST_F(SplayTreeTests, Construction)
	{
		SplayTreeType tree;
		SplayTreeType::Node* root = tree.rootNode();
		EXPECT_EQ(NULL, root);
	}

	TEST_F(SplayTreeTests, InsertAndClear)
	{
		SplayTreeType tree;
		SplayTreeType::Node* root = NULL;

		// Insert first value
		ASSERT_TRUE( tree.insert(-1, 5.0f) );
		root = tree.rootNode();
		ASSERT_FALSE(root == NULL);
		EXPECT_EQ(-1, root->key);
		EXPECT_EQ(5.0f, root->value);
		// Insert smaller and larger keys
		ASSERT_TRUE( tree.insert(-2, 132.0f) );
		ASSERT_TRUE( tree.insert(1, 435.0f) );
		ASSERT_EQ(root, tree.rootNode()); // root node should be the same
		ASSERT_FALSE(root->leftChild == NULL);
		ASSERT_FALSE(root->rightChild == NULL);
		EXPECT_EQ(-2, root->leftChild->key);
		EXPECT_EQ(132.0f, root->leftChild->value);
		EXPECT_EQ(1, root->rightChild->key);
		EXPECT_EQ(435.0f, root->rightChild->value);
		// Finally, insert the largest key so it's nested in the right child
		ASSERT_TRUE( tree.insert(2, 0.005f) );
		ASSERT_TRUE(root->rightChild->leftChild == NULL);
		ASSERT_FALSE(root->rightChild->rightChild == NULL);
		EXPECT_EQ(2, root->rightChild->rightChild->key);
		EXPECT_EQ(0.005f, root->rightChild->rightChild->value);
		// Clear structure and ensure that there are no nodes
		tree.clear();
		root = tree.rootNode();
		EXPECT_EQ(NULL, root);
	}

	TEST_F(SplayTreeTests, Remove)
	{
		SplayTreeType tree;
		// TODO
	}

	TEST_F(SplayTreeTests, Update)
	{
		SplayTreeType tree;
		// TODO
	}

	TEST_F(SplayTreeTests, Exists)
	{
		int testKeys[] = {
			0, 1, 5, 6, -4, 3, -1, 8
		};
		Real testValues[] = {
			0, 1, 2, 3, 4, 5, 6, 7
		};
		unsigned int NUM_TEST_PAIRS = 8;

		SplayTreeType tree;
		// Ensure points cannot be retrieved
		for (unsigned int i = 0; (i < NUM_TEST_PAIRS); i++)
		{
			ASSERT_EQ(NULL, tree.getValue(testKeys[i]));
		}
		// Load 10 different key-value pairs and check they exist
		for (unsigned int i = 0; (i < NUM_TEST_PAIRS); i++)
		{
			ASSERT_TRUE( tree.insert(testKeys[i], testValues[i]) );
			float* storedValue = tree.getValue(testKeys[i]);
			ASSERT_FALSE(storedValue == NULL);
			ASSERT_EQ(testValues[i], *storedValue);
		}
	}


	TEST_F(SplayTreeTests, Splay)
	{
		SplayTreeType tree;
		// TODO
	}

} }

#endif