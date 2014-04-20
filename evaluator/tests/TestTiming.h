#ifndef MDSEARCH_TEST_TIMING_H
#define MDSEARCH_TEST_TIMING_H

#include "Timing.h"

namespace mdsearch { namespace tests {

	class TimingTests : public ::testing::Test
	{

	};

	TEST_F(TimingTests, TimingsToString)
	{
		static const std::string EXPECTED_TIME_STRING =
			"kdtree delete\n1 4\n10 4.1\n100 4.2\n\nkdtree insert\n1 2\n10 2.1\n100 2.2\n\n";

		TimingCollection collection;
		TimingMap times;
		times["1"] = 2.0;
		times["10"] = 2.1;
		times["100"] = 2.2;
		collection["kdtree insert"] = times;
		times["1"] = 4.0;
		times["10"] = 4.1;
		times["100"] = 4.2;
		collection["kdtree delete"] = times;

		TimingWriter writer;
		EXPECT_EQ(EXPECTED_TIME_STRING, writer.timesToString(collection));
	}

} }

#endif