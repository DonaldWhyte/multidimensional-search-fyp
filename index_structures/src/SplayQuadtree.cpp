#include "SplayQuadtree.h"

#include <sstream>

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
					{
						parent->leftChild = newNode;
					}
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
		return (findNodeStoredIn(p) != NULL);
	}

	PointList SplayQuadtree::pointsInRegion(const Region& region)
	{
		return PointList();
	}

	bool SplayQuadtree::promote(SplayQuadtree::Node* node)
	{
		// Cannot promote if the given node is a leaf or the root
		if (node->type != SHRINKSPLIT_NODE || node->parent == NULL)
		{
			return false;
		}
		else
		{
			// Perform necessary casts
			ShrinkSplitNode* nodeToPromote = dynamic_cast<ShrinkSplitNode*>(node);
			if (node->parent->type != SHRINKSPLIT_NODE)
				return false;
			ShrinkSplitNode* parent = dynamic_cast<ShrinkSplitNode*>(node->parent);
			SplayQuadtree::Node* grandparent = parent->parent;
			if (node == parent->outerChild)
			{
				// Perform outer node promotion
				Node* tmp = nodeToPromote->leftChild;
				nodeToPromote->leftChild = parent;
				nodeToPromote->parent = grandparent;
				parent->outerChild = tmp;
				parent->parent = node;
			}
			else // left or right children
			{
				// If node is the right child, then swap it with the left child.
				// ONLY IF THE LEFT CHILD DOE SNOT HAVE AN INNER BOX!
				// If it doesn't, then the 
				if (node == parent->rightChild)
				{
					Node* tmp = parent->leftChild;
					if (tmp->type == FILLED_LEAF_NODE) // check left child doesn't have an inner box
					{
						LeafNode* leaf = dynamic_cast<LeafNode*>(tmp);
						if (leaf->containsInnerBox)
							return false;
					}
					parent->leftChild = node;
					parent->rightChild = tmp;
				}
				// Perform a left child promotion
				Node* tmp = nodeToPromote->outerChild;
				nodeToPromote->outerChild = parent;
				nodeToPromote->parent = grandparent;
				parent->leftChild = tmp;
				parent->parent = node;
			}
			return true;
		}
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
					{
						node = nonLeaf->leftChild;
					}
					else if (nonLeaf->rightChild && nonLeaf->rightChild->outerBox.contains(p))
					{
						node = nonLeaf->rightChild;
					}
					// If the point is contained in the shrink split node, but NOT
					// the left or right children regions (which together form the
					// inner box of the outer child), then the point MUST be in the
					// outer child!
					else
					{
						node = nonLeaf->outerChild;
					}
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

	SplayQuadtree::LeafNode* SplayQuadtree::findNodeStoredIn(const Point& p)
	{
		LeafNode* leaf = findContainingNode(p);
		if (leaf && !leaf->containsInnerBox && leaf->point == p)
			return leaf;
		else
			return NULL;
	}

	SplayQuadtree::ShrinkSplitNode* SplayQuadtree::performShrinkSplit(
		SplayQuadtree::LeafNode* leaf, const Point& p)
	{
		// Compute minimum quadtree BOX that encloses the existing content and new point
		Region minQuadtreeBox(numDimensions);
		if (leaf->containsInnerBox)
			minQuadtreeBox = Region::minimumBoundingBox(leaf->innerBox, p);
		else
			minQuadtreeBox = Region::minimumBoundingBox(leaf->point, p);

		// Find longest side of minimum quadtree box and use it to split region	
		unsigned int longestSide = minQuadtreeBox.findLongestDimension();
		// Compute value of longest side to split at (midpoint of longest side
		Real splitPoint = minQuadtreeBox[longestSide].min +
			((minQuadtreeBox[longestSide].max - minQuadtreeBox[longestSide].min) / 2.0f);
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
			leaf->outerBox, leftChild, rightChild, outerChild, leaf->parent
		);
		leftChild->parent = newNode;
		rightChild->parent = newNode;
		outerChild->parent = newNode;

		return newNode;
	}

	std::string SplayQuadtree::toString() const
	{
		return nodeToString(root);
	}

	std::string SplayQuadtree::nodeToString(SplayQuadtree::Node* node, int level) const
	{
		std::stringstream ss;
		for (unsigned int i = 0; (i < level); i++)
			ss << "\t";

		LeafNode* leaf = NULL;
		ShrinkSplitNode* shrinkSplit = NULL;
		switch (node->type)
		{
		case EMPTY_LEAF_NODE:
			ss << "LEAF (" << node->outerBox << ") -- EMPTY\n";
			break;
		case FILLED_LEAF_NODE:
			leaf = dynamic_cast<LeafNode*>(node);
			ss << "LEAF (" << leaf->outerBox << ") -- ";
			if (leaf->containsInnerBox)
				ss << "CONTAINS INNER BOX " << leaf->innerBox;
			else
				ss << "CONTAINS POINT " << leaf->point;
			ss << "\n";
			break;
		case SHRINKSPLIT_NODE:
			shrinkSplit = dynamic_cast<ShrinkSplitNode*>(node);
			ss << "SHRINK-SPLIT (" << shrinkSplit->outerBox << ")\n";
			ss << nodeToString(shrinkSplit->leftChild, level + 1);
			ss << nodeToString(shrinkSplit->rightChild, level + 1);
			ss << nodeToString(shrinkSplit->outerChild, level + 1);
		}
		return ss.str();
	}

}