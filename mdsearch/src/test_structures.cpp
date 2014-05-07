/******************************************************************************

mdsearch - Lightweight C++ library implementing a collection of
           multi-dimensional search structures

File:        test_structures.cpp
Description: TODO

*******************************************************************************

The MIT License (MIT)

Copyright (c) 2014 Donald Whyte

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

******************************************************************************/

#include "kdtree.hpp"
#include "pyramidtree.hpp"
#include "hashtable.hpp"
#include "multigrid.hpp"
#include "dataset.hpp"
#include <string>

using namespace mdsearch;

static const int NUM_DIMENSIONS = 10;
static const int NUM_TEST_POINTS = 10000;
typedef std::vector< Point<NUM_DIMENSIONS> > PointList;

Real generateRandomNumber(Real minimum, Real maximum)
{
	return minimum + static_cast<float>(rand()) / static_cast<float>(RAND_MAX / (maximum - minimum));
}

PointList generateRandomPoints(unsigned int numPoints)
{
	std::vector< Point<NUM_DIMENSIONS> > points;
	Real test = generateRandomNumber(0.0f, 1.0f);
	for (unsigned int i = 0; (i < numPoints); i++)
	{
		Point<NUM_DIMENSIONS> p;
		for (unsigned int d = 0; (d < NUM_DIMENSIONS); d++)
		{
			p[d] = generateRandomNumber(0.0f, 1.0f);
		}
		points.push_back(p);
	}
	return points;
}


template<typename STRUCT_TYPE>
bool testStructureOperations(STRUCT_TYPE* structure, const PointList& points)
{
	// NOTE; Tests assume all given points are UNIQUE!!!

	// Ensure structure is entirely empty
	for (unsigned int i = 0; (i < points.size()); i++)
	{
		if (structure->query(points[i]))
		{
			std::cout << "False positive point query with point " << i << ":" << points[i] << std::endl;
			return false;
		}
	}
	// Test insertions
	for (unsigned int i = 0; (i < points.size()); i++)
	{
		// Don't test if true returned -- may be duplicates in dataset!
		structure->insert(points[i]);
	}
	// Test queries
	for (unsigned int i = 0; (i < points.size()); i++)
	{
		if (!structure->query(points[i]))
		{
			std::cout << "Failed query with point " << i << ":" << points[i] << std::endl;
			return false;
		}
	}
	// Test removals
	for (unsigned int i = 0; (i < points.size()); i++)
	{
		if (structure->remove(points[i]))
		{
			if (structure->query(points[i]))
			{
				std::cout << "Failed removal (never actually removed) with point" << i << ":" << points[i] << std::endl;	
				return false;
			}
		}
		else
		{
			std::cout << "Failed removal (returned failure signal) with point " << i << ":" << points[i] << std::endl;
			return false;
		}
	}

	return true; // all tests passed
}

template<typename STRUCT_TYPE>
void testStructure(const std::string& structureName, STRUCT_TYPE* structure, const PointList& points)
{
	std::cout << "TESTING " << structureName << "..." << std::endl;
	if (testStructureOperations<STRUCT_TYPE>(structure, points))
		std::cout << "...SUCCESS." << std::endl;
	else
		std::cout << "...FAILED." << std::endl;
}


int main(int argc, char* argv[])
{
	// Generate test data using random number generator
	srand(time(NULL)); // seed generator to get different points every time!!
	Dataset<NUM_DIMENSIONS> dataset;
	dataset.load( generateRandomPoints(NUM_TEST_POINTS) );

	// Test desired structures
	KDTree<NUM_DIMENSIONS> kdTree;
	testStructure< KDTree<NUM_DIMENSIONS> >("kd-tree", &kdTree, dataset.getPoints());

	PyramidTree<NUM_DIMENSIONS> pyramidTree(dataset.computeBoundary());
	testStructure< PyramidTree<NUM_DIMENSIONS> >("pyramid tree", &pyramidTree, dataset.getPoints());

	HashTable<NUM_DIMENSIONS> hashTable;
	testStructure< HashTable<NUM_DIMENSIONS> >("hash table", &hashTable, dataset.getPoints());

	Multigrid<NUM_DIMENSIONS> multigrid(dataset.computeBoundary());
	testStructure< Multigrid<NUM_DIMENSIONS> >("multigrid", &multigrid, dataset.getPoints());

	return 0;
}