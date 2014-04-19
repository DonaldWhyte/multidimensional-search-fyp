#ifndef MDSEARCH_TEST_TESTSUITE_H
#define MDSEARCH_TEST_TESTSUITE_H

#include "CommandLineArguments.h"

namespace mdsearch { namespace tests
{

	class TestSuiteTests : public ::testing::Test
	{

	};

	TEST_F(TestSuiteTests, FromFile)
	{
		ASSERT_TRUE(false);
	}

} }

#endif