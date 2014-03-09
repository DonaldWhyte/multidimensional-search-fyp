#ifndef MDSEARCH_OCTREE_H
#define MDSEARCH_OCTREE_H

#include "IndexStructure.h"



namespace mdsearch
{

	class Octree : public IndexStructure
	{

	public:
		Octree(int numDimensions, const Region& boundary);
		virtual ~Octree();

		// Bulk load points into the structure
		void loadPoints(const PointList& points);

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

		static const unsigned int MAX_POINTS_PER_NODE = 8;

		Region boundary;
		unsigned int numSubdivisionsToMake;

		PointList points;
		std::vector<Octree*> children;

	};

}

#endif