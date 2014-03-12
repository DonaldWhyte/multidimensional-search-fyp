#include "Evaluator.h"
#include <ctime>

namespace mdsearch
{

	Evaluator::Evaluator(const std::vector<IndexStructure*>& structures)
		: structures(structures)
	{
	}

	std::vector<std::vector<TimingList> > Evaluator::timePerformance(
		const std::vector<PointList>& datasets,
		const std::vector<TestOperationList>& testOperationLists) const
	{
		// NOTE: reserve() calls are done to minimise the amount of dynamic allocations as possible

		std::vector<std::vector<TimingList> > timings;
		timings.reserve(datasets.size());

		// For each dataset, run the specified sets of test operations
		for (std::vector<PointList>::const_iterator ds = datasets.begin();
			(ds != datasets.end()); ds++)
		{
			std::vector<TimingList> datasetTimings;
			datasetTimings.reserve(testOperationLists.size());

			for (std::vector<TestOperationList>::const_iterator ops = testOperationLists.begin();
				(ops != testOperationLists.end()); ops++)
			{
				TimingList testOperationSetTimings;
				testOperationSetTimings.reserve(structures.size());

				for (std::vector<IndexStructure*>::const_iterator structure = structures.begin();
					(structure != structures.end()); structure++)
				{
					// Run operations
					long timeElapsed = runOperations(*structure, *ds, *ops);
					testOperationSetTimings.push_back(timeElapsed);

					// TODO: need to clear index structure here!!!
				}

				datasetTimings.push_back(testOperationSetTimings);
			}

			timings.push_back(datasetTimings);
		}

		return timings;
	}

	long Evaluator::runOperations(IndexStructure* structure,
		const PointList& dataset, const TestOperationList& operations) const
	{
		// Keep track of the time the performance test starts
		long startTime = time(NULL);

		// TODO 

		// ECompute elapsed time and return it
		return (time(NULL) - startTime);
	}

}