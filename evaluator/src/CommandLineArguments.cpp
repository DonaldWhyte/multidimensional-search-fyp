#include "CommandLineArguments.h"
#include <boost/program_options.hpp>

namespace mdsearch
{

	CommandLineArguments::CommandLineArguments(int argc, char* argv[]) : validArguments(false)
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
		try
		{
			// Define possible command line arguments
			po::options_description description("Options");
			description.add_options()
				("help,h", "print help message")
				("output,o", po::value<std::string>()->required(), "filename to store generated results")
				("index_structures,s", po::value<std::vector<std::string> >(), "index structure to evaluate")
				("datasets,d", po::value<std::vector<std::string> >(), "dataset to use for evaluation")
				("test_operations,t", po::value<std::vector<std::string> >(), "list of test operations to perform");
			// Parse given command line arguments
			po::variables_map parsedArgs;
			po::store( po::parse_command_line(argc, argv, description), parsedArgs );
			// If help message was asked for, print it and DON'T DO ANYTHING ELSE
			if (parsedArgs.count("help"))
			{
				std::cout << "Multi-dimensional Search Structure Evaluator" << std::endl << description << std::endl;
			}

			// Check output filename was given
			if (parsedArgs.count("output"))
			{
				outputFilename = parsedArgs["output"].as<std::string>();
				validArguments = true; // all required args given!
			}
		}
		catch(po::error& error)
		{
			validArguments = false;
		}
	}

	bool CommandLineArguments::isValid() const
	{
		return validArguments;
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