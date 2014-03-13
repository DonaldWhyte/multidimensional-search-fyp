#include "Evaluator.h"
#include <ctime>
#include <sstream>
#include <boost/timer.hpp>
#include <gperftools/profiler.h>
#include <gperftools/heap-profiler.h>

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

		for (unsigned int t = 0; (t < testOperationLists.size()); t++)
		{
			StructureTimings structureTimings;
			structureTimings.reserve(structures.size());

			for (unsigned int s = 0; (s < structures.size()); s++)
			{
				// Ensure structure is completely empty
				structures[s]->clear();				
				// Run test operations on structure
				Timing timeElapsed = runOperations(structures[s],
					testOperationLists[t],
					generateCPUProfilerFilename(t, s),
					generateHeapProfilerFilename(t, s)
				);
				structureTimings.push_back(timeElapsed);
			}

			testOpTimings.push_back(structureTimings);
		}

		return testOpTimings;
	}

	Timing Evaluator::runOperations(IndexStructure* structure,
		const TestOperationList& operations,
		const std::string& cpuProfilerOutputFilename,
		const std::string& heapProfilerOutputFilename) const
	{
		// Keep track of the time the performance test starts
		boost::timer timer;
		timer.restart();
		// Start CPU and heap profilers
		ProfilerStart(cpuProfilerOutputFilename.c_str());
		HeapProfilerStart(heapProfilerOutputFilename.c_str());

		// Iteration through all operations
		for (TestOperationList::const_iterator op = operations.begin();
			(op != operations.end()); op++)
		{
			switch (op->type)
			{
			case TestOperation::OPERATION_TYPE_INSERT:
				structure->insert(op->operand1);
				break;
			case TestOperation::OPERATION_TYPE_DELETE:
				structure->remove(op->operand1);
				break;
			case TestOperation::OPERATION_TYPE_UPDATE:
				structure->update(op->operand1, op->operand2);
				break;
			case TestOperation::OPERATION_TYPE_POINTQUERY:
				structure->pointExists(op->operand1);
				break;
			}
		}

		// Store time elapsed during execution of the operations
		Timing elapsed = timer.elapsed();
		// Stop profilers
		ProfilerStop();
		// NOTE: You have to call HeadProfilerDump() to produce the heap
		// profiling output. HeapProfilerStop() will not produce it for you
		// like ProfilerStop() does for PCU profiling.
		HeapProfilerDump(heapProfilerOutputFilename.c_str());
		HeapProfilerStop();

		return elapsed;
	}

	std::string Evaluator::generateCPUProfilerFilename(unsigned int testOpListIndex,
		unsigned int structureIndex) const
	{
		std::stringstream ss;
		ss << "CPU_PROFILE_" << testOpListIndex << "_" << structureIndex << ".prof";
		return ss.str();
	}
	
	std::string Evaluator::generateHeapProfilerFilename(unsigned int testOpListIndex,
		unsigned int structureIndex) const
	{
		std::stringstream ss;
		ss << "HEAP_PROFILE_" << testOpListIndex << "_" << structureIndex << ".heap";
		return ss.str();		
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