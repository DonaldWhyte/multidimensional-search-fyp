#include "BDTree.h"
#include <algorithm>

namespace mdsearch
{

	BDTree::Node::Node(BDTree::NodeType type, Node* parent,
		const Region& boundary, const Point& point) :
		type(type), parent(parent), leftChild(NULL), rightChild(NULL),
		point(point), boundary(boundary)
	{

	}

	BDTree::Node::~Node()
	{
		delete leftChild;
		delete rightChild;
	}

	bool BDTree::Node::contains(const Point& p)
	{
		if (type == OUTER_NODE)
		{
			// Check if point is in set theoretic difference between the two
			// boundaries (i.e. is point in parent's outer boundary but NOT
			// n the boundary of the parent's inner node (split node)
			const Region& outerBoundary = parent->boundary;
			const Region& innerBoundary = parent->leftChild->boundary;;
			return (outerBoundary.contains(p) && (!innerBoundary.contains(p)));
		}
		else
		{
			return boundary.contains(p);
		}
	}

	BDTree::BDTree(unsigned int numDimensions, const Region& boundary) :
		IndexStructure(numDimensions), root(NULL), boundary(boundary)
	{
		clear();
	}

	void BDTree::clear()
	{
		// Simply delete the root node and replace it with a new rooy
		delete root;
		// Construct empty inner box node for root
		root = new Node(EMPTY_NODE, NULL, boundary);
	}

	bool BDTree::insert(const Point& p)
	{
		// Find leaf node that contains point
		Node* node = findContainingNode(p);
		if (!node)
			return false;
		// Check if point is actually contained in this leaf node (already exists if so)
		if (node->type == LEAF_POINT_NODE && node->point == p)
			return false;

		// If node is empty, just add the point to it
		if (node->type == EMPTY_NODE)
		{
			node->type = LEAF_POINT_NODE;
			node->point = p;
			return true;
		}
		else if (node->type == LEAF_POINT_NODE || node->type == LEAF_BOX_NODE || node->type == OUTER_NODE) // all leaf node types
		{
			// Turn this leaf node into a SHRINK NODE
			shrink(node, p);
		}
		else
		{
			// NOTE: other types should never happen as they cannot be leaves!
			return false;
		}

		return false;
	}

	bool BDTree::remove(const Point& p)
	{
		// TODO
		return false;
	}

	bool BDTree::update(const Point& oldPoint, const Point& newPoint)
	{
		if (remove(oldPoint))
		{
			insert(newPoint);
			return true;
		}
		else
		{
			return false;
		}
	}

	bool BDTree::pointExists(const Point& p)
	{
		// Find node that potentially stores p
		Node* node = findContainingNode(p);
		if (node)
		{
			// If it's not a leaf node that stores p, p is NOT stored in the structure
			return (node->type == LEAF_POINT_NODE && node->point == p);
		}
		else
		{
			return false;
		}
	}

	PointList BDTree::pointsInRegion(const Region& region)
	{
		// TODO
		return PointList();
	}

	BDTree::Node* BDTree::findContainingNode(const Point& p)
	{
		Node* node = root;
		while (true)
		{
			if (node->contains(p))
			{
				if (node->leftChild && node->leftChild->contains(p))
					node = node->leftChild;
				else if (node->rightChild && node->rightChild->contains(p))
					node = node->rightChild;
				else
					return node;
			}
			else
			{
				return NULL;
			}
		}		
	}	

	BDTree::Node* BDTree::split(BDTree::Node* node, const Point& newPoint)
	{
		// Compute minimum quadtree box that enclodes the contents of the
		// containing both points or the point+inner box
		Region minQuadtreeBox(numDimensions);
		if (node->type == LEAF_POINT_NODE)
			minQuadtreeBox = minimumBoundingBox(node->point, newPoint);
		else
			minQuadtreeBox = minimumBoundingBox(node->boundary, newPoint); // TODO: separate inner box?
		Node* splitNode = new Node(SPLIT_NODE, node, minQuadtreeBox);

		// Find longest side of minimum quadtree box and use it to split region	
		unsigned int longestSide = minQuadtreeBox.findLongestDimension();

		// Compute value of longest side to split at
		Real min = minQuadtreeBox[longestSide].min;
		Real max = minQuadtreeBox[longestSide].max;			
		Real diffBetweenContents = 0;
		if (node->type == LEAF_POINT_NODE)
		{
			diffBetweenContents = std::abs(node->point[longestSide] - newPoint[longestSide]);
		}
		else
		{
			Real innerBoxMin = node->boundary[longestSide].min;
			Real innerBoxMax = node->boundary[longestSide].max;
			if (newPoint[longestSide] > innerBoxMax) // if point is to RIGHT of inner box
				diffBetweenContents = std::abs(newPoint[longestSide] - innerBoxMax);
			else // if point is to LEFT of inner box
				diffBetweenContents = std::abs(innerBoxMin - newPoint[longestSide]);
		}
		Real splitPoint = min + (diffBetweenContents / 2.0f);
		// Construct split regions
		Region leftSplit = minQuadtreeBox;
		leftSplit[longestSide] = Interval(min, splitPoint);
		Region rightSplit = minQuadtreeBox;			
		rightSplit[longestSide] = Interval(splitPoint, max);

		// Determine which split is for 
		const Region& newPointSplit = (leftSplit.contains(newPoint)) ? leftSplit : rightSplit;
		const Region& existingContentsSplit = (leftSplit.contains(newPoint)) ? rightSplit : leftSplit;

		// Type of left child differs depending on the node type
		if (node->type == LEAF_POINT_NODE)
		{
			const Point& originalNodePoint = node->point;
			splitNode->leftChild = new Node(LEAF_POINT_NODE, splitNode, existingContentsSplit, originalNodePoint);	
		}
		else if (node->type == LEAF_BOX_NODE || node->type == OUTER_NODE)
		{
			splitNode->leftChild = new Node(LEAF_BOX_NODE, splitNode, existingContentsSplit);
		}

		// Right child alays contains the newly inserted point
		splitNode->rightChild = new Node(LEAF_POINT_NODE, splitNode, newPointSplit, newPoint);

		return splitNode;
	}

	void BDTree::shrink(BDTree::Node* node, const Point& newPoint)
	{
		// Turn current node into a shrink node
		node->type == SHRINK_NODE;
		// Construct split node and make it inner (left) child of shrink node
		node->leftChild = split(node, newPoint);
		// Construct outer node and make it outer (right) child of shrink node
		Node* outerNode = new Node(OUTER_NODE, node, node->boundary);
		node->rightChild = outerNode;
		// Be sure to clear the stored point from the (now) shrink node
		node->point = Point(0);
	}

	bool BDTree::promote(BDTree::Node* node)
	{
		// TODO
	}

	Region BDTree::minimumBoundingBox(const Point& existingContents, const Point& newPoint)
	{
		// Find min and max values for each dimension of two points. This is minimum boundary
		Region minBoundBox(existingContents.numDimensions(), Interval(0, 0));
		for (unsigned int d = 0; (d < existingContents.numDimensions()); d++)
		{
			if (existingContents[d] < newPoint[d])
			{
				minBoundBox[d].min = existingContents[d];
				minBoundBox[d].max = newPoint[d];
			}
			else // new point < existing
			{
				minBoundBox[d].min = newPoint[d];
				minBoundBox[d].max = existingContents[d];
			}
		}
		return minBoundBox;
	}

	Region BDTree::minimumBoundingBox(const Region& existingContents, const Point& newPoint)
	{
		// Find min and max values for each dimension of point and region
		Region minBoundBox(existingContents.numDimensions(), Interval(0, 0));
		for (unsigned int d = 0; (d < existingContents.numDimensions()); d++)
		{
			// NOTE: Assumes max >= min for all dimension intervals of region
			minBoundBox[d].min = std::min(existingContents[d].min, newPoint[d]);
			minBoundBox[d].max = std::max(existingContents[d].max, newPoint[d]);
		}
		return minBoundBox;
	}

	std::ostream& operator<<(std::ostream& out, const BDTree::Node& node)
	{
		out << "(Type=" << node.type << ", Boundary=" << node.boundary << ", Point=" << node.point
			<< ", Parent=" << node.parent << ", LeftChild=" << node.leftChild
			<< ", RightChild=" << node.rightChild << ")";
		return out;
	}

}