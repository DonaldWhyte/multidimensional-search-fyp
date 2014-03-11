#ifndef MDSEARCH_TEST_DATASETGENERATORS_H
#define MDSEARCH_TEST_DATASETGENERATORS_H

#include <gtest/gtest.h>
#include "UniformDatasetGenerator.h"
#include "RandomPointGenerator.h"
#include "ClusteredDatasetGenerator.h"
#include "SkewedDatasetGenerator.h"

namespace mdsearch { namespace tests
{

	// Constant which determines how many poins to generate when
	// testing genewrators based on randomness
	static const unsigned int NUM_RANDOM_POINTS_TO_GENERATE = 1000;

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

	TEST_F(DatasetGeneratorTests, RandomPointsInRegions)
	{
		RandomPointGenerator generator;
		// Construct test data
		Region region1D(1, Interval(1, 5));
		Region region10D(10, Interval(2, 4));
		region10D[2].min = 3;
		region10D[6].max = 5.5; // make boundary point coordinates not all same value
		Region zeroSizedRegion(1, Interval(8, 8));

		// Generate a collection of 1D and 10D points
		PointList oneDPoints = generator.generatePointsInRegion(1, region1D, NUM_RANDOM_POINTS_TO_GENERATE);
		PointList tenDPoints = generator.generatePointsInRegion(10, region10D, NUM_RANDOM_POINTS_TO_GENERATE);
		// Ensure enough points were returned
		ASSERT_EQ(NUM_RANDOM_POINTS_TO_GENERATE, oneDPoints.size());
		ASSERT_EQ(NUM_RANDOM_POINTS_TO_GENERATE, tenDPoints.size());
		// Ensure all points generated are within the correct bounds
		// NOTE: While the numbers are randomly generated, meaning this is not an
		// exhaustive test, it has been written to be a quick verifier of the
		// ckass' behaviour, even if it is not 100% correct
		for (PointList::const_iterator it = oneDPoints.begin(); (it != oneDPoints.end()); it++)	
		{
			const Point& p = *it;
			ASSERT_GE(p[0], region1D[0].min);
			ASSERT_LE(p[0], region1D[0].max);
		}
		for (PointList::const_iterator it = tenDPoints.begin(); (it != tenDPoints.end()); it++)	
		{
			const Point& p = *it;
			for (unsigned int d = 0; (d < 10); d++)
			{
				ASSERT_GE(p[d], region10D[d].min);
				ASSERT_LE(p[d], region10D[d].max);
			}
		}

		// Test with zero-sized hyper-cube (same point should always be generated)
		PointList samePoints = generator.generatePointsInRegion(1, zeroSizedRegion, 10);
		for (PointList::const_iterator it = samePoints.begin(); (it != samePoints.end()); it++)	
		{
			ASSERT_EQ(zeroSizedRegion[0].max, (*it)[0]);
		}
	}

	TEST_F(DatasetGeneratorTests, ClusteredData)
	{
		// NOTE: Tests which validate the generated points are actually clustered
		// in space will be in the RandomPointGenerator class tests, as the clustered
		// points are actually generated using that.

		ClusteredDatasetGenerator generator;

		// Construct test data
		Region testRegions[] = {
			Region(3, Interval(1, 10)),
			Region(3, Interval(84, 132)),
			Region(3, Interval(50, 50))
		};
		Point zeroSizedRegionPoint = Point(3, 50);

		std::vector<ClusterSpecification> oneCluster;
		oneCluster.push_back( ClusterSpecification(testRegions[0], 10) );
		std::vector<ClusterSpecification> twoClusters;
		twoClusters.push_back( ClusterSpecification(testRegions[0], 10) );
		twoClusters.push_back( ClusterSpecification(testRegions[1], 100) );
		std::vector<ClusterSpecification> twoClustersWithZeroSized;
		twoClustersWithZeroSized.push_back( ClusterSpecification(testRegions[1], 100) );
		twoClustersWithZeroSized.push_back( ClusterSpecification(testRegions[2], 5) );

		// Test no cluster specifications given
		EXPECT_EQ(PointList(), generator.generate(3, std::vector<ClusterSpecification>()));
		// Test one cluster
		PointList oneClusterPoints = generator.generate(3, oneCluster); 
		EXPECT_EQ(10, oneClusterPoints.size());
		// Test two clusters
		PointList twoClusterPoints = generator.generate(3, twoClusters);
		EXPECT_EQ(110, twoClusterPoints.size());
		// Test two clusters, one with zero-sized region
		PointList twoClustersWithZeroSizedPoints = generator.generate(3, twoClustersWithZeroSized);
		EXPECT_EQ(105, twoClustersWithZeroSizedPoints.size());
		// Ensure zero-sized region's points were generated correctly
		for (unsigned int i = 100; (i < 105); i++)
			ASSERT_EQ(twoClustersWithZeroSizedPoints[i], zeroSizedRegionPoint);
	}

	TEST_F(DatasetGeneratorTests, SkewedDatasetGenerator)
	{
		SkewedDatasetGenerator generator;
		SkewDirection skewDir = SKEW_DIRECTION_LEFT;
		Real skewSeverity = 3.0;

		Point minPoint1D(1, 1);
		Point maxPoint1D(1, 5);
		Point minPoint10D(10, 2);
		minPoint10D[2] = 3;
		Point maxPoint10D(10, 4);
		maxPoint10D[6] = 5.5;
		// Generate a collection of 1D and 10D points
		PointList oneDPoints = generator.generate(1, minPoint1D, maxPoint1D,
			NUM_RANDOM_POINTS_TO_GENERATE, skewSeverity, skewDir);
		PointList tenDPoints = generator.generate(10, minPoint10D, maxPoint10D,
			NUM_RANDOM_POINTS_TO_GENERATE, skewSeverity, skewDir);
		// Ensure enough points were returned
		ASSERT_EQ(NUM_RANDOM_POINTS_TO_GENERATE, oneDPoints.size());
		ASSERT_EQ(NUM_RANDOM_POINTS_TO_GENERATE, tenDPoints.size());
		// Ensure all points generated are within the correct bounds
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
		// Zero-sized range
		PointList samePoints = generator.generate(1, minPoint1D, minPoint1D, 10, skewSeverity, skewDir);
		for (PointList::const_iterator it = samePoints.begin(); (it != samePoints.end()); it++)	
		{
			ASSERT_EQ(minPoint1D, *it);
		}

		// Repeat above tests where the data is skewed towards the upper values
		skewDir = SKEW_DIRECTION_RIGHT;
		oneDPoints = generator.generate(1, minPoint1D, maxPoint1D,
			NUM_RANDOM_POINTS_TO_GENERATE, skewSeverity, skewDir);
		tenDPoints = generator.generate(10, minPoint10D, maxPoint10D,
			NUM_RANDOM_POINTS_TO_GENERATE, skewSeverity, skewDir);
		ASSERT_EQ(NUM_RANDOM_POINTS_TO_GENERATE, oneDPoints.size());
		ASSERT_EQ(NUM_RANDOM_POINTS_TO_GENERATE, tenDPoints.size());
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

		samePoints = generator.generate(1, minPoint1D, minPoint1D, 10, skewSeverity, skewDir);
		for (PointList::const_iterator it = samePoints.begin(); (it != samePoints.end()); it++)	
		{
			ASSERT_EQ(minPoint1D, *it);
		}		
	}

} }

#endif