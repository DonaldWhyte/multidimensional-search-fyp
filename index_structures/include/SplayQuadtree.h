#ifndef MDSEARCH_SPLAYQUADTREE_H
#define MDSEARCH_SPLAYQUADTREE_H

#include "IndexStructure.h"
#include "Region.h"
#include "Point.h"

namespace mdsearch
{

	class SplayQuadtree : public IndexStructure
	{

	public:
		enum NodeType
		{
			EMPTY_LEAF_NODE = 0,
			FILLED_LEAF_NODE,
			SHRINKSPLIT_NODE
		};

		enum NodeRelation
		{
			NO_RELATION = 0,
			LEFT_CHILD_RELATION,
			RIGHT_CHILD_RELATION,
			OUTER_CHILD_RELATION,
		};

		struct Node
		{
			Node* parent;
			NodeType type;
			// Region of space the node covers
			Region outerBox;

			Node(NodeType type, const Region& outerBox, Node* parent = NULL) :
				parent(parent), type(type), outerBox(outerBox)
			{
			}
			virtual ~Node() { } // so class counts as polymorphic type
		};

		struct ShrinkSplitNode : public Node
		{
			Node* leftChild; // left split of inner box
			Node* rightChild; // right split of inner box
			Node* outerChild; // child which initially contains inner box

			ShrinkSplitNode(const Region& outerBox, Node* leftChild,
				Node* rightChild, Node* outerChild, Node* parent = NULL) :
				Node(SHRINKSPLIT_NODE, outerBox, parent), leftChild(leftChild),
				rightChild(rightChild), outerChild(outerChild)
			{
			}
			virtual ~ShrinkSplitNode()
			{
				delete leftChild;
				delete rightChild;
				delete outerChild;
			}
		};

		struct LeafNode : public Node
		{
			// If true, then the point contains an inner box and NOT a point
			bool containsInnerBox;
			// Which of these is set depends on 'containsInnerBox' flag
			Region innerBox;
			Point point;

			LeafNode(const Region& outerBox, Node* parent = NULL) :
				Node(EMPTY_LEAF_NODE, outerBox, parent), containsInnerBox(false),
				innerBox(0), point(0)
			{
			}

			LeafNode(const Region& outerBox, const Region& innerBox, Node* parent) :
				Node(FILLED_LEAF_NODE, outerBox, parent), containsInnerBox(true),
				innerBox(innerBox), point(0)
			{
			}

			LeafNode(const Region& outerBox, const Point& point, Node* parent) :
				Node(FILLED_LEAF_NODE, outerBox, parent), containsInnerBox(false),
				innerBox(0), point(point)
			{
			}
		};

		SplayQuadtree(unsigned int numDimensions, const Region& initialBoundary);
		virtual ~SplayQuadtree();

		void clear();
		bool insert(const Point& p);
		bool remove(const Point& p);
		bool update(const Point& oldPoint, const Point& newPoint);
		bool pointExists(const Point& p);
		PointList pointsInRegion(const Region& region);

		/* NOTE: Only public so it can be tesed thoroughly in unit tests. */
		bool promote(SplayQuadtree::Node* node);

		/* Return root node of the splay quadtree. */
		Node* rootNode();

		/* Generate textual string representation of Splay Quadtree node hierarhcy. */
		std::string toString() const;

	private:
		/* Find leaf node with a region that contains the given point.
		 * Returns NULL if the point is out the boundary of the structure. */
		LeafNode* findContainingNode(const Point& p) const;
		/* Find leaf node that STORES the given point. If the point is not
		 * being stored in the structure, then NULL will be returned. */
		LeafNode* findNodeStoredIn(const Point& p) const;
		/* Perform a shrink-split operation on a leaf node, so the new
		 * point 'p' can be stored. */
		ShrinkSplitNode* performShrinkSplit(LeafNode* leaf, const Point& p);
		/* Return relation between a specified parent node and a potential child. */
		NodeRelation relation(ShrinkSplitNode* parent, Node* child) const;

		/* Recursively generates a nested text representation of a tree node. */
		std::string nodeToString(Node* node, int level = 0) const;

		Region boundary; // use to initialise root node
		Node* root;

	};

}

#endif