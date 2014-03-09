#ifndef MDSEARCH_TEST_OCTREE_H
#define MDSEARCH_TEST_OCTREE_H

#include <gtest/gtest.h>
#include "Octree.h"

namespace mdsearch { namespace tests
{

	class OctreeTests : public ::testing::Test
	{

	protected:
		virtual void SetUp()
		{
			// Construct test dataset
			Real testPointValues[NUM_TEST_POINTS][NUM_DIMENSIONS] = {
				{9, 2, 0}, {3, 6, 6}, {4, 0, 3}, {6, 5, 8}, {6, 7, 6},
				{1, 3, 0}, {6, 9, 0}, {2, 4, 7}, {4, 5, 9}, {5, 6, 0}
			};
			testPoints.reserve(NUM_TEST_POINTS);
			for (unsigned int i = 0; (i < NUM_TEST_POINTS); i++)
				testPoints.push_back( Point(NUM_DIMENSIONS, testPointValues[i]) );

			Interval initialBoundaryIntervals[3] = { Interval(0, 10), Interval(0, 10), Interval(0, 10) };
			initialBoundary = Region(3, initialBoundaryIntervals);
		}

		virtual void TearDown()
		{
			// Clear test dataset
			testPoints.clear();
		}

		// Constants about test data
		static const unsigned int NUM_TEST_POINTS = 10;
		static const unsigned int NUM_DIMENSIONS = 3;
		PointList testPoints;
		Region initialBoundary;

	};

	TEST_F(OctreeTests, Construction)
	{
		Octree structure(NUM_DIMENSIONS, initialBoundary);
		// TODO
	}

	TEST_F(OctreeTests, Insertion)
	{
		Octree structure(NUM_DIMENSIONS, initialBoundary);
		// TODO
	}

	TEST_F(OctreeTests, Removal)
	{
		Octree structure(NUM_DIMENSIONS, initialBoundary);
		// TODO
	}

	TEST_F(OctreeTests, Updating)
	{
		Octree structure(NUM_DIMENSIONS, initialBoundary);
		// TODO
	}

	TEST_F(OctreeTests, PointQueries)
	{
		Octree structure(NUM_DIMENSIONS, initialBoundary);
		// TODO
	}

	TEST_F(OctreeTests, RegionQueries)
	{
		Octree structure(NUM_DIMENSIONS, initialBoundary);
		// TODO
	}


} }

#endif