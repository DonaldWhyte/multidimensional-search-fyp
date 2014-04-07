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

		virtual std::string toString() const;

	private:
		KDNode* recursiveInsert(const Point& p, KDNode* t, unsigned int cuttingDim);
		std::string nodeToString(const std::string& title, const KDNode* node,
			unsigned int cuttingDim, unsigned int level = 0) const;

		KDNode* root;


	};

}

#endif