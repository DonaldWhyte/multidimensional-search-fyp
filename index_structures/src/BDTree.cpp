#include "BDTree.h"

namespace mdsearch
{

	BDTree::Node::Node(BDTree::NodeType type) : type(type) { }
	BDTree::Node::~Node() { }

	BDTree::SplitNode::SplitNode() : Node(BDTree::SPLIT_NODE) { }
	BDTree::SplitNode::~SplitNode()
	{
		delete leftChild;
		delete rightChild;
	}
	bool BDTree::SplitNode::contains(const Point& p)
	{
		return (leftChild->contains(p) || rightChild->contains(p));
	}

	BDTree::ShrinkNode::ShrinkNode() : Node(BDTree::SHRINK_NODE) { }
	BDTree::ShrinkNode::~ShrinkNode()
	{
		delete innerChild;
		delete outerChild;
	}
	bool BDTree::ShrinkNode::contains(const Point& p)
	{
		return (innerChild->contains(p) || outerChild->contains(p));
	}

	BDTree::PointNode::PointNode(const Point& point) :
		Node(BDTree::LEAF_POINT_NODE), point(point) { }
	bool BDTree::PointNode::contains(const Point& p)
	{
		return (p == point);
	}

	BDTree::InnerBoxNode::InnerBoxNode(const Region& boxRegion) :
		 Node(BDTree::LEAF_BOX_NODE), innerBox(boxRegion) { }
	bool BDTree::InnerBoxNode::contains(const Point& p)
	{
		return innerBox.contains(p);
	}

	BDTree::BDTree(unsigned int numDimensions, const Region& boundary) :
		IndexStructure(numDimensions), root(NULL)
	{
		clear();
	}

	void BDTree::clear()
	{
		// Simply delete the root node and replace it with a new rooy
		delete root;
		// Construct empty inner box node for root
		root = new InnerBoxNode(boundary);
	}

	bool BDTree::insert(const Point& p)
	{
		// TODO: find leaf node u that contains p


		// TODO: let b be u's contents (left node??) and C be u's outer box

		// TODO: compute minimum quadtree box E that encloses both p and b

		// TODO: split E to produce two nodes v and w
		// v is a node which shrinks from u's outer box to E -- THIS REPLACES u!
		// w is an INNER CHILD OF V, which is a split node that separates b from p
		// by inner-left-convention, the LEFT CHILD of w contains b and the right child contains p

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
		// TODO
		return false;
	}

	PointList BDTree::pointsInRegion(const Region& region)
	{
		// TODO
		return PointList();
	}

	BDTree::SplitNode* BDTree::split(BDTree::Node* node)
	{
		// TODO
		return NULL;
	}

	BDTree::ShrinkNode* BDTree::shrink(BDTree::Node* node)
	{
		// TODO
		return NULL;
	}

	bool BDTree::promote(BDTree::Node* node)
	{
		// TODO
	}

}