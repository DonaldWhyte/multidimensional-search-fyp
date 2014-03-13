#ifndef MDSEARCH_EVALUATOR_H
#define MDSEARCH_EVALUATOR_H

#include <vector>
#include "IndexStructure.h"
#include "TestOperation.h"

namespace mdsearch
{

	// Types used for timinbg information
	typedef double Timing;
	typedef std::vector<Timing> StructureTimings;
	typedef std::vector<StructureTimings> OperationListTimings;

	/* Generate string that contains human-readable report of
	 * recorded dataset timings. */
	std::string generateTimingReport(const OperationListTimings& timings);

	/* Used to evaluate multiple index structures' performance using a collection
	 * of datasets and operation lists. */
	class Evaluator
	{

	public:
		/* Construct an evaluator specifically for the given structures. */
		Evaluator(const std::vector<IndexStructure*>& structures);

		/* Given a list of datasets and test operations, run each index
		 * structure on EACH OPERATION LIST!
		 *
		 * Let t be the number of operation lists and s be the number of 
		 * structures returned. A vector of size d is return, which
		 * contains t lists that contain s timing values (milliseconds)
		 * a structure took to perform the respective operations on a dataset.
		 */
		 OperationListTimings timePerformance(
			const std::vector<TestOperationList>& testOperationLists) const;

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
		bool verbose; // if true, progress log is outputted as test operations are being executed

	};

}

#endif