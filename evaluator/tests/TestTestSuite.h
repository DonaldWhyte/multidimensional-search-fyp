#ifndef MDSEARCH_TEST_TESTSUITE_H
#define MDSEARCH_TEST_TESTSUITE_H

#include "TestSuite.h"

namespace mdsearch { namespace tests
{

	class TestSuiteTests : public ::testing::Test
	{

	};

	TEST_F(TestSuiteTests, FromFile)
	{
		TestSuite suite = TestSuite::fromFile("test_files/test.suite");

		EXPECT_EQ("testsuite", suite.name());
		EXPECT_EQ("/tmp/inputData/", suite.inputDirectory());
		EXPECT_EQ("/var/log/", suite.outputDirectory());
		EXPECT_EQ(3, suite.numRunsPerTiming());
		
		const std::vector<IndexStructureSpecification>& structureSpecs = suite.structureSpecs();
		ASSERT_EQ(3, structureSpecs.size());
		EXPECT_EQ("kdtree", structureSpecs[0].type);
		EXPECT_EQ(0, structureSpecs[0].arguments.size());
		EXPECT_EQ("pyramid_tree", structureSpecs[1].type);
		EXPECT_EQ(0, structureSpecs[1].arguments.size());
		EXPECT_EQ("index_pseudo_pyramid_tree", structureSpecs[2].type);
		ASSERT_EQ(2, structureSpecs[2].arguments.size());
		EXPECT_EQ("9000", structureSpecs[2].arguments[0]);
		EXPECT_EQ("defragment", structureSpecs[2].arguments[1]);

		const std::vector<DatasetSpecification>& dsSpecs = suite.datasetSpecs();
		ASSERT_EQ(2, dsSpecs.size());
		EXPECT_EQ("randuniform", dsSpecs[0].name);
		EXPECT_EQ("Dimensions", dsSpecs[0].varyingParameter);
		ASSERT_EQ(3, dsSpecs[0].subDatasets.size());
		EXPECT_EQ("1", dsSpecs[0].subDatasets[0].name);
		EXPECT_EQ("something.dat", dsSpecs[0].subDatasets[0].filename);
		EXPECT_EQ("5", dsSpecs[0].subDatasets[1].name);
		EXPECT_EQ("something.dat", dsSpecs[0].subDatasets[1].filename);
		EXPECT_EQ("10", dsSpecs[0].subDatasets[2].name);
		EXPECT_EQ("something.dat", dsSpecs[0].subDatasets[2].filename);
		EXPECT_EQ("astrophysics", dsSpecs[1].name);
		EXPECT_EQ("", dsSpecs[1].varyingParameter);
		ASSERT_EQ(3, dsSpecs[1].subDatasets.size());
		EXPECT_EQ("10000", dsSpecs[1].subDatasets[0].name);
		EXPECT_EQ("astrophysics_10000.dat", dsSpecs[1].subDatasets[0].filename);
		EXPECT_EQ("50000", dsSpecs[1].subDatasets[1].name);
		EXPECT_EQ("astrophysics_50000.dat", dsSpecs[1].subDatasets[1].filename);
		EXPECT_EQ("100000", dsSpecs[1].subDatasets[2].name);
		EXPECT_EQ("astrophysics_100000.dat", dsSpecs[1].subDatasets[2].filename);
	}

} }

#endif