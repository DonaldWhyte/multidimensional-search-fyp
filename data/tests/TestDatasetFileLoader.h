#ifndef MDSEARCH_TEST_DATASETFILELOADER_H
#define MDSEARCH_TEST_DATASETFILELOADER_H

#include <gtest/gtest.h>
#include "DatasetFileLoader.h"

namespace mdsearch { namespace tests
{

	class DatasetFileLoaderTests : public ::testing::Test
	{

	};

	TEST_F(DatasetFileLoaderTests, InvalidData)
	{
		DatasetFileLoader loader;

		EXPECT_EQ(loader.load("test_files/does_not_exist.dat"), PointList());
		EXPECT_EQ(loader.load("test_files/invalid_syntax.dat"), PointList());
		EXPECT_EQ(loader.load("test_files/invalid_dimension.dat"), PointList());

		static const unsigned int NUM_NOT_ENOUGH_DATAPOINTS = 4;
		Real notEnoughDataPointValues[NUM_NOT_ENOUGH_DATAPOINTS][3] = {
			{ 4, 5, 2 }, { 2, 3, 3 },
			{ 2.4, 5, 2.2 }, { 0, 0, 0 }
		};
		PointList notEnoughDataPoints;
		for (unsigned int i = 0; (i < NUM_NOT_ENOUGH_DATAPOINTS); i++)
			notEnoughDataPoints.push_back(Point(3, notEnoughDataPointValues[i]));
		std::cout << "NOT ENOUGH DATA POINTS: " << notEnoughDataPoints.size() << std::endl;
		EXPECT_EQ(loader.load("test_files/not_enough.dat"), notEnoughDataPoints);
	}

	TEST_F(DatasetFileLoaderTests, ValidData)
	{
		DatasetFileLoader loader;

		EXPECT_EQ(loader.load("test_files/empty.dat"), PointList());

		PointList singleDataPoint;
		Real singlePointValues[] = { 4, 6, 2, 5 };
		singleDataPoint.push_back(Point(4, singlePointValues));
		EXPECT_EQ(loader.load("test_files/single.dat"), singleDataPoint);

		static const unsigned int NUM_MULTIPLE_DATAPOINTS = 5;
		Real multipleDataPointValues[NUM_MULTIPLE_DATAPOINTS][3] = {
			{ 0, 0, 0 }, { 1, 1, 1 }, { 2, 2, 2 },
			{ 5.5, 405, -2.42 }, { 3.2, 6.7, -5}
		};
		PointList multipleDataPoints;
		for (unsigned int i = 0; (i < NUM_MULTIPLE_DATAPOINTS); i++)
			multipleDataPoints.push_back(Point(3, multipleDataPointValues[i]));		
		EXPECT_EQ(loader.load("test_files/multiple.dat"), multipleDataPoints);
	}

} }

#endif