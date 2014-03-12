#include "CommandLineArguments.h"

namespace mdsearch
{

	CommandLineArguments::CommandLineArguments(char* argv[])
	{
		// TODO
	}

	const std::vector<CommandLineArguments::IndexStructureSpecification>& CommandLineArguments::indexStructures() const
	{
		return specifiedIndexStructures;
	}

	const StringList& CommandLineArguments::datasetFilenames() const
	{
		return datasets;
	}

	const StringList& CommandLineArguments::testOperationFilenames() const
	{
		return testOperations;
	}

	const std::string& CommandLineArguments::resultFilename() const
	{
		return outputFilename;
	}

}