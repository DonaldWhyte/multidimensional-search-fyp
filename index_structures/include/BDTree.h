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
			LEAF_BOX_NODE
		};

		struct Node
		{
			Node(NodeType type);
			virtual ~Node();

			virtual bool contains(const Point& p) = 0;

			NodeType type;
		};

		struct SplitNode : public Node
		{
			SplitNode();
			virtual ~SplitNode();
			bool contains(const Point& p);

			Region innerBox; // overall boundary of split node
			Node* leftChild;
			Node* rightChild; 
		};

		struct ShrinkNode : public Node
		{
			ShrinkNode();
			virtual ~ShrinkNode();
			bool contains(const Point& p);

			SplitNode* innerChild; // containers inner region
			Node* outerChild; // contains outer region
			// NOTE: use both to compute set-theoretic difference
			// of two to get outerChild's actual region
		};		

		struct PointNode : public Node
		{
			PointNode(const Point& point);
			bool contains(const Point& p);

			Point point;
		};

		struct InnerBoxNode : public Node
		{
			InnerBoxNode(const Region& boxRegion);
			bool contains(const Point& p);

			Region innerBox;
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
		SplitNode* split(Node* node);
		/* Partition node region into two children. The left child contains
		 * the node's inner box and the right is the set theoretic different
		 * of the left node's shrunk region and the original node's region
		 * (this is known as the outer box). */
		ShrinkNode* shrink(Node* node);
		/* Perform a Splay quadtree promote(x) operation on the given node.
		 * Return true if the node was successfully promoted and false
		 * if it wasn't (due to it already being the root). */
		bool promote(Node* node);

		Node* root;

	};

}

#endif