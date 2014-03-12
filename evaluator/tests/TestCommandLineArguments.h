#ifndef MDSEARCH_TEST_COMMANDLINEARGUMENTS_H
#define MDSEARCH_TEST_COMMANDLINEARGUMENTS_H

#include "CommandLineArguments.h"

namespace mdsearch { namespace tests
{

	class CommandLineArgumentsTests : public ::testing::Test
	{


	};

	TEST_F(CommandLineArgumentsTests, Core) // required arguments
	{
		char* testArgs1[] = { "./evaluator", "--output=filename" };
		char* testArgs2[] = { "--output=filename" };
		char* testArgs3[] = { "./evaluator" };
		char* testArgs4[] = { };

		// Test with program name and without
		CommandLineArguments parsedArgs(2, testArgs1);
		EXPECT_TRUE(parsedArgs.isValid());
		EXPECT_EQ(parsedArgs.resultFilename(), "filename");
		parsedArgs = CommandLineArguments(1, testArgs2);
		EXPECT_TRUE(parsedArgs.isValid());
		EXPECT_EQ(parsedArgs.resultFilename(), "filename");
		// Test with NO output filename
		parsedArgs = CommandLineArguments(1, testArgs3);
		EXPECT_FALSE(parsedArgs.isValid());
		EXPECT_EQ(parsedArgs.resultFilename(), "");
		parsedArgs = CommandLineArguments(0, testArgs4);
		EXPECT_FALSE(parsedArgs.isValid());
		EXPECT_EQ(parsedArgs.resultFilename(), "");
	}

	TEST_F(CommandLineArgumentsTests, IndexStructures)
	{
		// TODO
	}

	TEST_F(CommandLineArgumentsTests, Datasets)
	{
		// TODO
	}

	TEST_F(CommandLineArgumentsTests, Operations)
	{
		// TODO
	}

} }

#endif