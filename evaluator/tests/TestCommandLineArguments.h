#ifndef MDSEARCH_TEST_COMMANDLINEARGUMENTS_H
#define MDSEARCH_TEST_COMMANDLINEARGUMENTS_H

#include "CommandLineArguments.h"

namespace mdsearch { namespace tests
{

	class CommandLineArgumentsTests : public ::testing::Test
	{

	protected:
		typedef std::vector<CommandLineArguments::IndexStructureSpecification> StructureSpecList;

	};

	TEST_F(CommandLineArgumentsTests, OutputFilename)
	{
		char* testArgs1[] = { "./evaluator", "--output=filename" };
		char* testArgs2[] = { "./evaluator", "-ofilename" };
		char* testArgs3[] = { "./evaluator" };
		char* testArgs4[] = { };

		// Test with long and short versions of putput
		CommandLineArguments parsedArgs(2, testArgs1);
		EXPECT_TRUE(parsedArgs.isValid());
		EXPECT_EQ("filename", parsedArgs.resultFilename());
		parsedArgs = CommandLineArguments(2, testArgs2);
		EXPECT_TRUE(parsedArgs.isValid());
		EXPECT_EQ("filename", parsedArgs.resultFilename());
		// Test with NO output filename
		parsedArgs = CommandLineArguments(1, testArgs3);
		EXPECT_TRUE(parsedArgs.isValid());
		EXPECT_EQ("", parsedArgs.resultFilename());
		parsedArgs = CommandLineArguments(0, testArgs4);
		EXPECT_TRUE(parsedArgs.isValid());
		EXPECT_EQ("", parsedArgs.resultFilename());
	}

	TEST_F(CommandLineArgumentsTests, IndexStructures)
	{
		char* testArgs1[] = { "./evaluator", "--output=filename" };
		char* testArgs2[] = { "./evaluator", "--output=filename", "--index_structure=octree" };
		char* testArgs3[] = { "./evaluator", "--output=filename", "--index_structure=octree,2" };
		char* testArgs4[] = { "./evaluator", "--output=filename", "--index_structure=octree,2,0,100,0,100" };
		char* testArgs5[] = { "./evaluator", "--output=filename", "--index_structure=octree,2,0,100,0,100", "-ssequential_scan,10" };
		char* testArgs6[] = { "./evaluator", "--output=filename", "--index_structure=octree,r4uhr4" };

		// Test no index structures given
		CommandLineArguments parsedArgs(2, testArgs1);
		StructureSpecList structures = parsedArgs.indexStructures();
		EXPECT_TRUE(parsedArgs.isValid());
		EXPECT_EQ(0, structures.size());
		// Test single index structure w/ no args (INVALID, SO IT'S IGNORED)
		parsedArgs = CommandLineArguments(3, testArgs2);
		structures = parsedArgs.indexStructures();
		ASSERT_EQ(0, structures.size());
		// Test single index structure w/ arg (invalid dimension)
		parsedArgs = CommandLineArguments(3, testArgs6);
		structures = parsedArgs.indexStructures();
		ASSERT_EQ(0, structures.size());		
		// Test single index structure w/ arg (dimension)
		parsedArgs = CommandLineArguments(3, testArgs3);
		structures = parsedArgs.indexStructures();
		EXPECT_TRUE(parsedArgs.isValid());
		ASSERT_EQ(1, structures.size());
		EXPECT_EQ("octree", structures[0].type);
		EXPECT_EQ(2, structures[0].numDimensions);
		EXPECT_EQ(0, structures[0].arguments.size());
		// Test single index structure w/ multiple args
		parsedArgs = CommandLineArguments(3, testArgs4);
		structures = parsedArgs.indexStructures();
		EXPECT_TRUE(parsedArgs.isValid());
		ASSERT_EQ(1, structures.size());
		EXPECT_EQ("octree", structures[0].type);
		EXPECT_EQ(2, structures[0].numDimensions);
		EXPECT_EQ(4, structures[0].arguments.size());
		EXPECT_EQ("0", structures[0].arguments[0]); EXPECT_EQ("100", structures[0].arguments[1]);
		EXPECT_EQ("0", structures[0].arguments[2]); EXPECT_EQ("100", structures[0].arguments[3]);

		// Test multiple index structures
		parsedArgs = CommandLineArguments(4, testArgs5);
		structures = parsedArgs.indexStructures();
		EXPECT_TRUE(parsedArgs.isValid());
		ASSERT_EQ(2, structures.size());
		EXPECT_EQ("octree", structures[0].type);
		EXPECT_EQ(4, structures[0].arguments.size());
		EXPECT_EQ(2, structures[0].numDimensions);
		EXPECT_EQ("0", structures[0].arguments[0]); EXPECT_EQ("100", structures[0].arguments[1]);
		EXPECT_EQ("0", structures[0].arguments[2]); EXPECT_EQ("100", structures[0].arguments[3]);
		EXPECT_EQ("sequential_scan", structures[1].type);
		EXPECT_EQ(10, structures[1].numDimensions);
		EXPECT_EQ(0, structures[1].arguments.size());
	}

	TEST_F(CommandLineArgumentsTests, PreloadedDataset)
	{
		char* testArgs1[] = { "./evaluator", "--output=filename" };
		char* testArgs2[] = { "./evaluator", "--output=filename", "--preloaded_dataset=astro.dat" };
		char* testArgs3[] = { "./evaluator", "--output=filename", "-pastro.dat" };

		// Test no preloaded dataset
		CommandLineArguments parsedArgs(2, testArgs1);
		EXPECT_EQ("", parsedArgs.datasetToPreloadFilename());
		// Test one dataset with long and short option
		parsedArgs = CommandLineArguments(3, testArgs2);
		EXPECT_EQ("astro.dat", parsedArgs.datasetToPreloadFilename());
		parsedArgs = CommandLineArguments(3, testArgs3);
		EXPECT_EQ("astro.dat", parsedArgs.datasetToPreloadFilename());
	}

	TEST_F(CommandLineArgumentsTests, TestOperations)
	{
		char* testArgs1[] = { "./evaluator", "--output=filename" };
		char* testArgs2[] = { "./evaluator", "--output=filename", "--test_operations=joint_cnet.ops" };
		char* testArgs3[] = { "./evaluator", "--output=filename", "--test_operations=joint_cnet.ops", "-trandom1000.ops" };

		// Test no operation lists
		CommandLineArguments parsedArgs(2, testArgs1);
		StringList operations = parsedArgs.testOperationFilenames();
		ASSERT_EQ(0, operations.size());
		// Test single operation list
		parsedArgs = CommandLineArguments(3, testArgs2);
		operations = parsedArgs.testOperationFilenames();
		ASSERT_EQ(1, operations.size());
		EXPECT_EQ("joint_cnet.ops", operations[0]);
		// Test multiple operation lists
		parsedArgs = CommandLineArguments(4, testArgs3);
		operations = parsedArgs.testOperationFilenames();
		ASSERT_EQ(2, operations.size());
		EXPECT_EQ("joint_cnet.ops", operations[0]);
		EXPECT_EQ("random1000.ops", operations[1]);
	}

	TEST_F(CommandLineArgumentsTests, Verbose)
	{
		char* testArgs1[] = { "./evaluator", "--output=filename" };
		char* testArgs2[] = { "./evaluator", "--output=filename", "-v" };
		char* testArgs3[] = { "./evaluator", "--output=filename", "--verbose" };

		CommandLineArguments parsedArgs(2, testArgs1);
		ASSERT_FALSE(parsedArgs.isVerbose());
		parsedArgs = CommandLineArguments(3, testArgs2);
		ASSERT_TRUE(parsedArgs.isVerbose());
		parsedArgs = CommandLineArguments(3, testArgs3);
		ASSERT_TRUE(parsedArgs.isVerbose());
	}

	TEST_F(CommandLineArgumentsTests, ProfileHeap)
	{
		char* testArgs1[] = { "./evaluator" };
		char* testArgs2[] = { "./evaluator", "--heap" };

		CommandLineArguments parsedArgs(1, testArgs1);
		ASSERT_FALSE(parsedArgs.profileHeap());
		parsedArgs = CommandLineArguments(2, testArgs2);
		ASSERT_TRUE(parsedArgs.profileHeap());
	}

	TEST_F(CommandLineArgumentsTests, TestRunCount)
	{
		char* testArgs1[] = { "./evaluator" };
		char* testArgs2[] = { "./evaluator", "-r3" };
		char* testArgs3[] = { "./evaluator", "--runs=8" };
		CommandLineArguments parsedArgs(1, testArgs1);
		EXPECT_EQ(CommandLineArguments::DEFAULT_TEST_RUNS, parsedArgs.testRunsToPerform());
		parsedArgs = CommandLineArguments(2, testArgs2);
		EXPECT_EQ(3, parsedArgs.testRunsToPerform());
		parsedArgs = CommandLineArguments(2, testArgs3);
		EXPECT_EQ(8, parsedArgs.testRunsToPerform());
	}

	TEST_F(CommandLineArgumentsTests, ProfileCPU)
	{
		char* testArgs1[] = { "./evaluator" };
		char* testArgs2[] = { "./evaluator", "--cpu" };

		CommandLineArguments parsedArgs(1, testArgs1);
		ASSERT_FALSE(parsedArgs.profileCPU());
		parsedArgs = CommandLineArguments(2, testArgs2);
		ASSERT_TRUE(parsedArgs.profileCPU());
	}

	TEST_F(CommandLineArgumentsTests, IndividualOperationDatasets)
	{
		char* testArgs1[] = { "./evaluator" };
		char* testArgs2[] = { "./evaluator", "-isome_dataset.ops" };
		char* testArgs3[] = { "./evaluator", "--individual_optest_datasets=some_dataset.ops" };
		char* testArgs4[] = { "./evaluator", "-isome_dataset.ops", "--individual_optest_datasets=another_dataset.ops" };

		CommandLineArguments parsedArgs(1, testArgs1);
		ASSERT_EQ(0, parsedArgs.individualOpDatasetFilenames().size());
		parsedArgs = CommandLineArguments(2, testArgs2);
		ASSERT_EQ(1, parsedArgs.individualOpDatasetFilenames().size());
		ASSERT_EQ("some_dataset.ops", parsedArgs.individualOpDatasetFilenames()[0]);
		parsedArgs = CommandLineArguments(2, testArgs3);
		ASSERT_EQ(1, parsedArgs.individualOpDatasetFilenames().size());
		ASSERT_EQ("some_dataset.ops", parsedArgs.individualOpDatasetFilenames()[0]);
		parsedArgs = CommandLineArguments(3, testArgs4);
		ASSERT_EQ(2, parsedArgs.individualOpDatasetFilenames().size());
		ASSERT_EQ("some_dataset.ops", parsedArgs.individualOpDatasetFilenames()[0]);
		ASSERT_EQ("another_dataset.ops", parsedArgs.individualOpDatasetFilenames()[1]);
	}

	TEST_F(CommandLineArgumentsTests, PointCounts)
	{
		char* testArgs1[] = { "./evaluator" };
		char* testArgs2[] = { "./evaluator", "-c2" };
		char* testArgs3[] = { "./evaluator", "--point_counts=5" };
		char* testArgs4[] = { "./evaluator", "-c2", "-c5", "--point_counts=123" };
		char* testArgs5[] = { "./evaluator", "-c2", "-chahaha" };

		CommandLineArguments parsedArgs(1, testArgs1);
		ASSERT_TRUE(parsedArgs.isValid());
		ASSERT_EQ(0, parsedArgs.pointCountsToSample().size());
		parsedArgs = CommandLineArguments(2, testArgs2);
		ASSERT_TRUE(parsedArgs.isValid());
		ASSERT_EQ(1, parsedArgs.pointCountsToSample().size());
		ASSERT_EQ(2, parsedArgs.pointCountsToSample()[0]);
		parsedArgs = CommandLineArguments(2, testArgs3);
		ASSERT_TRUE(parsedArgs.isValid());
		ASSERT_EQ(1, parsedArgs.pointCountsToSample().size());
		ASSERT_EQ(5, parsedArgs.pointCountsToSample()[0]);
		parsedArgs = CommandLineArguments(4, testArgs4);
		ASSERT_TRUE(parsedArgs.isValid());
		ASSERT_EQ(3, parsedArgs.pointCountsToSample().size());
		ASSERT_EQ(2, parsedArgs.pointCountsToSample()[0]);
		ASSERT_EQ(5, parsedArgs.pointCountsToSample()[1]);
		ASSERT_EQ(123, parsedArgs.pointCountsToSample()[2]);
		// Test invalid count (non-integer given)
		parsedArgs = CommandLineArguments(3, testArgs5);
		ASSERT_FALSE(parsedArgs.isValid());
	}

} }

#endif