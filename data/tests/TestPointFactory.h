#ifndef MDSEARCH_TEST_POINTFACTORY_H
#define MDSEARCH_TEST_POINTFACTORY_H

#include <gtest/gtest.h>
#include "PointFactory.h"

namespace mdsearch { namespace tests
{

	class PointFactoryTests : public ::testing::Test
	{

	};

	TEST_F(PointFactoryTests, CreatePoint)
	{
		Real oneDPointValue = 6;
		Real twoDPointValues[] = { 4, 3 };

		// TODO: fix SEG FAULT that occurs whren comparing points with
		// zero-dimensions to points with > 0 dimensions

		// Test with pool size of zero
		PointFactory zeroSizeFactory(0);
		ASSERT_EQ(WeakPoint(0, NULL), zeroSizeFactory.createPoint(1, &oneDPointValue));
		// Test with pool that has enough space for a three reals
		PointFactory smallFactory(3);
		ASSERT_EQ(WeakPoint(1, &oneDPointValue), smallFactory.createPoint(1, &oneDPointValue));
		ASSERT_EQ(WeakPoint(1, &oneDPointValue), smallFactory.createPoint(1, &oneDPointValue));
		ASSERT_EQ(WeakPoint(0, NULL), smallFactory.createPoint(2, twoDPointValues));
		// Test with pool that can fit more points in
		PointFactory factory(2000);
		for (unsigned int i = 0; (i < 1000); i++)
		{
			ASSERT_EQ(WeakPoint(2, twoDPointValues), factory.createPoint(2, twoDPointValues));
		}
		ASSERT_EQ(WeakPoint(0, NULL), factory.createPoint(2, twoDPointValues));
	}

	TEST_F(PointFactoryTests, Clear)
	{
		Real oneDPointValue = 6;
		Real twoDPointValues[] = { 4, 3 };

		PointFactory factory(3);
		ASSERT_EQ(WeakPoint(1, &oneDPointValue), factory.createPoint(1, &oneDPointValue));
		ASSERT_EQ(WeakPoint(1, &oneDPointValue), factory.createPoint(1, &oneDPointValue));
		ASSERT_EQ(WeakPoint(0, NULL), factory.createPoint(2, twoDPointValues));
		factory.clear();
		ASSERT_EQ(WeakPoint(2, twoDPointValues), factory.createPoint(2, twoDPointValues));
		ASSERT_EQ(WeakPoint(0, NULL), factory.createPoint(2, twoDPointValues));
	}

} }

#endif