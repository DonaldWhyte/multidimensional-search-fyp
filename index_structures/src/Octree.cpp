#include "Octree.h"

namespace mdsearch
{

	Octree::Octree(int numDimensions, const Region& boundary) : IndexStructure(numDimensions),
		numSubdivisionsToMake(2 ^ numDimensions), boundary(boundary)
	{
		// TODO: comment
		points.reserve(MAX_POINTS_PER_NODE);
	}

	Octree::~Octree()
	{
		for (std::vector<Octree*>::iterator it = children.begin();
			(it != children.end()); it++)
		{
			delete *it;
		}
	}

	void Octree::loadPoints(const PointList& pointsToAdd)
	{
		for (PointList::const_iterator it = pointsToAdd.begin();
			(it != pointsToAdd.end()); it++)
		{
			insert(*it);
		}
	}
	
	bool Octree::insert(const Point& p)
	{
		if (!boundary.contains(p))
		{
			return false;
		}
		// If there is more space to put shapes in this node, add the shape
	    else if (points.size() < MAX_POINTS_PER_NODE)
	    {
	        points.push_back(p);
	    }
	    // Otherwise, divide node into sub-nodes and add point to the correct child
	    else
	    {
	        subdivide();
			for (std::vector<Octree*>::iterator it = children.begin();
				(it != children.end()); it++)
			{
	            if ((*it)->insert(p))
	                return true;
	        }
			// If this is reached, point could not be inserted.
			// NOTE: THIS CODE SHOULD NEVER BE REACHED!
			// It's here to make the compiler happy.
	        return false;
	    }
	}

	bool Octree::remove(const Point& p)
	{
		// TODO
	}

	bool Octree::update(const Point& oldPoint, const Point& newPoint)
	{
		// TODO
	}
	
	bool Octree::pointExists(const Point& p)
	{
		// TODO
	}

	PointList Octree::pointsInRegion(const Region& region)
	{
		// TODO
	}

	void Octree::subdivide()
	{
		// TODO: comment
		if (children.size() > 0)
			return;

		// Construct sub-region for each child (equal size)
		RegionList childrenBoundaries;
		childrenBoundaries.reserve(numSubdivisionsToMake);
		// TODO
		
		// Create the children
		children.reserve(numSubdivisionsToMake);
		for (unsigned int i = 0; (i < numSubdivisionsToMake); i++)
		    children.push_back( new Octree(numDimensions, childrenBoundaries[i]) );
		// For all the points currently in this node, move them to one of the children
		unsigned int pointsMoved = 0;
		for (unsigned int pointIndex = 0; (pointIndex < points.size()); pointIndex++)
		{
		    for (unsigned int childIndex = 0; (childIndex < children.size()); childIndex++)
		    {
		        // If true is returned, then we've inserted the shape, so we
		        // move onto the next by breaking out of child-loop
		        Octree* child = children[childIndex];
		        if (child->insert(points[pointIndex]))
		        {
		        	pointsMoved++;
		            break;
		        }
		    }
		}
		
		points.clear();
	}

}