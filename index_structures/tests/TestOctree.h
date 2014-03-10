#ifndef MDSEARCH_TEST_OCTREE_H
#define MDSEARCH_TEST_OCTREE_H

#include <gtest/gtest.h>
#include "Octree.h"
#include "Common.h"

namespace mdsearch { namespace tests
{

	static const unsigned int NUM_OCTREE_DIMENSIONS = 3;

	class OctreeTests : public ::testing::Test
	{

	protected:
		virtual void SetUp()
		{
			Real closePointValues[NUM_CLOSE_POINTS][NUM_OCTREE_DIMENSIONS] = {
				{ 1, 1, 1 }, { 2, 1, 1 }, { 1, 2, 1 }, { 1, 1, 2 }, 
				{ 2, 2, 1 }, { 1, 2, 2 }, { 2, 1, 2 }, { 2, 2, 2 }, 
			};
			closePoints.reserve(NUM_CLOSE_POINTS);
			for (unsigned int i = 0; (i < NUM_CLOSE_POINTS); i++)
				closePoints.push_back( Point(NUM_OCTREE_DIMENSIONS, closePointValues[i]) );			

			Interval initialBoundaryIntervals[3] = { Interval(0, 10), Interval(0, 10), Interval(0, 10) };
			initialBoundary = Region(3, initialBoundaryIntervals);
		}

		// Constants about test data
		static const unsigned int NUM_CLOSE_POINTS = 8;

		PointList closePoints;
		Region initialBoundary;

	};

	TEST_F(OctreeTests, Construction)
	{
		Octree structure(NUM_OCTREE_DIMENSIONS, initialBoundary);
		ASSERT_EQ(0, structure.storedPoints().size());
		ASSERT_EQ(0, structure.nodeChildren().size());
		ASSERT_EQ(NUM_OCTREE_DIMENSIONS, structure.dimensionality());
		ASSERT_EQ(8, structure.childrenPerNode());

		structure = Octree(8, initialBoundary);
		ASSERT_EQ(0, structure.storedPoints().size());
		ASSERT_EQ(0, structure.nodeChildren().size());
		ASSERT_EQ(8, structure.dimensionality());
		ASSERT_EQ(256, structure.childrenPerNode());
	}

	TEST_F(OctreeTests, InsertionAndSubdivide)
	{
		Octree structure(NUM_OCTREE_DIMENSIONS, initialBoundary);
		IndexStructureTester tester;
		const PointList& testPoints = tester.getTestPoints();

		// Test point that IS NOT CONAINED in the region the octree covers
		Real outsidePointValues[3] = { 30, 30, 30 };
		Point outsidePoint(3, outsidePointValues);
		ASSERT_FALSE(structure.insert(outsidePoint));
		ASSERT_EQ(0, structure.storedPoints().size());
		ASSERT_EQ(0, structure.nodeChildren().size());
		
		// Insert point and check it's direct child of octree
		ASSERT_TRUE(structure.insert(closePoints[0]));
		ASSERT_EQ(1, structure.storedPoints().size());
		ASSERT_EQ(closePoints[0], structure.storedPoints()[0]);
		ASSERT_EQ(0, structure.nodeChildren().size());
		// Insert same point to ensure it is NOT inserted
		ASSERT_FALSE(structure.insert(closePoints[0]));
		ASSERT_EQ(1, structure.storedPoints().size());
		ASSERT_EQ(closePoints[0], structure.storedPoints()[0]);
		ASSERT_EQ(0, structure.nodeChildren().size());

		// Insert points close together and then a point far away
		for (unsigned int i = 1; (i < NUM_CLOSE_POINTS); i++)
		{
			ASSERT_TRUE(structure.insert(closePoints[i]));
		}
		ASSERT_TRUE(structure.insert(testPoints[8]));
		// Ensure correct number of children have been made
		ASSERT_EQ(0, structure.storedPoints().size());
		ASSERT_EQ(structure.childrenPerNode(), structure.nodeChildren().size());
		// Ensure all children are empty EXCEPT FOR ONE REGION
		bool foundCluster = false;
		bool foundFar = false;
		const OctreeNodeList& children = structure.nodeChildren();
		for (OctreeNodeList::const_iterator it = children.begin();
			(it != children.end()); it++)
		{
			const PointList& storedPoints = (*it)->storedPoints();
			unsigned int nPoints = storedPoints.size();
			if (nPoints == 1) // far away point
			{
				EXPECT_EQ(testPoints[8], storedPoints[0]);
				foundFar = true;
			}
			else if (nPoints == Octree::MAX_POINTS_PER_NODE) // cluster of close points
			{
				EXPECT_EQ(closePoints, storedPoints);
				foundCluster = true;
			}
			else
			{
				EXPECT_EQ(0, nPoints);
			}
		}
		EXPECT_TRUE(foundFar);
		EXPECT_TRUE(foundCluster);
	}

	TEST_F(OctreeTests, Removal)
	{
		Octree structure(NUM_OCTREE_DIMENSIONS, initialBoundary);
		// TODO
	}

	TEST_F(OctreeTests, Updating)
	{
		Octree structure(NUM_OCTREE_DIMENSIONS, initialBoundary);
		// TODO
	}

	TEST_F(OctreeTests, PointQueries)
	{
		Octree structure(IndexStructureTester::NUM_TEST_DIMENSIONS, initialBoundary);
		IndexStructureTester tester;
		tester.testPointQueries(&structure);
	}

	TEST_F(OctreeTests, RegionQueries)
	{
		Octree structure(IndexStructureTester::NUM_TEST_DIMENSIONS, initialBoundary);
		IndexStructureTester tester;
		tester.testRegionQueries(&structure);
	}


} }

#endif