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
		// region of space is covers
		Octree(int numDimensions, const Region& boundary);
		virtual ~Octree();

		// Bulk load points into the structure
		void loadPoints(const PointList& points);
		// Return number of children each non-leaf node has
		// (fixed number based on dimensionality)
		unsigned int childrenPerNode() const;
		// Read-only accessors
		const PointList& storedPoints() const;
		const OctreeNodeList& nodeChildren() const;
		const Region& regionCovered() const;

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

		Region boundary;
		unsigned int numChildrenPerNode;

		PointList points;
		OctreeNodeList children;

	};

}

#endif