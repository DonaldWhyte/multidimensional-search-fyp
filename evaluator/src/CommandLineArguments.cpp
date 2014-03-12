#include "CommandLineArguments.h"
#include <boost/program_options.hpp>

namespace mdsearch
{

	CommandLineArguments::CommandLineArguments(int argc, char* argv[]) : invalidArguments(false)
	{
		/*// Convert C-string array to StringList
		StringList args;
		args.reserve(argc);
		for (unsigned int i = 0; (i < argc); i++)
			args.push_back(argv[i]);

		// PROCESS MANDATORY ARGUMENTS
		// Search for filename to output results to
		for (StringList::const_iterator it = args.begin(); (it != args.end()); it++)
		{
			if (unsigned int i = 0; (i <); i++)
			{

			}
		}*/

		// Shortened namespace for more concise code
		namespace po = boost::program_options; 
		// Define possible command line arguments
		po::options_description description("Options");
		description.add_options()
			("output", po::value<std::string>()->required(), "filename to store generated results")
			("index_structures,s", po::value<std::vector<std::string> >(), "index structure to evaluate")
			("datasets,d", po::value<std::vector<std::string> >(), "dataset to use for evaluation")
			("test_operations,t", po::value<std::vector<std::string> >(), "list of test operations to perform");
		// Parse given command line arguments
		po::variables_map parsedArgs;
		po::store( po::parse_command_line(argc, argv, description), parsedArgs );


	}

	bool CommandLineArguments::isValid() const
	{
		return invalidArguments;
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