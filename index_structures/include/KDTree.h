#ifndef MDSEARCH_KDTREE_H
#define MDSEARCH_KDTREE_H

#include "IndexStructure.h"

/* Only define this if you want the kd-tree to internally keep
 * track of the average cost of each operation performed on it.
 * This adds a small amount of overhead, so only enable this
 * feature when necessary. */
#define TRACK_OPERATION_COSTS 1

namespace mdsearch
{

	struct KDNode
	{
		Point point;
		KDNode* leftChild;
		KDNode* rightChild;

		KDNode(const Point& p);
		virtual ~KDNode();
	};

	class KDTree : public IndexStructure
	{

	public:
		KDTree(unsigned int numDimensions);
		virtual ~KDTree();

		virtual void clear();
		virtual bool insert(const Point& p);
		virtual bool remove(const Point& p);
		virtual bool update(const Point& oldPoint, const Point& newPoint);
		virtual bool pointExists(const Point& p);

		KDNode* rootNode() const;
		unsigned int numPointsStored() const;
		double averageInsertionCost() const;
		double averageDeletionCost() const;
		double averageQueryCost() const;
		unsigned int totalInsertionOps() const;
		unsigned int totalDeletionOps() const;
		unsigned int totalQueryOps() const;

		double computeBalanceFactor() const;
		void toHistogramFile(const std::string& filename) const;

		/* Find point with minimum value of a specific dimension in subtree
		 * rooted at given node. */
		const Point* findMinimum(KDNode* node, unsigned int dimension, unsigned int cuttingDim = 0);

		virtual std::string toString() const;

	private:
		KDNode* recursiveRemove(KDNode* node, const Point& p,
			unsigned int cuttingDim, bool* removed);

		std::string nodeToString(const std::string& title, const KDNode* node,
			unsigned int cuttingDim, unsigned int level = 0) const;

		KDNode* root;
		unsigned int totalPoints; // also means total nodes in tree
		// Average costs of the three supported operations
		double avgInsertionCost;
		unsigned int insertionOpCount;
		double avgDeletionCost;
		unsigned int deletionOpCount;
		double avgQueryCost;
		unsigned int queryOpCount;
		// Temporary field used to compute cost of single operation
		// as it is executing
		unsigned int nodesVisitedInOp;


	};

}

#endif