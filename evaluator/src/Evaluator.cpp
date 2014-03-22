#include "Evaluator.h"
#include <ctime>
#include <sstream>
#include <boost/timer.hpp>
#include <gperftools/profiler.h>
#include <gperftools/heap-profiler.h>

namespace mdsearch
{

	Evaluator::Evaluator(const std::vector<IndexStructure*>& structures,
		unsigned int numTestRuns, bool profileCPU, bool profileHeap, bool verbose)
		: structures(structures), numTestRuns(numTestRuns),
		profileCPU(profileCPU), profileHeap(profileHeap), verbose(verbose)
	{
	}

	OperationListTimings Evaluator::timePerformance(
		const std::vector<TestOperationList>& testOperationLists,
		const PointList& dataToPreload) const
	{
		// NOTE: reserve() calls are done to minimise the amount of dynamic allocations as possible

		if (verbose)
		{
			std::cout << "STARTING PERFORMANCE TESTS USING " << testOperationLists.size()
				<< " OPERATION SETS FOR" << structures.size() << " INDEX STRUCTUES" << std::endl;
		}

		OperationListTimings testOpTimings;
		testOpTimings.reserve(testOperationLists.size());

		for (unsigned int t = 0; (t < testOperationLists.size()); t++)
		{
			StructureTimings structureTimings;
			structureTimings.reserve(structures.size());

			if (verbose)
				std::cout << "Test Operations (" << t << "):" << std::endl;

			for (unsigned int s = 0; (s < structures.size()); s++)
			{
				if (verbose)
					std::cout << "\tRunning operations on structure (" << s << ")..." << std::endl;

				Timing sumOfTimes = 0;
				for (unsigned int i = 0; (i < numTestRuns); i++) // multiple runs of operations
				{
					// Ensure structure is completely empty
					structures[s]->clear();
					// Pre-load specified points
					if (dataToPreload.size() > 0)
					{
						if (verbose)
							std::cout << "\t\tPre-loading " << dataToPreload.size() << " points into structure for run " << (i + 1) << std::endl;
						structures[s]->loadPoints(dataToPreload);
					}
					// Run test operations on structure and add time elapsed
					// to the total sum
					if (verbose)
						std::cout << "\t\tExecuting test run..." << (i + 1) << std::endl;
					sumOfTimes += runOperations(structures[s],
						testOperationLists[t],
						generateCPUProfilerFilename(t, s),
						generateHeapProfilerFilename(t, s)
					);
				}
				
				Timing averageTime = sumOfTimes / numTestRuns;
				structureTimings.push_back(averageTime);
			}

			testOpTimings.push_back(structureTimings);
		}

		if (verbose)
			std::cout << "PERFORMANCE TESTS FINISHED" << std::endl;

		return testOpTimings;
	}

	bool Evaluator::isVerbose() const
	{
		return verbose;
	}

	void Evaluator::setVerbose(bool verbose)
	{
		this->verbose = verbose;
	}

	Timing Evaluator::runOperations(IndexStructure* structure,
		const TestOperationList& operations,
		const std::string& cpuProfilerOutputFilename,
		const std::string& heapProfilerOutputFilename) const
	{
		// Start CPU and heap profilers (heap first so it doesn't affect timings)
		if (profileHeap)
		{
			HeapProfilerStart(heapProfilerOutputFilename.c_str());
		}
		if (profileCPU)
		{
			ProfilerStart(cpuProfilerOutputFilename.c_str());
		}
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
		if (profileCPU)
		{
			ProfilerStop();
		}
		// NOTE: You have to call HeadProfilerDump() to produce the heap
		// profiling output. HeapProfilerStop() will not produce it for you
		// like ProfilerStop() does for PCU profiling.
		if (profileHeap)
		{
			HeapProfilerDump(heapProfilerOutputFilename.c_str());
			HeapProfilerStop();
		}

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
			ss << "Test Operations (" << operationListIndex << "):" << "\n";

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