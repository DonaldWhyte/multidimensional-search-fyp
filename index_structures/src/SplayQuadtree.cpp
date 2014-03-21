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
					NodeRelation rel = relation(parent, leaf);
					switch (rel)
					{
					case LEFT_CHILD_RELATION:
						parent->leftChild = newNode;
						break;
					case RIGHT_CHILD_RELATION:
						parent->rightChild = newNode;
						break;
					case OUTER_CHILD_RELATION:
						parent->outerChild = newNode;
						break;
					// leaf is not actually child of its assigned parent -- sohuld never happen
					// Return false if so, as the tree structure is invalid
					default:
						return false;
					}
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

	bool SplayQuadtree::promote(SplayQuadtree::ShrinkSplitNode* nodeToPromote)
	{
		// If root node or parent is not non-leaf (although it SHOULD be)
		if (!nodeToPromote->parent || nodeToPromote->parent->type != SHRINKSPLIT_NODE)
			return false;
		ShrinkSplitNode* parent = dynamic_cast<ShrinkSplitNode*>(nodeToPromote->parent);
		SplayQuadtree::Node* grandparent = parent->parent; // NULL is acceptable in this case

		NodeRelation rel = relation(parent, nodeToPromote);
		if (rel == OUTER_CHILD_RELATION)
		{
			// Perform outer node promotion
			Node* tmp = nodeToPromote->leftChild;
			nodeToPromote->leftChild = parent;
			nodeToPromote->parent = grandparent;
			parent->outerChild = tmp;
			parent->parent = nodeToPromote;
		}
		else // left or right children
		{
			// If node is the right child, then swap it with the left child.
			// ONLY IF THE LEFT CHILD DOE SNOT HAVE AN INNER BOX!
			// If it doesn't, then the 
			if (rel == RIGHT_CHILD_RELATION)
			{
				Node* tmp = parent->leftChild;
				if (tmp->type == FILLED_LEAF_NODE) // check left child doesn't have an inner box
				{
					LeafNode* leaf = dynamic_cast<LeafNode*>(tmp);
					if (leaf->containsInnerBox)
						return false;
				}
				parent->leftChild = nodeToPromote;
				parent->rightChild = tmp;
			}
			// Perform a left child promotion
			Node* tmp = nodeToPromote->outerChild;
			nodeToPromote->outerChild = parent;
			nodeToPromote->parent = grandparent;
			parent->leftChild = tmp;
			parent->parent = nodeToPromote;
		}
		return true;
	}

	bool SplayQuadtree::basicSplay(SplayQuadtree::ShrinkSplitNode* node)
	{
		if (!node->parent)
		{
			return true;
		}
		else
		{
			if (node->parent->type != SHRINKSPLIT_NODE)
				return false;
			ShrinkSplitNode* parent = dynamic_cast<ShrinkSplitNode*>(node->parent);
			// If parent is the root
			if (!parent->parent) // Zig
			{
				return promote(node);
			}
			else
			{
				if (parent->parent->type != SHRINKSPLIT_NODE)
					return false;
				ShrinkSplitNode* grandparent = dynamic_cast<ShrinkSplitNode*>(parent->parent);				
				NodeRelation relP = relation(parent, node);	
				NodeRelation relG = relation(grandparent, parent); // first char
				if ((relG == LEFT_CHILD_RELATION && relP == OUTER_CHILD_RELATION) ||
					(relG == LEFT_CHILD_RELATION && relP == OUTER_CHILD_RELATION) ||
					(relG == LEFT_CHILD_RELATION && relP == OUTER_CHILD_RELATION)) // Zig-Zag
				{
					promote(node);
					return promote(node); // TODO: return first promote or do if() with it??
				}
				else // Zig-Zig
				{
					promote(parent);
					return promote(node);
				}
			}

			return true;
		}
	}

	bool SplayQuadtree::splay(ShrinkSplitNode* node)
	{
		// --------PHASE 1----------
		std::cout << "PHASE 1" << std::endl;
		ShrinkSplitNode* currentNode = node;
		while (currentNode->parent)
		{
			TwoDeepRelation rel = twoDeepRelation(currentNode);
			while (disallowedInPhase1(rel)) // loop until two-deep relation is allowed
			{
				if ((rel.grandparentToParent == LEFT_CHILD_RELATION && rel.parentToNode == LEFT_CHILD_RELATION) ||
					(rel.grandparentToParent == OUTER_CHILD_RELATION && rel.parentToNode == OUTER_CHILD_RELATION))
				{
					ShrinkSplitNode* parent = dynamic_cast<ShrinkSplitNode*>(currentNode->parent);
					promote(parent);
					promote(currentNode);
				}
				else
				{
					promote(currentNode);
					promote(currentNode);
				}
				// Compute new new two-deep relation
				rel = twoDeepRelation(currentNode);
			}
			// If current node is root or its parent is the roots
			if (!currentNode->parent || !currentNode->parent->parent)
			{
				break;
			}
			else
			{
				ShrinkSplitNode* parent = dynamic_cast<ShrinkSplitNode*>(currentNode->parent);
				if (relation(parent, currentNode) == RIGHT_CHILD_RELATION)
				{
					currentNode = parent;
				}
				else
				{
					ShrinkSplitNode* grandparent = dynamic_cast<ShrinkSplitNode*>(parent->parent);
					currentNode = grandparent;
				}
			}
		}

		// --------PHASE 2----------
		std::cout << "PHASE 2" << std::endl;
		currentNode = node;
		while (currentNode->parent)
		{
			ShrinkSplitNode* parent = dynamic_cast<ShrinkSplitNode*>(currentNode->parent);
			TwoDeepRelation rel = twoDeepRelation(currentNode);
			// If str(x) == OR, perform a zig-zig
			if (rel.grandparentToParent == OUTER_CHILD_RELATION && rel.parentToNode == RIGHT_CHILD_RELATION)
			{
				promote(parent);
				promote(currentNode);
			}
			// Otherwise, just move up to the parent without making any changes
			else
			{
				currentNode = parent;
			}
		}

		// --------PHASE 3----------
		std::cout << "PHASE 3" << std::endl;
		while (node->parent) // repeatedly splay node until it is the root
		{
			basicSplay(node);
		}
		root = node; // make sure to assign root!
		return true;
	}

	SplayQuadtree::Node* SplayQuadtree::rootNode()
	{
		return root;
	}

	SplayQuadtree::LeafNode* SplayQuadtree::findContainingNode(const Point& p) const
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
				return NULL;
			}
		}
	}

	SplayQuadtree::LeafNode* SplayQuadtree::findNodeStoredIn(const Point& p) const
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
		expandToQuadtreeBox(leaf->outerBox, minQuadtreeBox);

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

	SplayQuadtree::NodeRelation SplayQuadtree::relation(
		SplayQuadtree::ShrinkSplitNode* parent,
		SplayQuadtree::Node* child) const
	{
		if (parent->leftChild == child)
			return LEFT_CHILD_RELATION;
		else if (parent->rightChild == child)
			return RIGHT_CHILD_RELATION;
		else if (parent->outerChild == child)
			return OUTER_CHILD_RELATION;
		else
			return NO_RELATION;
	}	

	SplayQuadtree::TwoDeepRelation SplayQuadtree::twoDeepRelation(
		SplayQuadtree::ShrinkSplitNode* node) const
	{
		if (!node->parent || node->parent->type != SHRINKSPLIT_NODE) // SHOULDN'T HAPPEN
			return TwoDeepRelation(NO_RELATION, NO_RELATION);
		ShrinkSplitNode* parent = dynamic_cast<ShrinkSplitNode*>(node->parent);

		if (!parent->parent || parent->parent->type != SHRINKSPLIT_NODE) // SHOULDN'T HAPPEN
			return TwoDeepRelation(NO_RELATION, relation(parent, node));
		ShrinkSplitNode* grandparent = dynamic_cast<ShrinkSplitNode*>(parent->parent);

		return TwoDeepRelation(relation(grandparent, parent), relation(parent, node));
	}

	bool SplayQuadtree::disallowedInPhase1(SplayQuadtree::TwoDeepRelation rel) const
	{
		return ((rel.grandparentToParent == LEFT_CHILD_RELATION && rel.parentToNode == LEFT_CHILD_RELATION) ||
			(rel.grandparentToParent == OUTER_CHILD_RELATION && rel.parentToNode == OUTER_CHILD_RELATION) ||
			(rel.grandparentToParent == LEFT_CHILD_RELATION && rel.parentToNode == OUTER_CHILD_RELATION) ||
			(rel.grandparentToParent == OUTER_CHILD_RELATION && rel.parentToNode == LEFT_CHILD_RELATION)
		);
	}

	void SplayQuadtree::expandToQuadtreeBox(const Region& parentCell, Region& boxToExpand)
	{
		static const Real BOX_GAP_THRESHOLD = 0.5; // to ensure boxes w/ aspect ratio of 1 or 2

		// Find length of longest side of box to expand
		Real longestLength = parentCell.longestLength();

		for (unsigned int d = 0; (d < numDimensions); d++)
		{
			// Gaps between tight bound box and parent cell box
			Real gapHi = parentCell[d].max - boxToExpand[d].max;
			Real gapLow = boxToExpand[d].min - parentCell[d].min;
			if (gapHi < longestLength * BOX_GAP_THRESHOLD) // big enough gap to shrink?
				boxToExpand[d].max = parentCell[d].max; // no - expand
			if (gapLow < longestLength * BOX_GAP_THRESHOLD)
				boxToExpand[d].min = parentCell[d].min;
		}
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