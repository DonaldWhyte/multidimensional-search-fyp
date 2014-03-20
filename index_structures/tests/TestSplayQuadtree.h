#ifndef MDSEARCH_TEST_SPLAYQUADTREE_H
#define MDSEARCH_TEST_SPLAYQUADTREE_H

#include <gtest/gtest.h>
#include "SplayQuadtree.h"

namespace mdsearch { namespace tests {

	static const unsigned int NUM_SPLAYQUADTREE_DIMENSIONS = 3;

	class SplayQuadtreeTests : public ::testing::Test
	{

	protected:
		virtual void SetUp()
		{
			Interval initialBoundaryIntervals[3] = { Interval(0, 10), Interval(0, 10), Interval(0, 10) };
			initialBoundary = Region(3, initialBoundaryIntervals);
		}

		Region initialBoundary;

	};



} }

#endif