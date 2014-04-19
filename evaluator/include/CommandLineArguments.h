#ifndef MDSEARCH_COMMANDLINEARGUMENTS_H
#define MDSEARCH_COMMANDLINEARGUMENTS_H

#include "Specifications.h"
#include "Region.h"

namespace mdsearch
{

	class CommandLineArguments
	{

	public:
		/* Default number of test runs to perform. */
		static const unsigned int DEFAULT_TEST_RUNS;

		CommandLineArguments(int argc, char* argv[]);

		/* Return true if parsed command line arguments are valid. */
		bool isValid() const;
		/* If true is returned, verbose output is expected when evaluator is running. */
		bool isVerbose() const;
		/* If true, then the CPU will be profiled for per-function timings and call graph generation. */
		bool profileCPU() const;
		/* If true, then the heap will be profiled for heap memory allocation. */
		bool profileHeap() const;
		/* Number of runs to perform of each test operation set (to compute average time). */
		unsigned int testRunsToPerform() const;
		/* Accessors for parsed command line data. */
		const std::vector<IndexStructureSpecification>& indexStructures() const;
		const std::string& datasetToPreloadFilename() const;
		const StringList& testOperationFilenames() const;
		const std::string& resultFilename() const;

		const StringList& individualOpDatasetFilenames() const;
		const std::vector<int>& pointCountsToSample() const;

		bool boundaryProvided() const;
		const Region& structureBoundary() const;

	private:
		// If this flag is set to true, then some of the required arguments
		// are either missing or invalid
		bool validArguments;
		bool verbose;
		bool profileCPUFlag;
		bool profileHeapFlag;
		unsigned int runs;

		std::vector<IndexStructureSpecification> specifiedIndexStructures;
		std::string preloadedDatasetFilename;
		StringList testOperations;
		std::string outputFilename;

		StringList individualOpDatasets;
		std::vector<int> pCountsToSample;

		Region boundary;

	};
}

#endif