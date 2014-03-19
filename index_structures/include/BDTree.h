#ifndef MDSEARCH_BDTREE_H
#define MDSEARCH_BDTREE_H

#include "IndexStructure.h"

namespace mdsearch
{

	enum NodeRelation
	{
		NODE_RELATION_NONE = 0,
		NODE_RELATION_LEFT,
		NODE_RELATION_RIGHT,
		NODE_RELATION_OUTER
	};

	class BDTree : public IndexStructure
	{

	public:
		enum NodeType
		{
			EMPTY_NODE = 0,
			SPLIT_NODE,
			SHRINK_NODE,
			LEAF_POINT_NODE,
			LEAF_BOX_NODE,
			OUTER_NODE
		};

		struct Node
		{
			Node(NodeType type, Node* parent, const Region& boundary, const Point& point = Point(0));
			virtual ~Node();

			bool contains(const Point& p);

			NodeType type;
			Node* parent; // NULL if node is root
			Node* leftChild;
			Node* rightChild; 

			Region boundary;
			Point point;
		};

		BDTree(unsigned int numDimensions, const Region& boundary);

		virtual void clear();

		virtual bool insert(const Point& p);
		virtual bool remove(const Point& p);
		virtual bool update(const Point& oldPoint, const Point& newPoint);

		virtual bool pointExists(const Point& p);
		virtual PointList pointsInRegion(const Region& region);

	private:
		/* Partition node region using orthogonal hyperlane to its longest size,
		 * creating two children. */
		Node* split(Node* parentShrinkNode, const Point& newPoint);
		/* Partition node region into two children. The left child contains
		 * the node's inner box and the right is the set theoretic different
		 * of the left node's shrunk region and the original node's region
		 * (this is known as the outer box). */
		void shrink(Node* node, const Point& newPoint);
		/* Perform a Splay quadtree promote(x) operation on the given node.
		 * Return true if the node was successfully promoted and false
		 * if it wasn't (due to it already being the root). */
		bool promote(Node* node);
		/* Compute minimum boundary that surrounds the existing contents of
		 * BD-tree node and a new point. */
		Region minimumBoundingBox(const Point& existingContents, const Point& newPoint);
		Region minimumBoundingBox(const Region& existingContents, const Point& newPoint);

		Node* root;
		Region boundary; // overall boundary of space the tree contains

	};

}

#endif