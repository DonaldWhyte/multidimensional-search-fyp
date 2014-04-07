#include "UniqueHashTable.h"
#include "Hashing.h"

namespace mdsearch
{

	UniqueHashTable::UniqueHashTable(unsigned int numDimensions) : IndexStructure(numDimensions)
	{
		clear();
	}

	void UniqueHashTable::clear()
	{
		hashMap = PointMap();
	}

	bool UniqueHashTable::insert(const Point& point)
	{
		Real searchKey = hashPoint(point);
		if (hashMap.find(searchKey) != hashMap.end())
		{
			return false;
		}
		else
		{
			hashMap.insert(std::make_pair(searchKey, point));
			return true;
		}
	}

	bool UniqueHashTable::remove(const Point& point)
	{
		Real searchKey = hashPoint(point);
		PointMap::iterator it = hashMap.find(searchKey);
		if (it != hashMap.end())
		{
			hashMap.erase(it);
			return true;
		}
		else
		{
			return false;
		}
	}

	bool UniqueHashTable::update(const Point& oldPoint, const Point& newPoint)
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

	bool UniqueHashTable::pointExists(const Point& point)
	{
		Real searchKey = hashPoint(point);
		PointMap::const_iterator it = hashMap.find(searchKey);
		if (it != hashMap.end())
		{
			return (it->second == point);
		}
		else
		{
			return false;
		}
	}

}