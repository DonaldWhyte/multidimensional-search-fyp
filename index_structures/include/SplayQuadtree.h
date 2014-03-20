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
			EMPTY_NODE = 0,
			LEAF_NODE,
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
		};

		struct ShrinkSplitNode : public Node
		{
			Node* leftChild; // left split of inner box
			Node* rightChild; // right split of inner box
			Node* outerChild; // child which initially contains inner box

			ShrinkSplitNode(NodeType type, const Region& outerBox, Node* parent = NULL) :
				Node(type, outerBox, parent)
			{
			}
		};

		struct LeafNode : public Node
		{
			// If true, then the point contains an inner box and NOT a point
			bool containsInnerBox;
			// Which of these is set depends on 'containsInnerBox' flag
			Region innerBox;
			Point point;

			LeafNode(NodeType type, const Region& outerBox, const Region& innerBox, Node* parent) :
				Node(type, outerBox, parent), containsInnerBox(true), innerBox(innerBox), point(0)
			{
			}

			LeafNode(NodeType type, const Region& outerBox, const Point& point, Node* parent) :
				Node(type, outerBox, parent), containsInnerBox(false), innerBox(0), point(point)
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
		Region boundary; // use to initialise root node
		Node* root;

	};

}

#endif