#ifndef MDSEARCH_TEST_TESTOPERATIONLOADER_H
#define MDSEARCH_TEST_TESTOPERATIONLOADER_H

#include "TestOperationLoader.h"

namespace mdsearch { namespace tests
{

	class TestOperationLoaderTests : public ::testing::Test
	{

	};

	TEST_F(TestOperationLoaderTests, InvalidData)
	{
		TestOperationLoader loader;

		EXPECT_EQ(TestOperationList(), loader.loadFromFile("test_files/does_not_exist.ops"));
		EXPECT_EQ(TestOperationList(), loader.loadFromFile("test_files/invalid_syntax.ops"));
		EXPECT_EQ(TestOperationList(), loader.loadFromFile("test_files/invalid_header.ops"));

		TestOperationList notEnoughOperations;
		notEnoughOperations.push_back(TestOperation(
			TestOperation::OPERATION_TYPE_INSERT, Point(3, 2)));
		notEnoughOperations.push_back(TestOperation(
			TestOperation::OPERATION_TYPE_DELETE, Point(3, 1.5)));
		notEnoughOperations.push_back(TestOperation(
			TestOperation::OPERATION_TYPE_POINTQUERY, Point(3, 3.2)));

		EXPECT_EQ(notEnoughOperations, loader.loadFromFile("test_files/not_enough.ops"));
	}

	TEST_F(TestOperationLoaderTests, ValidData)
	{
		TestOperationLoader loader;

		EXPECT_EQ(TestOperationList(), loader.loadFromFile("test_files/empty.ops"));

		TestOperationList singleOperation;
		singleOperation.push_back(TestOperation(
			TestOperation::OPERATION_TYPE_INSERT, Point(4, 3)));
		EXPECT_EQ(singleOperation, loader.loadFromFile("test_files/single.ops"));

		Real varyingPointValues[] = { 2.42, 0, -53.5, 5 }; // done so point loading can be fully tested
		TestOperationList multipleOperations;
		multipleOperations.push_back(TestOperation(
			TestOperation::OPERATION_TYPE_INSERT, Point(4, varyingPointValues)));
		multipleOperations.push_back(TestOperation(
			TestOperation::OPERATION_TYPE_DELETE, Point(4, 1)));
		multipleOperations.push_back(TestOperation(
			TestOperation::OPERATION_TYPE_UPDATE, Point(4, 2.242), Point(4, -4)));
		multipleOperations.push_back(TestOperation(
			TestOperation::OPERATION_TYPE_POINTQUERY, Point(4, 2473.64)));
		multipleOperations.push_back(TestOperation(
			TestOperation::OPERATION_TYPE_INSERT, Point(4, -535)));
		EXPECT_EQ(multipleOperations, loader.loadFromFile("test_files/multiple.ops"));
	}

} }

#endif