#ifndef MDSEARCH_OCTREE_H
#define MDSEARCH_OCTREE_H

#include "IndexStructure.h"

namespace mdsearch
{

	class Octree;
	typedef std::vector<Octree*> OctreeNodeList;

	/* Specifically, this is a Bucket PR Octree. */
	class Octree : public IndexStructure
	{

	public:
		// Constant that determines the bucket size
		// That is, how many points can be stored in a single leaf node
		static const unsigned int MAX_POINTS_PER_NODE = 8;

		// An Octree must be initialised with its dimensionality and the
		// region of space is covers. A pointer to the node's parent (if
		// the node is not the parent) must also be provided for efficient
		// removal and updating.
		Octree(int numDimensions, const Region& boundary, Octree* nodeParent = NULL);
		virtual ~Octree();

		// Return number of children each non-leaf node has
		// (fixed number based on dimensionality)
		unsigned int childrenPerNode() const;
		// Read-only accessors
		const PointList& storedPoints() const;
		const OctreeNodeList& nodeChildren() const;
		const Region& regionCovered() const;
		// Read-write accessors
		Octree* parent() const;

		// Return true if node is a leaf
		bool isLeaf() const;
		// Checks if node (including all of its children) are empty
		bool empty() const;

		// Dynamic operations
		bool insert(const Point& p);
		bool remove(const Point& p);
		bool update(const Point& oldPoint, const Point& newPoint);
		// Queries
		bool pointExists(const Point& p);
		PointList pointsInRegion(const Region& region);

	private:
		// Sub-divide octree into four sub-regions (nodes)
		void subdivide();
		// Return true if this node stores the given point, false otherwise
		bool storesPoint(const Point& p) const;
		// Perform recursive search through octree to find all points inside
		// the given region. At the top level, an empty PointList should be
		// given. This will be populated with any found points at the
		// end of the recursive algorithm.
		void recursiveRegionQuery(const Region& region, PointList& foundPoints);
		// Remove all child nodes from this Octree node
		void removeAllChildren();

		Octree* nodeParent;
		Region boundary;
		unsigned int numChildrenPerNode;

		PointList points;
		OctreeNodeList children;

	};

}

#endif