#include "Evaluator.h"
#include <ctime>
#include <sstream>

namespace mdsearch
{

	Evaluator::Evaluator(const std::vector<IndexStructure*>& structures)
		: structures(structures)
	{
	}

	OperationListTimings Evaluator::timePerformance(
		const std::vector<TestOperationList>& testOperationLists) const
	{
		// NOTE: reserve() calls are done to minimise the amount of dynamic allocations as possible

		OperationListTimings testOpTimings;
		testOpTimings.reserve(testOperationLists.size());

		for (std::vector<TestOperationList>::const_iterator ops = testOperationLists.begin();
			(ops != testOperationLists.end()); ops++)
		{
			StructureTimings structureTimings;
			structureTimings.reserve(structures.size());

			for (std::vector<IndexStructure*>::const_iterator structure = structures.begin();
				(structure != structures.end()); structure++)
			{
				// Run operations
				long timeElapsed = runOperations(*structure, *ops);
				structureTimings.push_back(timeElapsed);

				// TODO: need to clear index structure here!!!
			}

			testOpTimings.push_back(structureTimings);
		}

		return testOpTimings;
	}

	long Evaluator::runOperations(IndexStructure* structure,
		const TestOperationList& operations) const
	{
		// Keep track of the time the performance test starts
		long startTime = time(NULL);

		// TODO 

		// Compute elapsed time and return it
		return (time(NULL) - startTime);
	}

	std::string generateTimingReport(const OperationListTimings& timings)
	{
		std::stringstream ss;

		for (unsigned int operationListIndex = 0; (operationListIndex < timings.size()); operationListIndex++)
		{
			ss << "Dataset and Operation List (" << operationListIndex << "):" << "\n";

			const StructureTimings& structureTimings = timings[operationListIndex];
			for (unsigned int structureIndex = 0; (structureIndex < structureTimings.size()); structureIndex++)
			{
				long elapsedTime = structureTimings[structureIndex];
				ss << "\t\tStructure (" << structureIndex << "): " << elapsedTime << "\n";
			}
		}

		return ss.str();
	}

}