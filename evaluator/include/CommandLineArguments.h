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
		struct IndexStructureSpecification
		{
			std::string type;
			StringList arguments;
		};

		CommandLineArguments(char* argv[]);

		const std::vector<IndexStructureSpecification>& indexStructures() const;
		const StringList& datasetFilenames() const;
		const StringList& testOperationFilenames() const;
		const std::string& resultFilename() const;


	private:
		std::vector<IndexStructureSpecification> specifiedIndexStructures;
		StringList datasets;
		StringList testOperations;
		std::string outputFilename;


	};
}

#endif