#ifndef MDSEARCH_TEST_DATATYPES_H
#define MDSEARCH_TEST_DATATYPES_H

#include <gtest/gtest.h>
#include "Point.h"
#include "Region.h"

namespace mdsearch { namespace tests
{

	class DataTypeTests : public ::testing::Test
	{

	protected:
		DataTypeTests() { }
		virtual ~DataTypeTests() { }

	};

	TEST_F(DataTypeTests, Add) {
		EXPECT_EQ(1 + 2, 3);
	}

} }

#endif