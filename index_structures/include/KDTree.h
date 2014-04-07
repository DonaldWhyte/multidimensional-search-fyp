#ifndef MDSEARCH_KDTREE_H
#define MDSEARCH_KDTREE_H

#include "IndexStructure.h"

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

		/* Find point with minimum value of a specific dimension in subtree
		 * rooted at given node. */
		const Point* findMinimum(KDNode* node, unsigned int dimension, unsigned int cuttingDim = 0);
		/* Find point with maximum value of a specific dimension in subtree
		 * rooted at given node. */
		const Point* findMaximum(KDNode* node, unsigned int dimension, unsigned int cuttingDim = 0);

		virtual std::string toString() const;

	private:
		KDNode* recursiveRemove(KDNode* node, const Point& p,
			unsigned int cuttingDim, bool* removed);

		std::string nodeToString(const std::string& title, const KDNode* node,
			unsigned int cuttingDim, unsigned int level = 0) const;

		KDNode* root;


	};

}

#endif