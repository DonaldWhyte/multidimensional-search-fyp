#include "Octree.h"
#include <cmath>

namespace mdsearch
{

	Octree::Octree(int numDimensions, const Region& boundary) : IndexStructure(numDimensions),
		numChildrenPerNode( pow(2, numDimensions) ),
		boundary(boundary)
	{
		// Pre-allocate enough memory to store max amount of points in this node
		points.reserve(MAX_POINTS_PER_NODE);
	}

	Octree::~Octree()
	{
		// Recursively delete child nodes
		for (OctreeNodeList::iterator it = children.begin();
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

	unsigned int Octree::childrenPerNode() const
	{
		return numChildrenPerNode;
	}

	const PointList& Octree::storedPoints() const
	{
		return points;
	}

	const OctreeNodeList& Octree::nodeChildren() const
	{
		return children;
	}

	const Region& Octree::regionCovered() const
	{
		return boundary;
	}
	
	bool Octree::insert(const Point& p)
	{
		if (!boundary.contains(p))
		{
			return false;
		}
		// If this is a non-leaf node
		else if (!children.empty())
		{
			for (OctreeNodeList::iterator it = children.begin();
				(it != children.end()); it++)
			{
	            if ((*it)->insert(p))
	                return true;
	        }			
		}
		// If there is more space to put shapes in this node, add the shape
	    else if (points.size() < MAX_POINTS_PER_NODE)
	    {
	    	// Check if point already exists. If so, don't add it!
	    	if (storesPoint(p))
	    	{
	    		return false;
	    	}
	    	else
	    	{
	    		points.push_back(p);
	    		return true;	
	    	}
	    }
	    // Otherwise, divide node into sub-nodes and add point to the correct child
	    else
	    {
	        subdivide();
			for (OctreeNodeList::iterator it = children.begin();
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
		if (boundary.contains(p))
		{
			//std::cout << p << " CHILDREN SIZE: " << children.size() << std::endl;
			if (children.empty())
			{
				for (PointList::const_iterator it = points.begin();
					(it != points.end()); it++)
				{
					//std::cout << "\t" << p << " " << *it << std::endl;
					if (*it == p)
					{
						return true;
					}
				}
			}
			else
			{
				for (OctreeNodeList::const_iterator it = children.begin();
					(it != children.end()); it++)
				{
					//std::cout << p << " " << (*it)->storedPoints().size() << " " << (*it)->nodeChildren().size() << std::endl;
					if ((*it)->pointExists(p))
					{
						return true;
					}
				}
			}
			return false;
		}
		else
		{
			return false;
		}
	}

	PointList Octree::pointsInRegion(const Region& region)
	{
		// TODO
	}

	void Octree::subdivide()
	{
		// Do nothing if the octree has children. This is because a
		// subdivision has already been performed
		if (children.size() > 0)
			return;

		// Construct sub-region for each child (equal size)
		RegionList childrenBoundaries = boundary.subdivide();
		
		// Create the children
		children.reserve(numChildrenPerNode);
		for (unsigned int i = 0; (i < numChildrenPerNode); i++)
		{
		    children.push_back( new Octree(numDimensions, childrenBoundaries[i]) );
		}
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

	bool Octree::storesPoint(const Point& p) const
	{
		for (PointList::const_iterator it = points.begin();
			(it != points.end()); it++)
		{
			if (*it == p)
				return true;
		}
		return false;
	}

}