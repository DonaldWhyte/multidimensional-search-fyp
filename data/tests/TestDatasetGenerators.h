#ifndef MDSEARCH_TEST_DATASETGENERATORS_H
#define MDSEARCH_TEST_DATASETGENERATORS_H

#include <gtest/gtest.h>
#include "UniformDatasetGenerator.h"

namespace mdsearch { namespace tests
{

	class UniformDatasetGeneratorTests : public ::testing::Test
	{

	};

	TEST_F(UniformDatasetGeneratorTests, generate)
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

} }

#endif