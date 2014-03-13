#include "Evaluator.h"
#include <ctime>
#include <sstream>
#include <boost/timer.hpp>

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
				Timing timeElapsed = runOperations(*structure, *ops);
				structureTimings.push_back(timeElapsed);

				// TODO: need to clear index structure here!!!
			}

			testOpTimings.push_back(structureTimings);
		}

		return testOpTimings;
	}

	Timing Evaluator::runOperations(IndexStructure* structure,
		const TestOperationList& operations) const
	{
		// Keep track of the time the performance test starts
		boost::timer timer;
		timer.restart();

		// Iteration through all operations
		for (TestOperationList::const_iterator op = operations.begin();
			(op != operations.end()); op++)
		{
			switch (op->type)
			{
			case TestOperation::OPERATION_TYPE_INSERT:
				structure->insert(op->value);
				break;
			case TestOperation::OPERATION_TYPE_DELETE:
				structure->remove(op->value);
				break;
			case TestOperation::OPERATION_TYPE_UPDATE:
				// TODO: handle update later!
				//structure->update(op->value);
				break;
			case TestOperation::OPERATION_TYPE_POINTQUERY:
				structure->pointExists(op->value);
				break;
			}
		}

		// Compute elapsed time and return it
		return timer.elapsed();
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
				Timing elapsedTime = structureTimings[structureIndex];
				ss << "\t\tStructure (" << structureIndex << "): " << elapsedTime << " seconds\n";
			}
		}

		return ss.str();
	}

}