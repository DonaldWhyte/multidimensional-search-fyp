#include <gtest/gtest.h>
// By including all the header files which contain the unit
// tests, these tests will automatically be added to the
// test suite that executes when RUN_ALL_TESTS() is called
#include "TestSequentialScan.h"
#include "TestOctree.h"
#include "TestPyramidTree.h"
#include "TestIndexPyramidTree.h"
#include "TestSplayTree.h"
#include "TestSplayPyramidTree.h"
#include "TestParallelSequentialScan.h"
#include "TestPseudoPyramidTree.h"
#include "TestBoundaryDistanceHashing.h"
#include "TestKDTree.h"

int main(int argc, char* argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}