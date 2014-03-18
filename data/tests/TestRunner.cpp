#include <gtest/gtest.h>
// By including all the header files which contain the unit
// tests, these tests will automatically be added to the
// test suite that executes when RUN_ALL_TESTS() is called
#include "TestTypes.h"
#include "TestPointFactory.h"
#include "TestDatasetFileLoader.h"
#include "TestDatasetGenerators.h"

int main(int argc, char* argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}