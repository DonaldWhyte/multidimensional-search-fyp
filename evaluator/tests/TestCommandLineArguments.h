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

	TEST_F(CommandLineArgumentsTests, Core) // required arguments
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
		EXPECT_FALSE(parsedArgs.isValid());
		EXPECT_EQ("", parsedArgs.resultFilename());
		parsedArgs = CommandLineArguments(0, testArgs4);
		EXPECT_FALSE(parsedArgs.isValid());
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

	TEST_F(CommandLineArgumentsTests, Datasets)
	{
		char* testArgs1[] = { "./evaluator", "--output=filename" };
		char* testArgs2[] = { "./evaluator", "--output=filename", "--dataset=astro.dat" };
		char* testArgs3[] = { "./evaluator", "--output=filename", "--dataset=astro.dat", "-dclustered100.dat" };

		// Test no datasets
		CommandLineArguments parsedArgs(2, testArgs1);
		StringList datasets = parsedArgs.datasetFilenames();
		ASSERT_EQ(0, datasets.size());
		// Test one dataset
		parsedArgs = CommandLineArguments(3, testArgs2);
		datasets = parsedArgs.datasetFilenames();
		ASSERT_EQ(1, datasets.size());
		EXPECT_EQ("astro.dat", datasets[0]);
		// Test multiple datasets
		parsedArgs = CommandLineArguments(4, testArgs3);
		datasets = parsedArgs.datasetFilenames();
		ASSERT_EQ(2, datasets.size());
		EXPECT_EQ("astro.dat", datasets[0]);
		EXPECT_EQ("clustered100.dat", datasets[1]);
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

} }

#endif