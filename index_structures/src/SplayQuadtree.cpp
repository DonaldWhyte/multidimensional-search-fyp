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
		if (!leaf) // if no node containing the point could be found (out of bounds)
		{
			return false;
		}
		else if (!leaf->containsInnerBox && leaf->point == p) // if point already exists
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
				if (leaf->parent)
				{
					// Replace existing leaf with new shrink-split node
					ShrinkSplitNode* parent = dynamic_cast<ShrinkSplitNode*>(leaf->parent);
					if (parent->leftChild == leaf)
						parent->leftChild = newNode;
					else if (parent->rightChild == leaf)
						parent->rightChild = newNode;
					else if (parent->outerChild == leaf)
						parent->outerChild = newNode;
					// leaf is not actually child of its assigned parent -- sohuld never happen
					// Return false if so, as the tree structure is invalid
					else
						return false;
					delete leaf; // free memory used for old leaf
				}
				else // if node to replace is the root
				{
					delete leaf;
					root = newNode;
				}

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
		LeafNode* leaf = findContainingNode(p);
		if (leaf && !leaf->containsInnerBox && leaf->point == p)
		{
			return true;
		}
		else
		{
			return false;
		}
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
					if (nonLeaf->rightChild && nonLeaf->rightChild->outerBox.contains(p))
						node = nonLeaf->rightChild;
					// If the point is contained in the shrink split node, but NOT
					// the left or right children regions (which together form the
					// inner box of the outer child), then the point MUST be in the
					// outer child!
					else
						node = nonLeaf->outerChild;
				}
				else // leaf node
				{
					return dynamic_cast<LeafNode*>(node);
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
		// Compute minimum quadtree box that enclodes the contents of thener box
		Region minQuadtreeBox(numDimensions);
		if (leaf->containsInnerBox)
			minQuadtreeBox = Region::minimumBoundingBox(leaf->innerBox, p);
		else
			minQuadtreeBox = Region::minimumBoundingBox(leaf->point, p);
		// Find longest side of minimum quadtree box and use it to split region	
		unsigned int longestSide = minQuadtreeBox.findLongestDimension();
		// Compute value of longest side to split at (midpoint of region/point)
		Real splitPoint = computeSplitPoint(leaf, p, minQuadtreeBox, longestSide);
		// Construct split regions
		Region leftSplit, rightSplit;
		minQuadtreeBox.split(longestSide, splitPoint, leftSplit, rightSplit);

		// Determine which split is for 
		const Region& newPointSplit = (leftSplit.contains(p)) ? leftSplit : rightSplit;
		const Region& existingContentsSplit = (leftSplit.contains(p)) ? rightSplit : leftSplit;

		// Construct inner box leaf if the original node contained an inner box
		LeafNode* leftChild = NULL;
		if (leaf->containsInnerBox)
			leftChild = new LeafNode(existingContentsSplit, leaf->innerBox, NULL);
		else
			leftChild = new LeafNode(existingContentsSplit, leaf->point, NULL);
		LeafNode* rightChild = new LeafNode(newPointSplit, p, NULL);
		LeafNode* outerChild = new LeafNode(leaf->outerBox, minQuadtreeBox, NULL); // outer child always new inner box node
		// Construct new non-leaf node
		ShrinkSplitNode* newNode = new ShrinkSplitNode(
			leaf->outerBox, leftChild, rightChild, outerChild
		);
		leftChild->parent = newNode;
		rightChild->parent = newNode;
		outerChild->parent = newNode;

		return newNode;
	}	

	Real SplayQuadtree::computeSplitPoint(LeafNode* leaf, const Point& newPoint,
		const Region& minQuadtreeBox, unsigned int splitDimension)
	{
		Real min = minQuadtreeBox[splitDimension].min;
		Real max = minQuadtreeBox[splitDimension].max;
		Real diffBetweenContents = 0;
		if (leaf->containsInnerBox)
		{
			Real innerBoxMin = leaf->innerBox[splitDimension].min;
			Real innerBoxMax = leaf->innerBox[splitDimension].max;
			if (newPoint[splitDimension] > innerBoxMax) // if point is to RIGHT of inner box
				diffBetweenContents = std::abs(newPoint[splitDimension] - innerBoxMax);
			else // if point is to LEFT of inner box
				diffBetweenContents = std::abs(innerBoxMin - newPoint[splitDimension]);	

			return min + innerBoxMin + (diffBetweenContents / 2.0f);
		}
		else
		{
			diffBetweenContents = std::abs(leaf->point[splitDimension] - newPoint[splitDimension]);
			return min + (diffBetweenContents / 2.0f);
		}
	}

}