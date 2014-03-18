#ifndef MDSEARCH_COMMANDLINEARGUMENTS_H
#define MDSEARCH_COMMANDLINEARGUMENTS_H

#include <vector>
#include <string>

namespace mdsearch
{

	typedef std::vector<std::string> StringList;

	class CommandLineArguments
	{

	public:
		/* Default number of test runs to perform. */
		static const unsigned int DEFAULT_TEST_RUNS;

		struct IndexStructureSpecification
		{
			std::string type;
			unsigned int numDimensions;
			StringList arguments;
		};

		CommandLineArguments(int argc, char* argv[]);

		/* Return true if parsed command line arguments are valid. */
		bool isValid() const;
		/* If true is returned, verbose output is expected when evaluator is running. */
		bool isVerbose() const;
		/* If true, then the heap will be profiled for heap memory allocation. */
		bool profileHeap() const;
		/* Number of runs to perform of each test operation set (to compute average time). */
		unsigned int testRunsToPerform() const;
		/* Accessors for parsed command line data. */
		const std::vector<IndexStructureSpecification>& indexStructures() const;
		const StringList& datasetFilenames() const;
		const StringList& testOperationFilenames() const;
		const std::string& resultFilename() const;


	private:
		// If this flag is set to true, then some of the required arguments
		// are either missing or invalid
		bool validArguments;
		bool verbose;
		bool profileHeapFlag;
		unsigned int runs;

		std::vector<IndexStructureSpecification> specifiedIndexStructures;
		StringList datasets;
		StringList testOperations;
		std::string outputFilename;


	};
}

#endif