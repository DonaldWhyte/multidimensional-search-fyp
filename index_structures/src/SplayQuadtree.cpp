#include "SplayQuadtree.h"

namespace mdsearch
{

	SplayQuadtree::SplayQuadtree(unsigned int numDimensions, const Region& initialBoundary)
		: IndexStructure(numDimensions), boundary(initialBoundary), root(NULL)
	{
		clear();
	}

	SplayQuadtree::~SplayQuadtree()
	{
		delete root;
	}

	void SplayQuadtree::clear()
	{
		delete root;
		root = new LeafNode(boundary);
	}

	bool SplayQuadtree::insert(const Point& p)
	{
		LeafNode* leaf = findContainingNode(p);
		if (!leaf->containsInnerBox && leaf->point == p)
		{
			return false;
		}
		else
		{
			if (leaf->type == EMPTY_LEAF_NODE) // leaf node with nothing in it
			{
				leaf->type = FILLED_LEAF_NODE;
				leaf->containsInnerBox = false;
				leaf->point = p;
			}
			else // filled leaf node
			{
				ShrinkSplitNode* newNode = performShrinkSplit(leaf, p);
				// Replace existing leaf with new shrink-split node
				ShrinkSplitNode* parent = dynamic_cast<ShrinkSplitNode*>(leaf->parent);
				if (parent->leftChild == leaf)
					parent->leftChild = newNode;
				else if (parent->leftChild == leaf)
					parent->rightChild = newNode;
				else if (parent->leftChild == leaf)
					parent->leftChild = newNode;
				// leaf is not actually child of its assigned parent???
				// Return false if so,as the tree structure is invalid
				else
					return false;
				delete leaf; // free memory used for old leaf

				return true;
			}
		}
	}

	bool SplayQuadtree::remove(const Point& p)
	{
		return false;
	}

	bool SplayQuadtree::update(const Point& oldPoint, const Point& newPoint)
	{
		return false;
	}

	bool SplayQuadtree::pointExists(const Point& p)
	{
		return false;
	}

	PointList SplayQuadtree::pointsInRegion(const Region& region)
	{
		return PointList();
	}

	SplayQuadtree::Node* SplayQuadtree::rootNode()
	{
		return root;
	}

	SplayQuadtree::LeafNode* SplayQuadtree::findContainingNode(const Point& p)
	{
		Node* node = root;
		while (true)
		{
			if (node->outerBox.contains(p))
			{
				if (node->type == SHRINKSPLIT_NODE)
				{
					ShrinkSplitNode* nonLeaf = dynamic_cast<ShrinkSplitNode*>(node);
					if (nonLeaf->leftChild && nonLeaf->leftChild->outerBox.contains(p))
						node = nonLeaf->leftChild;
					if (nonLeaf->leftChild && nonLeaf->leftChild->outerBox.contains(p))
						node = nonLeaf->rightChild;
					else if (nonLeaf->leftChild && nonLeaf->leftChild->outerBox.contains(p))
						node = nonLeaf->outerChild;
					else // NOTE: This should not happen!!!
						return NULL;
				}
				else // leaf node
				{
					LeafNode* leaf = dynamic_cast<LeafNode*>(node);
					if (leaf->containsInnerBox)
					{
						if (!leaf->innerBox.contains(p)) // if the point is NOT in the inner box
							return leaf;
						// NOTE: this sohuld never be reached as the inner box (left/right children) will checked first
						else
							return NULL;
					}
					else
					{
						return leaf;
					}
				}
			}
			else
			{
				return NULL; // not contained in structure at all!
			}
		}	
	}

	SplayQuadtree::ShrinkSplitNode* SplayQuadtree::performShrinkSplit(
		SplayQuadtree::LeafNode* leaf, const Point& p)
	{
		// Compute minimum quadtree box that enclodes the contents of the
		// containing both points or the point+inner box
		Region minQuadtreeBox(numDimensions);
		if (leaf->containsInnerBox)
			minQuadtreeBox = Region::minimumBoundingBox(leaf->point, p);
		else
			minQuadtreeBox = Region::minimumBoundingBox(leaf->innerBox, p);

		// Find longest side of minimum quadtree box and use it to split region	
		unsigned int longestSide = minQuadtreeBox.findLongestDimension();
		// Compute value of longest side to split at
		Real min = minQuadtreeBox[longestSide].min;
		Real max = minQuadtreeBox[longestSide].max;	
		Real diffBetweenContents = 0;
		if (leaf->containsInnerBox)
		{
			Real innerBoxMin = leaf->innerBox[longestSide].min;
			Real innerBoxMax = leaf->innerBox[longestSide].max;
			if (p[longestSide] > innerBoxMax) // if point is to RIGHT of inner box
				diffBetweenContents = std::abs(p[longestSide] - innerBoxMax);
			else // if point is to LEFT of inner box
				diffBetweenContents = std::abs(innerBoxMin - p[longestSide]);			
		}
		else
		{
			diffBetweenContents = std::abs(leaf->point[longestSide] - p[longestSide]);
		}
		Real splitPoint = min + (diffBetweenContents / 2.0f);
		// Construct split regions
		Region leftSplit, rightSplit;
		minQuadtreeBox.split(longestSide, splitPoint, leftSplit, rightSplit);

		// Determine which split is for 
		const Region& newPointSplit = (leftSplit.contains(p)) ? leftSplit : rightSplit;
		const Region& existingContentsSplit = (leftSplit.contains(p)) ? rightSplit : leftSplit;

		/*
		LeafNode* leftChild = new LeafNode(TODO);
		LeafNode* rightChild = new LeafNode(TODO);
		LeafNode* outerChild = new LeafNode(TODO);
		ShrinkSplitNode newNode = new ShrinkSplitNode(
			leaf->TODO, leftChild, rightChild, outerChild
		);*/

		return NULL;
	}	

}