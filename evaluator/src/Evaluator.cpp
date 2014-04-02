#include "Evaluator.h"
#include <sstream>
#include <algorithm>
#include <gperftools/profiler.h>
#include <gperftools/heap-profiler.h>

#if defined(_WIN32)
	#include <time.h>
#elif defined(unix)
	#include <sys/time.h>
#else
	#error No timing mechanism supported for this platform
#endif

#include "PyramidTree.h"

namespace mdsearch
{

	Timing getTime()
	{
	#if defined(_WIN32)
		return static_cast<Timing>(time(NULL)); // in seconds already
	#elif defined(unix)
		timespec ts;
		clock_gettime(CLOCK_REALTIME, &ts); 
		return static_cast<Timing>(ts.tv_sec) + (static_cast<Timing>(ts.tv_nsec) / 1000000000.0);
	#else
		return 0.0;
	#endif
	}

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
				<< " OPERATION SETS FOR" << structures.size() << " INDEX STRUCTURES" << std::endl;
		}

		OperationListTimings testOpTimings;
		testOpTimings.reserve(testOperationLists.size());

		for (unsigned int t = 0; (t < testOperationLists.size()); t++)
		{
			if (verbose)
				std::cout << "Test Operations (" << t << "):" << std::endl;

			// Cycle through structures until all test runs are complete
			std::vector<std::vector<Timing> > structureTimingsAll;
			structureTimingsAll.resize(structures.size());
			for (unsigned int s = 0; (s < structures.size()); s++)
				structureTimingsAll[s].resize(numTestRuns);

			for (unsigned r = 0; (r < numTestRuns); r++)
			{
				if (verbose)
					std::cout << "\t\tExecuting test run" << (r + 1) << "..." << std::endl;
				for (unsigned int s = 0; (s < structures.size()); s++)
				{
					if (verbose)
						std::cout << "\tRunning operations on structure (" << s << ")..." << std::endl;
					// Ensure structure is completely empty
					structures[s]->clear();
					// Pre-load specified points
					if (dataToPreload.size() > 0)
					{
						if (verbose)
							std::cout << "\t\tPre-loading " << dataToPreload.size() << " points into structure for run " << (r + 1) << std::endl;
						structures[s]->loadPoints(dataToPreload);
					}
					// Run test operations on structure and store to compute average later
					structureTimingsAll[s][r] = runOperations(structures[s],
						testOperationLists[t],
						generateCPUProfilerFilename(t, s),
						generateHeapProfilerFilename(t, s)
					);

					PyramidTree* pt = dynamic_cast<PyramidTree*>(structures[s]);
					if (pt)
					{
						std::cout << "Bucket utilisation: " << pt->averagePointsPerBucket() << std::endl;
						std::cout << "\tLoad factor: " << (pt->averagePointsPerBucket() / testOperationLists[t].size()) << std::endl;
					}	

				}
			}
			// Now compute average times for each structure 
			StructureTimings averageStructureTimings;
			averageStructureTimings.reserve(structures.size());
			for (unsigned int s = 0; (s < structures.size()); s++)
			{
				Timing sum = 0;
				for (unsigned r = 0; (r < numTestRuns); r++)
					sum += structureTimingsAll[s][r];
				averageStructureTimings.push_back( sum / numTestRuns );
			}

			testOpTimings.push_back(averageStructureTimings);
		}

		if (verbose)
			std::cout << "PERFORMANCE TESTS FINISHED" << std::endl;

		return testOpTimings;
	}

	std::vector<StructureOperationTimings> Evaluator::timeIndividualOperations(
		const PointList& dataset,
		const std::vector<int>& pointCountsToTime) const
	{
		if (verbose)
		{
			std::cout << "STARTING INDIVIDUAL OPERATION PERFORMANCE TESTS USING " << dataset.size()
				<< " POINTS, SAMPLING AT " << pointCountsToTime.size() << " STRUCTURE SIZES, FOR "
				<< structures.size()  << " INDEX STRUCTUES" << std::endl;
		}

		std::vector<StructureOperationTimings> individualOpTimings;
		for (unsigned int s = 0; (s < structures.size()); s++)
		{
			if (verbose)
				std::cout << "\tTiming individual operations of structure (" << s << ")..." << std::endl;			
			structures[s]->clear();

			StructureOperationTimings structOpTimings;
			for (unsigned int p = 0; (p < dataset.size()); p++)	
			{
				// Insert next point in dataset
				structures[s]->insert(dataset[p]);
				// If operations with this structure size should be timed!
				if (std::find(pointCountsToTime.begin(), pointCountsToTime.end(), p + 1) != pointCountsToTime.end())
				{
					OperationTimings opTimings;
					opTimings.n = p + 1;
					// Remove point and time
					Timing startTime = getTime();
					structures[s]->remove(dataset[p]);
					opTimings.remove = getTime() - startTime;
					// Insert same point, then time point query
					structures[s]->insert(dataset[p]);
					startTime = getTime();
					structures[s]->pointExists(dataset[p]);
					opTimings.pointQuery = getTime() - startTime;
					// Insert ANOTHER point and time
					startTime = getTime();
					structures[s]->insert(dataset[p]);
					opTimings.insert = getTime() - startTime;
					// Remove that point to prepare for next iteration of loop
					structures[s]->remove(dataset[p]);

					// Add recorded timings to structure
					structOpTimings.push_back(opTimings);
				}
			}

			individualOpTimings.push_back(structOpTimings);
		}

		if (verbose)
			std::cout << "INDIVIDUAL OPERATION PERFORMANCE TESTS FINISHED" << std::endl;

		return individualOpTimings;
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
		Timing startTime = getTime();

		// Iteration through all operations
		for (TestOperationList::const_iterator op = operations.begin();
			(op != operations.end()); op++)
		{
			switch (op->type)
			{
			case TestOperation::OPERATION_TYPE_INSERT:
				structure->insert(op->operand1);
				break;
			/*case TestOperation::OPERATION_TYPE_DELETE:
				structure->remove(op->operand1);
				break;*/
			case TestOperation::OPERATION_TYPE_UPDATE:
				structure->update(op->operand1, op->operand2);
				break;
			case TestOperation::OPERATION_TYPE_POINTQUERY:
				structure->pointExists(op->operand1);
				break;
			}
		}

		// Store time elapsed during execution of the operations
		Timing elapsed = getTime() - startTime;
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

	SizeTimingTable getTimingTableForOperation(
		const StructureOperationTimings& structureTimings,
		TestOperation::Type opType)
	{
		SizeTimingTable table;

		for (StructureOperationTimings::const_iterator timings = structureTimings.begin();
			(timings != structureTimings.end()); timings++)
		{
			switch (opType)
			{
			case TestOperation::OPERATION_TYPE_INSERT:
				table[timings->n] = timings->insert;
				break;
			case TestOperation::OPERATION_TYPE_DELETE:
				table[timings->n] = timings->remove;
				break;
			case TestOperation::OPERATION_TYPE_POINTQUERY:
				table[timings->n] = timings->pointQuery;
				break;
			}
		}

		return table;
	}

}