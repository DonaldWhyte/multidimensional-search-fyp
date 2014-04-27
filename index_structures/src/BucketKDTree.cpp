#include "BucketKDTree.h"
#include "Util.h"
#include <sstream>
#include <stack>
#include <map>

namespace mdsearch
{

	struct DFSEntry
	{
		BucketKDNode* node;
		unsigned int level; // level in tree the node is

		DFSEntry(BucketKDNode* node, unsigned int level) : node(node), level(level)
		{
		}
	};

	typedef void (*SplittingFunction)(const PointList&, const IndexList&, int&, int&);

	void rangeMidpointSplit(const PointList& allPoints, const IndexList& indices,
		int& cuttingDim, int& cuttingVal)
	{
		// Find dimension with highest range
		Region minBox = Region::minimumBoundingBox(allPoints, indices);
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

	inline Real computeVariance(const PointList& allPoints, const IndexList& indices, int d)
	{
		Real sum1 = 0;
		for (IndexList::const_iterator it = indices.begin(); (it != indices.end()); it++)
			sum1 += allPoints[*it][d];
		Real mean = sum1 / indices.size();

		Real sum2 = 0;
		Real sum3 = 0;
		for (IndexList::const_iterator it = indices.begin(); (it != indices.end()); it++)
		{
			sum3 += (allPoints[*it][d] - mean);
			sum2 += sum3 * sum3;
		}
		return (sum2 - (sum3 * sum3) / indices.size()) / (indices.size() - 1);
	}

	inline Real computeMean(const PointList& allPoints, const IndexList& indices, int d)
	{
		Real total = 0;
		for (IndexList::const_iterator it = indices.begin(); (it != indices.end()); it++)
			total += allPoints[*it][d];		
		return total / indices.size();
	}

	void meanSplit(const PointList& allPoints, const IndexList& indices,
		int& cuttingDim, int& cuttingVal)
	{
		// Compute variance of FIRST dimension
		cuttingDim = 0;
		Real cuttingDimVariance = computeVariance(allPoints, indices, 0);
		// Compute variance of 
		for (unsigned int d = 1; (d < allPoints[indices[0]].numDimensions()); d++)
		{
			Real variance = computeVariance(allPoints, indices, d);
			if (variance > cuttingDimVariance)
			{
				cuttingDim = d;
				cuttingDimVariance = variance;
			}
		}
		// Use mean of dimension as cutting value
		cuttingVal = computeMean(allPoints, indices, cuttingDim);
	}

	/* Return pointer to node's sibling.
	 * NOTE: Assumes node has a parent -- does not check if parent
	 * pointer is NULL. */
	inline BucketKDNode* getSibling(BucketKDNode* node)
	{
		if (node->parent->leftChild == node)
			return node->parent->rightChild;
		else
			return node->parent->leftChild;
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

	BucketKDTree::BucketKDTree(unsigned int numDimensions,
		int maxPointsUntilSplit, int minPointsUntilMerge) :
		IndexStructure(numDimensions),
		root(new BucketKDNode(NULL)),
		maxPointsUntilSplit(maxPointsUntilSplit),
		minPointsUntilMerge(minPointsUntilMerge)
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
		int level;
		BucketKDNode* leaf = findLeafForPoint(p, level);
		// NOTE: Assuming a non-NULL value is ALWAYS returned, so no pointer check!
		if (containsPoint(leaf, p))
		{
			return false; // cannot insert duplicate!
		}
		else
		{
			// Not a duplicate, add to the structure
			allPoints.push_back(p);
			allPointSums.push_back(p.sum());
			leaf->points.push_back(allPoints.size() - 1);
			// If bucket leaf has exceeded max capacity, split it into two nodes
			if (leaf->points.size() > maxPointsUntilSplit)
			{
				// Compute range in chosen dimension and split by half
				int cuttingDim = level % numDimensions;
				Real min, max;
				min = max = allPoints[leaf->points[0]][cuttingDim];
				for (IndexList::const_iterator it = leaf->points.begin();
					(it != leaf->points.end()); it++)
				{
					if (allPoints[*it][cuttingDim] < min)
						min = allPoints[*it][cuttingDim];
					else if (allPoints[*it][cuttingDim] > max)
						max = allPoints[*it][cuttingDim];
				}
				Real cuttingVal = (min + (max - min)) / 2.0f;

				// Construct empty left and right children and fill them
				// with the points from this node
				BucketKDNode* leftChild = new BucketKDNode(NULL);
				BucketKDNode* rightChild = new BucketKDNode(NULL);
				for (IndexList::const_iterator it = leaf->points.begin();
					(it != leaf->points.end()); it++)
				{
					if (allPoints[*it][cuttingDim] < cuttingVal)
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
		BucketKDNode* leaf = findLeafForPoint(p);
		// If element was present in leaf and was deleted
		int index = getPointIndexInBucket(leaf->points, p);
		if (index != -1)
		{
			removeElementAtIndex(leaf->points, index);
			if (leaf->parent) // if not root node
			{
				// If sibling is also a leaf, and the total points
				// contained in both leaf nodes is lower than minimum,
				// merge the two nodes together!
				BucketKDNode* sibling = getSibling(leaf);
				if ((leaf->points.size() + sibling->points.size()) < minPointsUntilMerge)
					mergeSiblingLeaves(leaf, sibling);
			}

			return true;
		}
		else
		{
			return false;
		}
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

	BucketKDNode* BucketKDTree::findLeafForPoint(const Point& p, int& level) const
	{
		level = 0;
		BucketKDNode* current = root;
		while (current)
		{
			if (current->type == BucketKDNode::TYPE_SPLIT)
			{
				if (p[current->cuttingDim] < current->cuttingVal)
					current = current->leftChild;
				else
					current = current->rightChild;
			}
			else
			{
				return current;
			}
			++level;
		}
		return NULL;
	}

	int BucketKDTree::getPointIndexInBucket(const IndexList& bucketIndices, const Point& p) const
	{
		for (unsigned int i = 0; (i < bucketIndices.size()); i++)
		{
			int actualIndex = bucketIndices[i];
			if (p.sum() == allPointSums[actualIndex] && p == allPoints[actualIndex])
				return i;
		}
		return -1;
	}	

	bool BucketKDTree::containsPoint(const BucketKDNode* leaf, const Point& p) const
	{
		// Search contents of leaf node for point
		for (IndexList::const_iterator it = leaf->points.begin();
			(it != leaf->points.end()); it++)
		{
			if (p.sum() == allPointSums[*it] && p == allPoints[*it])
				return true;
		}
		return false;
	}

	void BucketKDTree::mergeSiblingLeaves(BucketKDNode* leaf, BucketKDNode* sibling)
	{
		// Transform parent node into a leaf and copy all points to it
		leaf->parent->type = BucketKDNode::TYPE_LEAF;
		leaf->parent->points.reserve(leaf->points.size() + sibling->points.size());
		for (IndexList::const_iterator it = leaf->points.begin(); (it != leaf->points.end()); it++)
			leaf->parent->points.push_back(*it);
		for (IndexList::const_iterator it = sibling->points.begin(); (it != sibling->points.end()); it++)
			leaf->parent->points.push_back(*it);
		// Remove references to children and free memory used by two leaves
		leaf->parent->leftChild = NULL;
		leaf->parent->rightChild = NULL;
		delete leaf;
		delete sibling;
	}

	double BucketKDTree::computeBalanceFactor() const
	{
		unsigned int levelSum = 0.0; // sum of each leaf's level
		unsigned int numLeaves = 0; // number of leaves in tree

		// Perform DFS to get to each leaf, counting edges traversed along the way
		std::stack<DFSEntry> unvisitedNodes;
		unvisitedNodes.push( DFSEntry(root, 0) );
		while (!unvisitedNodes.empty())
		{
			DFSEntry entry = unvisitedNodes.top();
			unvisitedNodes.pop();
			if (entry.node != NULL) // ignore NULL nodes
			{
				if (entry.node->leftChild == NULL && entry.node->rightChild == NULL) // if node is leaf
				{
					levelSum += entry.level;
					numLeaves += 1;
				}
				else
				{
					unvisitedNodes.push( DFSEntry(entry.node->leftChild, entry.level + 1) );
					unvisitedNodes.push( DFSEntry(entry.node->rightChild, entry.level + 1) );
				}
			}
		}

		return static_cast<double>(levelSum) / static_cast<double>(std::max(1u, numLeaves));
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
			for (IndexList::const_iterator it = node->points.begin();
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