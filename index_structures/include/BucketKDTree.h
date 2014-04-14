#ifndef MDSEARCH_BUCKETKDTREE_H
#define MDSEARCH_BUCKETKDTREE_H

#include "IndexStructure.h"

namespace mdsearch
{

	struct BucketKDNode
	{
		enum NodeType
		{
			TYPE_LEAF = 0,
			TYPE_SPLIT
		};

		NodeType type;
		BucketKDNode* parent;
		// For leaves
		PointList points;
		// For non-leaves
		int cuttingDim;
		Real cuttingVal;
		BucketKDNode* leftChild;
		BucketKDNode* rightChild;

		/* Constructs empty leaf node. */
		BucketKDNode(BucketKDNode* parent);
		/* Constructs split node. */
		BucketKDNode(BucketKDNode* parent, int cuttingDim, Real cuttingVal,
			BucketKDNode* leftChild, BucketKDNode* rightChild);

		virtual ~BucketKDNode();
	};

	class BucketKDTree : public IndexStructure
	{

	public:
		/* Maximum number of nodes allowed to be stored in a leaf node.
		 * If the number of poiints in a leaf exceeds this number, the
		 * leaf is split. */
		static const int MAX_POINTS_PER_LEAF = 4;

		BucketKDTree(unsigned int numDimensions);
		virtual ~BucketKDTree();

		virtual void clear();
		virtual bool insert(const Point& p);
		virtual bool remove(const Point& p);
		virtual bool update(const Point& oldPoint, const Point& newPoint);
		virtual bool pointExists(const Point& p);

		BucketKDNode* rootNode() const;

		virtual std::string toString() const;

	private:
		BucketKDNode* findLeafForPoint(const Point& p) const;
		bool containsPoint(const BucketKDNode* leaf, const Point& p) const;

		std::string nodeToString(const std::string& title,
			const BucketKDNode* node, unsigned int level = 0) const;

		BucketKDNode* root;


	};

}

#endif