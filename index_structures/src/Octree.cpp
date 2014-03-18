#include "Octree.h"
#include <cmath>

namespace mdsearch
{

	Octree::Octree(int numDimensions, const Region& boundary) :
		IndexStructure(numDimensions), boundary(boundary),
		numChildrenPerNode( pow(2, numDimensions) ),
		totalPointsInsideNode(0)
		
	{
		// Pre-allocate enough memory to store max amount of points in this node
		points.reserve(MAX_POINTS_PER_NODE);
	}

	Octree::~Octree()
	{
		removeAllChildren();
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

	bool Octree::isLeaf() const
	{
		// Using size() not end() because it provide faster during profiling
		return (children.size() == 0);
	}

	bool Octree::empty() const
	{
		return (totalPointsInsideNode == 0);
	}
	
	void Octree::clear()
	{
		// Remove all child nodes and ensure points are cleared from node
		removeAllChildren();
		points = PointList();
		totalPointsInsideNode = 0;
	}

	bool Octree::insert(const Point& p)
	{
		if (!boundary.contains(p))
		{
			return false;
		}
		// If this is a non-leaf node
		else if (!isLeaf())
		{
			for (OctreeNodeList::iterator it = children.begin();
				(it != children.end()); it++)
			{
				if ((*it)->insert(p))
				{
					totalPointsInsideNode++;
					return true;
				}
			}
			return false;
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
				totalPointsInsideNode++;
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
				{
					totalPointsInsideNode++;
					return true;
				}
			}
			// If this is reached, point could not be inserted.
			// NOTE: THIS CODE SHOULD NEVER BE REACHED!
			// It's here to make the compiler happy.
			return false;
		}
	}

	bool Octree::remove(const Point& p)
	{
		if (points.size() > 0) // if leaf node
		{
			for (PointList::iterator it = points.begin(); (it != points.end()); it++)
			{
				// If the point was found, then ERASE it and return true!
				if (*it == p)
				{
					points.erase(it);
					totalPointsInsideNode--;
					return true;
				}
			}
			return false;
		}
		else // if non-leaf
		{
			// Try removing point from children
			// If a node returns true, then keep a pointer to that node
			for (OctreeNodeList::iterator it = children.begin(); (it != children.end()); it++)
			{
				if ((*it)->remove(p))
				{
					// Point was successfully removed, so this node now contains one
					// less point overall
					totalPointsInsideNode--;
			   		// If this node is now empty (no points stored in any of the children),
			   		// collapse into single node by removing all children
					if (empty())
						removeAllChildren();
					// Either way, a point was deleted so return true
					return true;
				}
			}

			return false;
		}

		return false; // here to ensure compilers don't complain
	}

	bool Octree::update(const Point& oldPoint, const Point& newPoint)
	{
		// Remove old point from tree
		bool success = remove(oldPoint);
		// If point was found and removed, insert new point IF AND 
		// ONLY IF the new point does not already exist (as we're
		// only allowed UNIQUE points with this index structure)
		if (success)
		{
			// Don't return result of this since we don't care if 
			// point is not inserted because it exists -- this
			// update() operation is still a success
			insert(newPoint);
			return true;
		}
		else
		{
			return false;
		}
	}
	
	bool Octree::pointExists(const Point& p)
	{
		if (boundary.contains(p))
		{
			if (isLeaf())
			{
				for (PointList::const_iterator it = points.begin();
					(it != points.end()); it++)
				{
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
		PointList foundPoints;
		recursiveRegionQuery(region, foundPoints);
		return foundPoints;
	}

	void Octree::recursiveRegionQuery(const Region& region, PointList& foundPoints)
	{
		if (boundary.intersects(region))
		{
			if (isLeaf())
			{
				for (PointList::const_iterator it = points.begin();
					(it != points.end()); it++)
				{
					if (region.contains(*it))
					{
						foundPoints.push_back(*it);
					}
				}
			}
			else
			{
				for (OctreeNodeList::const_iterator it = children.begin();
					(it != children.end()); it++)
				{
					// NOTE: Calling private method, but allowed because it is
					// a method of the same class (automatic friend class).
					// TODO: Code smell! Fix in the future??
					(*it)->recursiveRegionQuery(region, foundPoints);
				}
			}
		}
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
			Octree* child = new Octree(numDimensions, childrenBoundaries[i]);
			children.push_back(child);
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

	void Octree::removeAllChildren()
	{
		// Recursively delete child nodes
		for (OctreeNodeList::iterator it = children.begin();
			(it != children.end()); it++)
		{
			delete *it;
		}
		children.clear();
		totalPointsInsideNode = 0;
	}

}