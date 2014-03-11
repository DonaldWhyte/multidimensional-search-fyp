#ifndef MDSEARCH_TEST_DATASETGENERATORS_H
#define MDSEARCH_TEST_DATASETGENERATORS_H

#include <gtest/gtest.h>
#include "UniformDatasetGenerator.h"
#include "HyperCubeRandomGenerator.h"

namespace mdsearch { namespace tests
{

	class DatasetGeneratorTests : public ::testing::Test
	{

	};

	TEST_F(DatasetGeneratorTests, UniformData)
	{
		// Construct expected test data
		Real oneDZeroIntervalValues = 2;
		Real oneDInterval1PValues = 15;
		Real oneDInterval2PValues[] = { 10, 20 };
		Real oneDInterval3PValues[] = { 10, 15, 20 };
		Real spacing = (20.0 - 10.0) / 3;
		Real oneDInterval4PValues[] = { 10, 10 + spacing, 10 + spacing * 2, 20 };

		Real twoDZeroIntervalValues[2] = { 2, 2 };
		Real twoDInterval1PValues[2] = { 15, 15 };
		Real twoDInterval2PValues[][2] = { {10, 10}, {20, 10}, {10, 20}, {20, 20} };
		Real twoDInterval3PValues[][2] = {
			{10, 10}, {15, 10}, {20, 10}, {10, 15},
			{15, 15}, {20, 15}, {10, 20}, {15, 20},
			{20, 20}
		};

		PointList oneDZeroInterval;
		oneDZeroInterval.push_back(Point(1, oneDZeroIntervalValues));

		PointList oneDInterval1P;
		oneDInterval1P.push_back(Point(1, oneDInterval1PValues));
		PointList oneDInterval2P;
		oneDInterval2P.push_back(Point(1, oneDInterval2PValues[0]));
		oneDInterval2P.push_back(Point(1, oneDInterval2PValues[1]));
		PointList oneDInterval3P;
		oneDInterval3P.push_back(Point(1, oneDInterval3PValues[0]));
		oneDInterval3P.push_back(Point(1, oneDInterval3PValues[1]));
		oneDInterval3P.push_back(Point(1, oneDInterval3PValues[2]));
		PointList oneDInterval4P;
		oneDInterval4P.push_back(Point(1, oneDInterval4PValues[0]));
		oneDInterval4P.push_back(Point(1, oneDInterval4PValues[1]));
		oneDInterval4P.push_back(Point(1, oneDInterval4PValues[2]));
		oneDInterval4P.push_back(Point(1, oneDInterval4PValues[3]));

		PointList twoDZeroInterval;
		twoDZeroInterval.push_back(Point(2, twoDZeroIntervalValues));

		PointList twoDInterval1P;
		twoDInterval1P.push_back(Point(2, twoDInterval1PValues));
		PointList twoDInterval2P;
		twoDInterval2P.push_back(Point(2, twoDInterval2PValues[0]));
		twoDInterval2P.push_back(Point(2, twoDInterval2PValues[1]));
		twoDInterval2P.push_back(Point(2, twoDInterval2PValues[2]));
		twoDInterval2P.push_back(Point(2, twoDInterval2PValues[3]));
		PointList twoDInterval3P;
		twoDInterval3P.push_back(Point(2, twoDInterval3PValues[0]));
		twoDInterval3P.push_back(Point(2, twoDInterval3PValues[1]));
		twoDInterval3P.push_back(Point(2, twoDInterval3PValues[2]));
		twoDInterval3P.push_back(Point(2, twoDInterval3PValues[3]));
		twoDInterval3P.push_back(Point(2, twoDInterval3PValues[4]));
		twoDInterval3P.push_back(Point(2, twoDInterval3PValues[5]));
		twoDInterval3P.push_back(Point(2, twoDInterval3PValues[6]));
		twoDInterval3P.push_back(Point(2, twoDInterval3PValues[7]));
		twoDInterval3P.push_back(Point(2, twoDInterval3PValues[8]));

		UniformDatasetGenerator generator;

		// Zero dimensions (always should return empty point list)
		EXPECT_EQ(PointList(), generator.generate(0, 10, 20, 10));

		// One Dimension
		// 0 points per dimension (should always return 0)
		EXPECT_EQ(PointList(), generator.generate(1, 10, 20, 0));
		// zero-sized interval
		EXPECT_EQ(oneDZeroInterval, generator.generate(1, 2, 2, 1));
		// normal interval
		EXPECT_EQ(oneDInterval1P, generator.generate(1, 10, 20, 1));
		EXPECT_EQ(oneDInterval2P, generator.generate(1, 10, 20, 2));
		EXPECT_EQ(oneDInterval3P, generator.generate(1, 10, 20, 3));
		EXPECT_EQ(oneDInterval4P, generator.generate(1, 10, 20, 4));

		// Multiple Dimensions
		EXPECT_EQ(twoDZeroInterval, generator.generate(2, 2, 2, 1));
		EXPECT_EQ(twoDInterval1P, generator.generate(2, 10, 20, 1));
		EXPECT_EQ(twoDInterval2P, generator.generate(2, 10, 20, 2));
		EXPECT_EQ(twoDInterval3P, generator.generate(2, 10, 20, 3));
	}

	TEST_F(DatasetGeneratorTests, HyperCubeRandomGenerator)
	{
		HyperCubeRandomGenerator generator;

		Point minPoint1D(1, 1);
		Point maxPoint1D(1, 5);
		Point minPoint10D(10, 2);
		minPoint10D[2] = 3;
		Point maxPoint10D(10, 4);
		maxPoint10D[6] = 5.5; // make boundary point coordinates not all same value
		// Generate a collection of 1D and 10D points
		PointList oneDPoints = generator.generate(1, minPoint1D, maxPoint1D, 100);
		PointList tenDPoints = generator.generate(10, minPoint10D, maxPoint10D, 50);
		// Ensure enough points were returned
		ASSERT_EQ(100, oneDPoints.size());
		ASSERT_EQ(50, tenDPoints.size());
		// Ensure all points generated are within the correct bounds
		// NOTE: While the numbers are randomly generated, meaning this is not an
		// exhaustive test, it has been written to be a quick verifier of the
		// ckass' behaviour, even if it is not 100% correct
		for (PointList::const_iterator it = oneDPoints.begin(); (it != oneDPoints.end()); it++)	
		{
			const Point& p = *it;
			ASSERT_GE(p[0], minPoint1D[0]);
			ASSERT_LE(p[0], maxPoint1D[0]);
		}
		for (PointList::const_iterator it = tenDPoints.begin(); (it != tenDPoints.end()); it++)	
		{
			const Point& p = *it;
			for (unsigned int d = 0; (d < 10); d++)
			{
				ASSERT_GE(p[d], minPoint10D[d]);
				ASSERT_LE(p[d], maxPoint10D[d]);
			}
		}
	}

} }

#endif