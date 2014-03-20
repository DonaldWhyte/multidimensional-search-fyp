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
		};

		struct ShrinkSplitNode : public Node
		{
			Node* leftChild; // left split of inner box
			Node* rightChild; // right split of inner box
			Node* outerChild; // child which initially contains inner box
		};

		struct LeafNode : public Node
		{
			// If true, then the point contains an inner box and NOT a point
			bool containsInnerBox;
			// Which of these is set depends on 'containsInnerBox' flag
			Region innerBox;
			Point point;
		};

		SplayQuadtree(unsigned int numDimensions, const Region& initialBoundary);
		virtual ~SplayQuadtree();

	private:
		Region boundary; // use to initialise root node

	};

}

#endif