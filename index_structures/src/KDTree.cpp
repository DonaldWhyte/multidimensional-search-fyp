#include "KDTree.h"
#include <sstream>

namespace mdsearch
{

	inline unsigned int nextCuttingDimension(unsigned int cuttingDim, unsigned int numDimensions)
	{
		return (cuttingDim + 1) % numDimensions;
	}

	KDNode::KDNode(const Point& p) : point(p), leftChild(NULL), rightChild(NULL)
	{
	}

	KDNode::~KDNode()
	{
		delete leftChild;
		delete rightChild;
	}

	KDTree::KDTree(unsigned int numDimensions) : IndexStructure(numDimensions), root(NULL)
	{
	}

	KDTree::~KDTree()
	{
		delete root;
	}

	void KDTree::clear()
	{
		delete root;
		root = NULL;
	}

	bool KDTree::insert(const Point& p)
	{
		KDNode* previous = NULL; // previous node traversed
		bool leftChildOfPrevious = false; // set to true if 'current' is left child of 'previous'
		KDNode* current = root;
		unsigned int cuttingDim = 0;

		while (true) // loop util true/false returned - guaranteed to terminate eventually!
		{
			if (current == NULL)
			{
				current = new KDNode(p);
				if (previous) // assign parent's correct child pointer to new node
				{
					if (leftChildOfPrevious)
						previous->leftChild = current;
					else
						previous->rightChild = current;
				}
				else // if no parent, then ROOT NODE WAS INSERTED. Update root!
				{
					root = current;
				}
				return true;
			}
			// Duplicate point, it already exists! Cannot insert point
			else if (p == current->point)
			{
				return false;
			}
			else if (p[cuttingDim] < current->point[cuttingDim])
			{
				previous = current;
				current = current->leftChild;
				leftChildOfPrevious = true;
			}
			else
			{
				previous = current;
				current = current->rightChild;
				leftChildOfPrevious = false;
			}
			cuttingDim = nextCuttingDimension(cuttingDim, numDimensions);
		}
	}

	bool KDTree::remove(const Point& p)
	{
		bool removed = false;
		root = recursiveRemove(root, p, 0, &removed);
		return removed;
	}

	bool KDTree::update(const Point& oldPoint, const Point& newPoint)
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

	bool KDTree::pointExists(const Point& p)
	{
		KDNode* current = root;
		unsigned int cuttingDim = 0;
		while (current) // until end of tree is reached
		{
			if (p == current->point)
			{
				return true;
			}
			else if (p[cuttingDim] < current->point[cuttingDim])
			{
				current = current->leftChild;
			}
			else
			{
				current = current->rightChild;
			}
			cuttingDim = nextCuttingDimension(cuttingDim, numDimensions);
		}
		return false;
	}

	KDNode* KDTree::rootNode() const
	{
		return root;
	}

	const Point* KDTree::findMinimum(KDNode* node, unsigned int dimension, unsigned int cuttingDim)
	{
		// Reached leaf node
		if (node == NULL)
		{
			return NULL;
		}
		// If cutting dimension is dimension we're looking for minimum in, just search left child!
		else if (dimension == cuttingDim)
		{
			if (node->leftChild == NULL) // if no more 
				return &node->point;
			else
				return findMinimum(node->leftChild, dimension, nextCuttingDimension(cuttingDim, numDimensions));
		}
		// Otherwise, we have to search BOTH children
		else
		{
			const Point* a = findMinimum(node->leftChild, dimension, nextCuttingDimension(cuttingDim, numDimensions));
			const Point* b = findMinimum(node->rightChild, dimension, nextCuttingDimension(cuttingDim, numDimensions));
			if (a && b) // if minimums were returned from both children
			{
				Real minVal = std::min(node->point[dimension], std::min((*a)[dimension], (*b)[dimension]));
				if (minVal == node->point[dimension])
				{
					return &node->point;
				}
				else if (minVal == (*a)[dimension])
					return a;
				else
					return b;
			}
			else if (a) // if minimum was just returned from left child
			{
				Real minVal = std::min(node->point[dimension], (*a)[dimension]);
				if (minVal == node->point[dimension])
					return &node->point;
				else
					return a;
			}
			else if (b) // if minimum was just returned from right child
			{
				Real minVal = std::min(node->point[dimension], (*b)[dimension]);
				if (minVal == node->point[dimension])
					return &node->point;
				else
					return b;
			}
			else // no minimums returned!
			{
				return &node->point;
			}

		}
	}

	KDNode* KDTree::recursiveRemove(KDNode* node, const Point& p, unsigned int cuttingDim, bool* removed)
	{
		if (node == NULL)
		{
			return NULL;
		}
		else if (p[cuttingDim] < node->point[cuttingDim])
		{
			node->leftChild = recursiveRemove(node->leftChild, p,
				nextCuttingDimension(cuttingDim, numDimensions), removed);
		}
		else if (p[cuttingDim] > node->point[cuttingDim])
		{
			node->rightChild = recursiveRemove(node->rightChild, p,
				nextCuttingDimension(cuttingDim, numDimensions), removed);
		}
		else // found node that stores given point
		{
			// If node with point is leaf node, simply delete it!
			if (node->leftChild == NULL && node->rightChild == NULL)
			{
				*removed = true; // now we can set the 'removed' flag to true as removal has been successful
				delete node;
				return NULL; // to remove reference to node in parent
			}
			else
			{
				// Find minimum point for cutting dimension and REPLACE node's point with it
				if (node->rightChild)
				{
					node->point = *findMinimum(node->rightChild, cuttingDim,
						nextCuttingDimension(cuttingDim, numDimensions));
					node->rightChild = recursiveRemove(node->rightChild, node->point,
						nextCuttingDimension(cuttingDim, numDimensions), removed);
				}
				else // if there is no right child!!
				{
					node->point = *findMinimum(node->leftChild, cuttingDim,
						nextCuttingDimension(cuttingDim, numDimensions));
					node->leftChild = recursiveRemove(node->leftChild, node->point,
						nextCuttingDimension(cuttingDim, numDimensions), removed);
					// Swap left child with right child
					node->rightChild = node->leftChild;
					node->leftChild = NULL;
				}
			}
		}
		// If this point is reached, node should not be removed so we
		// just return the node
		return node;		
	}

	std::string KDTree::toString() const
	{
		return nodeToString("ROOT:", root, 0);
	}

	std::string KDTree::nodeToString(const std::string& title, const KDNode* node, unsigned int cuttingDim, unsigned int level) const
	{
		std::stringstream ss;
		for (unsigned int i = 0; (i < level); i++)
			ss << "\t";
		ss << title << " ";
		if (!node)
		{
			ss << "()\n";
		}
		else
		{
			ss << node->point << ", Cutting Dim: " << cuttingDim << "\n";
			int nextCD = nextCuttingDimension(cuttingDim, numDimensions);
			ss << nodeToString("LEFT:", node->leftChild, nextCD, level + 1);
			ss << nodeToString("RIGHT:", node->rightChild, nextCD, level + 1);	
		}
		return ss.str();
	}

}