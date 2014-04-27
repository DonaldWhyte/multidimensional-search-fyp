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
		IndexList points;
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
		BucketKDTree(unsigned int numDimensions, int maxPointsUntilSplit, int minPointsUntilMerge);
		virtual ~BucketKDTree();

		virtual void clear();
		virtual bool insert(const Point& p);
		virtual bool remove(const Point& p);
		virtual bool update(const Point& oldPoint, const Point& newPoint);
		virtual bool pointExists(const Point& p);

		double computeBalanceFactor() const;
		BucketKDNode* rootNode() const;

		virtual std::string toString() const;

	private:
		BucketKDNode* findLeafForPoint(const Point& p) const;
		BucketKDNode* findLeafForPoint(const Point& p, int& level) const; // returns level of tree leaf is on too
		int getPointIndexInBucket(const IndexList& bucketIndices, const Point& p) const;
		bool containsPoint(const BucketKDNode* leaf, const Point& p) const;
		void mergeSiblingLeaves(BucketKDNode* leaf, BucketKDNode* sibling);

		std::string nodeToString(const std::string& title,
			const BucketKDNode* node, unsigned int level = 0) const;

		PointList allPoints;
		RealList allPointSums;
		BucketKDNode* root;
		/* Maximum number of nodes allowed to be stored in a leaf node.
		 * If the number of points in a leaf exceeds this number, the
		 * leaf is split. */
		int maxPointsUntilSplit;
		/* Minimum total number of points two LEAF children of a node can
		 * store before merging the two children into a single leaf. */
		int minPointsUntilMerge;


	};

}

#endif