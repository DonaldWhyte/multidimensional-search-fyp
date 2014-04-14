#ifndef MDSEARCH_TEST_SPLAYTREE_H
#define MDSEARCH_TEST_SPLAYTREE_H

#include <gtest/gtest.h>
#include "DataTypes.h"
#include "SplayTree.h"

namespace mdsearch { namespace tests
{

	typedef SplayTree<int, Real> TestSplayTreeType;

	static const int SPLAYTREE_TEST_KEYS[] = {
		0, 1, 5, 6, -4, 3, -1, 8
	};
	static const Real SPLAYTREE_TEST_VALUES[] = {
		0, 1, 2, 3, 4, 5, 6, 7
	};
	static const unsigned int SPLAYTREE_NUM_TEST_PAIRS = 8;		

	class SplayTreeTests : public ::testing::Test
	{	

	};

	TEST_F(SplayTreeTests, Construction)
	{
		TestSplayTreeType tree;
		TestSplayTreeType::Node* root = tree.rootNode();
		EXPECT_EQ(NULL, root);
	}

	/* Tests insertion, splay operation and clear. */
	TEST_F(SplayTreeTests, InsertSplayAndClear)
	{
		TestSplayTreeType tree;
		TestSplayTreeType::Node* root = NULL;

		// NOTE: Newly inserted element should ALWAYS be the root node

		// Insert first value
		ASSERT_TRUE( tree.insert(-1, 5.0f) );
		root = tree.rootNode();
		ASSERT_FALSE(root == NULL);
		EXPECT_EQ(-1, root->key);
		EXPECT_EQ(5.0f, root->value);
		// Insert smaller key
		ASSERT_TRUE( tree.insert(-2, 132.0f) );
		ASSERT_EQ(-2, tree.rootNode()->key);
		ASSERT_EQ(132.0f, tree.rootNode()->value);
		ASSERT_FALSE(tree.rootNode()->leftChild);
		ASSERT_TRUE(tree.rootNode()->rightChild);
		ASSERT_EQ(-1, tree.rootNode()->rightChild->key);
		ASSERT_EQ(5.0f, tree.rootNode()->rightChild->value);
		// Insert larger key
		ASSERT_TRUE( tree.insert(1, 435.0f) );
		ASSERT_EQ(1, tree.rootNode()->key);
		ASSERT_EQ(435.0f, tree.rootNode()->value);
		ASSERT_TRUE(tree.rootNode()->leftChild);
		ASSERT_FALSE(tree.rootNode()->rightChild);
		ASSERT_EQ(-1, tree.rootNode()->leftChild->key);
		ASSERT_EQ(5.0f, tree.rootNode()->leftChild->value);
		ASSERT_TRUE(tree.rootNode()->leftChild->leftChild);
		ASSERT_FALSE(tree.rootNode()->leftChild->rightChild);
		ASSERT_EQ(-2, tree.rootNode()->leftChild->leftChild->key);
		ASSERT_EQ(132.0f, tree.rootNode()->leftChild->leftChild->value);
		// Insert medium key so it has left and right children
		ASSERT_TRUE( tree.insert(0, 0.005f) );
		ASSERT_EQ(0, tree.rootNode()->key);
		ASSERT_EQ(0.005f, tree.rootNode()->value);	
		ASSERT_TRUE(tree.rootNode()->leftChild);
		ASSERT_TRUE(tree.rootNode()->leftChild->leftChild);
		ASSERT_FALSE(tree.rootNode()->leftChild->rightChild);
		ASSERT_EQ(-1, tree.rootNode()->leftChild->key);
		ASSERT_EQ(5.0f, tree.rootNode()->leftChild->value);
		ASSERT_EQ(-2, tree.rootNode()->leftChild->leftChild->key);
		ASSERT_EQ(132.0f, tree.rootNode()->leftChild->leftChild->value);
		ASSERT_TRUE(tree.rootNode()->rightChild);
		ASSERT_FALSE(tree.rootNode()->rightChild->leftChild);
		ASSERT_FALSE(tree.rootNode()->rightChild->rightChild);
		ASSERT_EQ(1, tree.rootNode()->rightChild->key);
		ASSERT_EQ(435.0f, tree.rootNode()->rightChild->value);
		// Clear structure and ensure that there are no nodes
		tree.clear();
		root = tree.rootNode();
		EXPECT_EQ(NULL, root);
	}

	TEST_F(SplayTreeTests, RemoveAndGetValue)
	{
		TestSplayTreeType tree;
		// Ensure points cannot be retrieved
		for (unsigned int i = 0; (i < SPLAYTREE_NUM_TEST_PAIRS); i++)
		{
			ASSERT_EQ(NULL, tree.getValue(SPLAYTREE_TEST_KEYS[i]));
		}
		// Load 10 different key-value pairs and check they exist
		for (unsigned int i = 0; (i < SPLAYTREE_NUM_TEST_PAIRS); i++)
		{
			ASSERT_TRUE( tree.insert(SPLAYTREE_TEST_KEYS[i], SPLAYTREE_TEST_VALUES[i]) );
			// Ensure newly inserted key and ALL PREVIOUS INSERTED KEYS are
			// still accessible
			for (unsigned int j = 0; (j <= i); j++)
			{
				Real* storedValue = tree.getValue(SPLAYTREE_TEST_KEYS[j]);
				ASSERT_FALSE(storedValue == NULL);
				ASSERT_EQ(SPLAYTREE_TEST_VALUES[j], *storedValue);
			}
		}
		// Remove each of the 10 key-value pairs, ensuring that it cannot be
		// retrieved after removal (but the remaining pairs can be retrieved!)
		for (unsigned int i = 0; (i < SPLAYTREE_NUM_TEST_PAIRS); i++)
		{
			ASSERT_TRUE( tree.remove(SPLAYTREE_TEST_KEYS[i]) );
			for (unsigned int j = 0; (j <= i); j++) // ensuring keys are deleted...
			{
				ASSERT_EQ(NULL, tree.getValue(SPLAYTREE_TEST_KEYS[j]));
			}
			for (unsigned int j = i + 1; (j < SPLAYTREE_NUM_TEST_PAIRS); j++) //...but that the others can still be found
			{
				Real* storedValue = tree.getValue(SPLAYTREE_TEST_KEYS[j]);
				ASSERT_FALSE(storedValue == NULL);			
				ASSERT_EQ(SPLAYTREE_TEST_VALUES[j], *storedValue);
			}
		}		
	}

	TEST_F(SplayTreeTests, Update)
	{
		TestSplayTreeType tree;
		// Load test points in
		for (unsigned int i = 0; (i < SPLAYTREE_NUM_TEST_PAIRS); i++)
		{
			ASSERT_TRUE( tree.insert(SPLAYTREE_TEST_KEYS[i], SPLAYTREE_TEST_VALUES[i]) );
		}

		// Test with key that is not in tree
		ASSERT_FALSE(tree.update(5000, 3052, 88.0f));
		// Test with existing key, where the NEW key is already in the structure
		ASSERT_TRUE(tree.getValue(1)); ASSERT_EQ(1.0f, *tree.getValue(1));
		ASSERT_TRUE(tree.getValue(-4)); ASSERT_EQ(4.0f, *tree.getValue(-4));
		ASSERT_TRUE(tree.update(1, -4, 22.0f));
		ASSERT_EQ(NULL, tree.getValue(1));
		ASSERT_TRUE(tree.getValue(-4)); ASSERT_EQ(22.0f, *tree.getValue(-4));
		// Test with existing key, where the NEW key is NOT in the structure
		ASSERT_TRUE(tree.getValue(8)); ASSERT_EQ(7.0f, *tree.getValue(8));
		ASSERT_EQ(NULL, tree.getValue(3052));
		ASSERT_TRUE(tree.update(8, 3052, 88.0f));
		ASSERT_EQ(NULL, tree.getValue(8));
		ASSERT_TRUE(tree.getValue(3052)); ASSERT_EQ(88.0f, *tree.getValue(3052));
	}

} }

#endif