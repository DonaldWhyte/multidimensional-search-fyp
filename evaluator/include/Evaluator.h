#ifndef MDSEARCH_EVALUATOR_H
#define MDSEARCH_EVALUATOR_H

#include <vector>
#include <map>
#include "IndexStructure.h"
#include "TestOperation.h"

namespace mdsearch
{

	// Types used for timing information
	typedef double Timing;
	typedef std::vector<Timing> StructureTimings;
	typedef std::vector<StructureTimings> OperationListTimings;

	struct OperationTimings
	{
		unsigned int n; // current size of structure when timings were made
		Timing insert;
		Timing remove;
		Timing pointQuery;
	};
	typedef std::vector<OperationTimings> StructureOperationTimings; // for a single structure

	typedef std::map<int, Timing> SizeTimingTable;

	/* Return current time. */
	Timing getTime();

	/* Generate string that contains human-readable report of
	 * recorded dataset timings. */
	std::string generateTimingReport(const OperationListTimings& timings);
	/* Create map of dataset size to operation runtime for a specified
	 * operation, using all the timings from a structure. */
	SizeTimingTable getTimingTableForOperation(
		const StructureOperationTimings& structureTimings,
		TestOperation::Type opType);

	/* Used to evaluate multiple index structures' performance using a collection
	 * of datasets and operation lists. */
	class Evaluator
	{

	public:
		/* Construct an evaluator specifically for the given structures. */
		Evaluator(const std::vector<IndexStructure*>& structures,
			unsigned int numTestRuns, bool profileCPU = false,
			bool profileHeap = false, bool verbose = false);

		/* Given a list of datasets and test operations, run each index
		 * structure on EACH OPERATION LIST!
		 *
		 * Let t be the number of operation lists and s be the number of 
		 * structures returned. A vector of size d is return, which
		 * contains t lists that contain s timing values (milliseconds)
		 * a structure took to perform the respective operations on a dataset.
		 *
		 * If there are points that should be pre-loaded into each structure,
		 * BEFORE timing the operations on it, then this can be passed in
		 * as the 'dataToPreload' argument.
		 */
		OperationListTimings timePerformance(
			const std::vector<TestOperationList>& testOperationLists,
			const PointList& dataToPreload = PointList()) const;
		/* Given a dataset, time how long each individual insert(),
		 * delete() and point query takes at the different structure
		 * sizes specified in 'pointCountsToTime'.
		 *
		 * A vector of vectors is returned, where each vector contains
		 * the individual operation timings at different structure
		 * sizes for a SINGLE STRUCTURE TYPE. */
		std::vector<StructureOperationTimings> timeIndividualOperations(
			const PointList& dataset,
			const std::vector<int>& pointCountsToTime) const;

		/* Accessors */
		bool isVerbose() const;
		void setVerbose(bool verbose);

	private:
		Timing runOperations(IndexStructure* structure,
			const TestOperationList& operations,
			const std::string& cpuProfilerOutputFilename,
			const std::string& heapProfilerOutputFilename) const;

		/* Return filenames for output of CPU and heap profilers,
		 * which are generated based on current operation list
		 * and index structure being tested. */
		std::string generateCPUProfilerFilename(unsigned int testOpListIndex, unsigned int structureIndex) const;
		std::string generateHeapProfilerFilename(unsigned int testOpListIndex, unsigned int structureIndex) const;

		std::vector<IndexStructure*> structures;
		bool profileCPU; // if true, the CPU is profiled for each test operation ruin
		bool profileHeap; // if true, the heap is profiled for each test operation run
		bool verbose; // if true, progress log is outputted as test operations are being executed
		// Number of test runs to perform for each operation list 
		// Average time of all runs is used for the outputted time
		unsigned int numTestRuns;

	};

}

#endif