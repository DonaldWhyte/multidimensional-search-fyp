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

	KDTree::KDTree(unsigned int numDimensions) : IndexStructure(numDimensions), root(NULL)
	{
		// TODO
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
		bool leftChildOfPrevious = false; // TODO
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
		// TODO
		return false;
	}

	bool KDTree::update(const Point& oldPoint, const Point& newPoint)
	{
		// TODO
		return false;
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

	std::string KDTree::toString() const
	{
		return nodeToString("ROOT:", root, 0);
	}

	KDNode* KDTree::recursiveInsert(const Point& p, KDNode* node, unsigned int cuttingDim)
	{

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