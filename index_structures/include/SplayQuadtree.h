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

		/* Return root node of the splay quadtree. */
		Node* rootNode();

	private:
		/* TODO: comment */
		LeafNode* findContainingNode(const Point& p);
		ShrinkSplitNode* performShrinkSplit(LeafNode* leaf, const Point& p);
		Real computeSplitPoint(LeafNode* leaf, const Point& newPoint,
			const Region& minQuadtreeBox, unsigned int splitDimension);

		Region boundary; // use to initialise root node
		Node* root;

	};

}

#endif