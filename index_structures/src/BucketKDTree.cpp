#include "BucketKDTree.h"
#include <sstream>

namespace mdsearch
{

	typedef void (*SplittingFunction)(const PointList&, int&, int&);

	void rangeMidpointSplit(const PointList& points, int& cuttingDim, int& cuttingVal)
	{
		// Find dimension with highest range
		Region minBox = Region::minimumBoundingBox(points);
		cuttingDim = 0;
		Real cuttingDimRange = minBox[0].max - minBox[0].min;
		for (unsigned int d = 1; (d < minBox.numDimensions()); d++)
		{
			Real range = minBox[d].max - minBox[d].min;
			if (range > cuttingDimRange)
			{
				cuttingDim = d;
				cuttingDimRange = range;
			}
		}
		// Get midpoint of this range and use that as the cutting value
		cuttingVal = minBox[cuttingDim].min + (cuttingDimRange / 2.0f);
	}

	BucketKDNode::BucketKDNode(BucketKDNode* parent) : type(BucketKDNode::TYPE_LEAF),
		parent(parent), cuttingDim(0), cuttingVal(0), leftChild(NULL), rightChild(NULL)
	{
	}

	BucketKDNode::BucketKDNode(BucketKDNode* parent, int cuttingDim,
		Real cuttingVal, BucketKDNode* leftChild, BucketKDNode* rightChild) :
		type(BucketKDNode::TYPE_SPLIT), parent(parent),
		cuttingDim(cuttingDim), cuttingVal(cuttingVal),
		leftChild(leftChild), rightChild(rightChild)
	{
	}

	BucketKDNode::~BucketKDNode()
	{
		delete leftChild;
		delete rightChild;
	}

	BucketKDTree::BucketKDTree(unsigned int numDimensions) :
		IndexStructure(numDimensions),
		root(new BucketKDNode(NULL))
	{
	}

	BucketKDTree::~BucketKDTree()
	{
		delete root;
	}

	void BucketKDTree::clear()
	{
		delete root;
		root = new BucketKDNode(NULL);
	}

	bool BucketKDTree::insert(const Point& p)
	{
		BucketKDNode* leaf = findLeafForPoint(p);
		// NOTE: Assuming a non-NULL value is ALWAYS returned, so no pointer check!
		if (containsPoint(leaf, p))
		{
			return false; // cannot insert duplicate!
		}
		else
		{
			// Not a duplicate, add to the structure
			leaf->points.push_back(p);
			// If bucket leaf has exceeded max capacity, split it into two nodes
			if (leaf->points.size() > MAX_POINTS_PER_LEAF)
			{
				// Compute cutting dimension and value to use
				int cuttingDim, cuttingVal;
				rangeMidpointSplit(leaf->points, cuttingDim, cuttingVal);
				// Construct empty left and right children and fill them
				// with the points from this node
				BucketKDNode* leftChild = new BucketKDNode(NULL);
				BucketKDNode* rightChild = new BucketKDNode(NULL);
				for (PointList::const_iterator it = leaf->points.begin();
					(it != leaf->points.end()); it++)
				{
					if ((*it)[cuttingDim] < cuttingVal)
						leftChild->points.push_back(*it);
					else
						rightChild->points.push_back(*it);
				}
				// Construct split node to contain the two children
				BucketKDNode* splitNode = new BucketKDNode(leaf->parent,
					cuttingDim, cuttingVal, leftChild, rightChild);
				leftChild->parent = splitNode;
				rightChild->parent = splitNode;
				// Re-assign leaf's parent's pointer to point to split node
				if (leaf->parent)
				{
					if (leaf->parent->leftChild == leaf)
						leaf->parent->leftChild = splitNode;
					else 
						leaf->parent->rightChild = splitNode;
				}
				else // if no parent, it's the root!
				{
					root = splitNode;
				}
				// Delete the leaf node as it's no longer needed
				delete leaf;
			}
			return true;
		}
	}

	bool BucketKDTree::remove(const Point& p)
	{
		// TODO
		return false;
	}

	bool BucketKDTree::update(const Point& oldPoint, const Point& newPoint)
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

	bool BucketKDTree::pointExists(const Point& p)
	{
		return containsPoint(findLeafForPoint(p), p);
	}

	BucketKDNode* BucketKDTree::rootNode() const
	{
		return root;
	}

	BucketKDNode* BucketKDTree::findLeafForPoint(const Point& p) const
	{
		BucketKDNode* current = root;
		while (current) // until end of tree is reached
		{
			if (current->type == BucketKDNode::TYPE_SPLIT)
			{
				if (p[current->cuttingDim] < current->cuttingVal)
					current = current->leftChild;
				else
					current = current->rightChild;
			}
			else // leaf node
			{
				return current;
			}
		}
		return NULL; // shouldn't happen, since a leaf node should always be found		
	}

	bool BucketKDTree::containsPoint(const BucketKDNode* leaf, const Point& p) const
	{
		// Search contents of leaf node for point
		for (PointList::const_iterator it = leaf->points.begin();
			(it != leaf->points.end()); it++)
		{
			if (p == *it)
				return true;
		}
		return false;
	}	

	std::string BucketKDTree::toString() const
	{
		return nodeToString("ROOT", root);
	}

	std::string BucketKDTree::nodeToString(const std::string& title, const BucketKDNode* node, unsigned int level) const
	{
		std::stringstream ss;
		for (unsigned int i = 0; (i < level); i++)
			ss << "\t";
		ss << title << "-> ";
		if (!node)
		{
			ss << "()\n";
		}
		else if (node->type == BucketKDNode::TYPE_SPLIT)
		{
			ss << "Cutting Dimension: " << node->cuttingDim <<
				", Cutting Value: " << node->cuttingVal << "\n";
			ss << nodeToString("LEFT", node->leftChild, level + 1);
			ss << nodeToString("RIGHT", node->rightChild, level + 1);	
		}
		else // leaf node
		{
			ss << "Leaf with " << node->points.size() << " points:\n";
			for (PointList::const_iterator it = node->points.begin();
				(it != node->points.end()); it++)
			{
				for (unsigned int i = 0; (i < level + 1); i++) // ensure correct indenting
					ss << "\t";
				ss << *it << "\n";
			}
		}
		return ss.str();
	}

}