#ifndef MDSEARCH_BDTREE_H
#define MDSEARCH_BDTREE_H

#include "IndexStructure.h"

namespace mdsearch
{

	class BDTree : public IndexStructure
	{

	public:
		BDTree(unsigned int numDimensions, const Region& boundary);

		virtual void clear();

		virtual bool insert(const Point& p);
		virtual bool remove(const Point& p);
		virtual bool update(const Point& oldPoint, const Point& newPoint);

		virtual bool pointExists(const Point& p);
		virtual PointList pointsInRegion(const Region& region);

	private:
		Region boundary;

	};

}

#endif