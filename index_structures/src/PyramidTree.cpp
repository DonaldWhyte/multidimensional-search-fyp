#include "PyramidTree.h"

namespace mdsearch
{

	PyramidTree::PyramidTree(unsigned int nDimensions, const Region& treeBoundary)
		: IndexStructure(nDimensions), boundary(treeBoundary), bucketInterval(1),
		medianPoint(nDimensions)
	{
		// Compute the interval between buckets 
		bucketInterval = static_cast<double>(
			MAX_BUCKET_NUMBER / (numDimensions * 2)
		);
		bucketInterval = floor(bucketInterval);
		// Compute initial median value
		double m = static_cast<double>(1.0 / numDimensions);
		int div = ceil(pow(MAX_BUCKET_NUMBER, m));
		for (unsigned int d = 0; d < numDimensions; d++)
		{
			medianPoint[d] = div;
		}
		// Ensure boundaries are NEVER ZERO SIZED and the maximum
		// values are always larger than the minimum
		for (unsigned int d = 0; d < numDimensions; d++)
		{
			if (boundary[d].max <= boundary[d].min)
			{
				boundary[d].max = boundary[d].min + 1;
			}
		}
	}

	void PyramidTree::clear()
	{
		// NOTE: Using assigment not clear() to ensure memory is de-allocated
		// (through destructors of containers)
		points = PointList();
		pointSums = RealList();
		Map = OneDMap();
	}

	bool PyramidTree::insert(const Point& point)
	{
		int pointIndex = getPointIndex(point);
		if (pointIndex >= 0) // if point already exists in structure
		{
			return false;
		}
		else // if point does not exist, insert it!
		{
			bool searchKeyExists = (pointIndex == -2);
			insertToStructure(point, searchKeyExists);
			return true;
		}
	}

	bool PyramidTree::remove(const Point& point)
	{
		// TODO
		return false;
	}

	bool PyramidTree::update(const Point& oldPoint, const Point& newPoint)
	{
		if (remove(oldPoint))
		{
			insert(newPoint);
			return true;
		}
		else
		{
			return false;
		}
	}

	bool PyramidTree::pointExists(const Point& point)
	{
		return (getPointIndex(point) >= 0);
	}

	PointList PyramidTree::pointsInRegion(const Region& region)
	{
		// TODO
		return PointList();
	}

	const PointList& PyramidTree::allPoints() const
	{
		return points;
	}

	void PyramidTree::insertToStructure(const Point& point, bool searchKeyExists)
	{
		// Transform the original data to the encoded data sets.
		double sum = 0;

		for (int j = 0; j < numDimensions; j++) 
		{
			sum += point[j];
		}
		// Add raw point adn the sum of all its elements to the vectors
		points.push_back(point);
		pointSums.push_back(sum);

		int searchKey = HashValue(point);
		int currentIndex = points.size() - 1;
		if (searchKeyExists)
		{
			// If there is no inserted data, but there is a search key
			std::vector<int> value = Map.find(searchKey)->second;
			value.push_back(currentIndex);
			Map[searchKey] = value;
		}
		else
		{
			// If there is no inserted data and there is no search key
			std::vector<int> myVector;
			myVector.reserve(10);
			myVector.push_back(currentIndex);
			Map[searchKey] = myVector;
		}
	}

	int PyramidTree::getPointIndex(const Point& point)
	{
		int finalIndex;
		double pSum = 0;

		for(int d = 0; d < numDimensions; d++) 
		{
			pSum += point[d];
		}

		int searchKey = HashValue(point);
		if (Map.find(searchKey) != Map.end())
		{
			std::vector<int> output = Map.find(searchKey)->second;
			bool contain = false;

			for (int i = 0; i < output.size(); i++)
			{
				int index = output[i];
				double sum = pointSums[index];
				if (compare(sum, pSum) == 0)
				{
					Point foundPoint = points[index];
					if (point == foundPoint)
			  		{
			  			return index;
			  		}
				}
		  	}
			// if there is no inserted data, but there is search key
			if (!contain)
			{
				return -2;
			}
		}
		else
		{
			// if there is no inserted data
			return -1;
		}
	}	

	int PyramidTree::HashValue(const Point& point)
	{
		int searchKey = 0 ;
		int value[numDimensions];
		for (int d = 0; d < numDimensions; d++ )
		{
			value[d] = static_cast<int>(
				static_cast<double>((point[d] - boundary[d].min) / (boundary[d].max - boundary[d].min))
				* medianPoint[d]);
			if (value[d] >= medianPoint[d])
			{
				value[d] = medianPoint[d] - 1;
			}
		}
		
		for (int d = 0; d < numDimensions; d++)
		{
			int temp = value[d];
			for (int j = 0; j < d; j++)
			{	
				temp = temp * medianPoint[j];
			}
			searchKey = searchKey + temp;
		}
		return searchKey;
	}

}