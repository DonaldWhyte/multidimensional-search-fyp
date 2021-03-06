#include "KDTree.h"
#include <sstream>
#include <fstream>
#include <stack>
#include <map>

namespace mdsearch
{

	struct DFSEntry
	{
		KDNode* node;
		unsigned int level; // level in tree the node is

		DFSEntry(KDNode* node, unsigned int level) : node(node), level(level)
		{
		}
	};

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

	KDTree::KDTree(unsigned int numDimensions) :
		IndexStructure(numDimensions), root(NULL), totalPoints(0),
		avgInsertionCost(0), insertionOpCount(0), avgDeletionCost(0),
		deletionOpCount(0), avgQueryCost(0), queryOpCount(0)
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
		totalPoints = 0;
	}

	bool KDTree::insert(const Point& p)
	{
		#ifdef TRACK_OPERATION_COSTS  
			nodesVisitedInOp = 0;
		#endif

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
				totalPoints++;

				#ifdef TRACK_OPERATION_COSTS
					// Use number of nodes visited to update average cost
					avgInsertionCost = ((avgInsertionCost * static_cast<double>(insertionOpCount)) + nodesVisitedInOp) / (insertionOpCount + 1);
					insertionOpCount++;
				#endif

				return true;
			}
			else
			{
				#ifdef TRACK_OPERATION_COSTS 
					nodesVisitedInOp++;
				#endif

				// Duplicate point, it already exists! Cannot insert point
				if (p == current->point)
				{
					#ifdef TRACK_OPERATION_COSTS
						// Use number of nodes visited to update average cost
						avgInsertionCost = ((avgInsertionCost * static_cast<double>(insertionOpCount)) + nodesVisitedInOp) / (insertionOpCount + 1);
						insertionOpCount++;
					#endif

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
	}

	bool KDTree::remove(const Point& p)
	{
		#ifdef TRACK_OPERATION_COSTS  
			nodesVisitedInOp = 0;
		#endif

		bool removed = false;
		root = recursiveRemove(root, p, 0, &removed);

		#ifdef TRACK_OPERATION_COSTS
			// Use number of nodes visited to update average cost
			avgDeletionCost = ((avgDeletionCost * static_cast<double>(deletionOpCount)) + nodesVisitedInOp) / (deletionOpCount + 1);
			deletionOpCount++;
		#endif

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
		#ifdef TRACK_OPERATION_COSTS  
			nodesVisitedInOp = 0;
		#endif

		KDNode* current = root;
		unsigned int cuttingDim = 0;
		while (current) // until end of tree is reached
		{
			#ifdef TRACK_OPERATION_COSTS
				nodesVisitedInOp += 1;
			#endif

			if (p == current->point)
			{
				#ifdef TRACK_OPERATION_COSTS
					avgQueryCost = ((avgQueryCost * static_cast<double>(queryOpCount)) + nodesVisitedInOp) / (queryOpCount + 1);
					queryOpCount++;
				#endif

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

		#ifdef TRACK_OPERATION_COSTS
			avgQueryCost = ((avgQueryCost * static_cast<double>(queryOpCount)) + nodesVisitedInOp) / (queryOpCount + 1);
			queryOpCount++;
		#endif

		return false;
	}

	KDNode* KDTree::rootNode() const
	{
		return root;
	}

	unsigned int KDTree::numPointsStored() const
	{
		return totalPoints;
	}

	double KDTree::averageInsertionCost() const
	{
		return avgInsertionCost;
		}

	double KDTree::averageDeletionCost() const
	{
		return avgDeletionCost;
	}

	double KDTree::averageQueryCost() const
	{
		return avgQueryCost;
	}

	unsigned int KDTree::totalInsertionOps() const
	{
		return insertionOpCount;
	}

	unsigned int KDTree::totalDeletionOps() const
	{
		return deletionOpCount;
	}

	unsigned int KDTree::totalQueryOps() const
	{
		return queryOpCount;
	}

	KDTreeStats KDTree::computeStats() const
	{
		unsigned int levelSum = 0.0; // sum of each leaf's level
		unsigned int numLeaves = 0; // number of leaves in tree
		unsigned int maxPathLength = 0;

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
					if (entry.level > maxPathLength)
						maxPathLength = entry.level;
				}
				else
				{
					unvisitedNodes.push( DFSEntry(entry.node->leftChild, entry.level + 1) );
					unvisitedNodes.push( DFSEntry(entry.node->rightChild, entry.level + 1) );
				}
			}
		}

		KDTreeStats stats;
		stats.balanceFactor = static_cast<double>(levelSum) / static_cast<double>(std::max(1u, numLeaves));
		stats.maxPathLength = maxPathLength;
		return stats;
	}

	void KDTree::toHistogramFile(const std::string& filename) const
	{
		// Perform DFS to get to each leaf, counting edges traversed along the way
		std::map<unsigned int, unsigned int> lengthMap;
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
					if (lengthMap.find(entry.level) != lengthMap.end())
						lengthMap[entry.level] += 1;
					else // if this key has not been created yet, initialise to 1
						lengthMap[entry.level] = 1;
				}
				else
				{
					unvisitedNodes.push( DFSEntry(entry.node->leftChild, entry.level + 1) );
					unvisitedNodes.push( DFSEntry(entry.node->rightChild, entry.level + 1) );
				}
			}
		}

		std::ofstream file(filename.c_str());
		file << "Root-Leaf Path Length Distribution\n";
		// Write each 2D value to the file, in the same number o
		for (std::map<unsigned int, unsigned int>::const_iterator it = lengthMap.begin();
			(it != lengthMap.end()); it++)
		{
			for (unsigned int i = 0; (i < it->second); i++)
			{
				file << it->first << "\n";
			}
		}
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
			#ifdef TRACK_OPERATION_COSTS
				nodesVisitedInOp++;
			#endif

			if (node->leftChild == NULL) // if no more 
				return &node->point;
			else
			{
				return findMinimum(node->leftChild, dimension, nextCuttingDimension(cuttingDim, numDimensions));
			}
		}
		// Otherwise, we have to search BOTH children
		else
		{
			#ifdef TRACK_OPERATION_COSTS
				nodesVisitedInOp++;
			#endif

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
		else
		{
			#ifdef TRACK_OPERATION_COSTS
				nodesVisitedInOp++;
			#endif
	
			if (p[cuttingDim] < node->point[cuttingDim])
			{
				node->leftChild = recursiveRemove(node->leftChild, p,
					nextCuttingDimension(cuttingDim, numDimensions), removed);
			}
			else if (p == node->point) // may have found node that stores given point (TODO: wrong w/o/ equality)
			{

				// If node with point is leaf node, simply delete it!
				if (node->leftChild == NULL && node->rightChild == NULL)
				{
					*removed = true; // now we can set the 'removed' flag to true as removal has been successful
					totalPoints--;
					delete node;
					return NULL; // to remove reference to node in parent
				}
				else if (node->rightChild)
				{
					// Find minimum point for cutting dimension and REPLACE node's point with it
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
			else // right child!
			{
				node->rightChild = recursiveRemove(node->rightChild, p,
					nextCuttingDimension(cuttingDim, numDimensions), removed);
			}

			// If this point is reached, node should not be removed so we just return the node
			return node;
		}
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