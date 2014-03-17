#include "CommandLineArguments.h"
#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

namespace mdsearch
{

	const unsigned int CommandLineArguments::DEFAULT_TEST_RUNS = 1;

	CommandLineArguments::CommandLineArguments(int argc, char* argv[]) :
		validArguments(true), verbose(false), profileHeapFlag(false),
		runs(DEFAULT_TEST_RUNS)
	{
		// Shortened namespace for more concise code
		namespace po = boost::program_options; 
		using boost::lexical_cast;
		using boost::bad_lexical_cast;

		try
		{
			// Define possible command line arguments
			po::options_description description("Options");
			description.add_options()
				("help,h", "print help message")
				("output,o", po::value<std::string>(), "filename to store generated results")
				("verbose,v", "verbose output produced by evaluator if this flag is set")
				("heap", "heap is profiled if this flag is set")
				("runs,r", po::value<unsigned int>(), "number of runs to perform of test operations to get AVERAGE time")
				("index_structures,s", po::value<std::vector<std::string> >(), "index structure to evaluate")
				("datasets,d", po::value<std::vector<std::string> >(), "dataset to use for evaluation")
				("test_operations,t", po::value<std::vector<std::string> >(), "list of test operations to perform");
			// Parse given command line arguments
			po::variables_map parsedArgs;
			po::store( po::parse_command_line(argc, argv, description), parsedArgs );
			// If help message was asked for, print it and DON'T DO ANYTHING ELSE
			if (parsedArgs.count("help"))
			{
				std::cout << "Multi-dimensional Search Structure Evaluator"
					<< std::endl << description << std::endl;
			}

			po::notify(parsedArgs); // throws exception if error in parse

			// Check output filename was given
			if (parsedArgs.count("output"))
			{
				outputFilename = parsedArgs["output"].as<std::string>();
			}
			// If verbose flag given, set verbose flag to true
			if (parsedArgs.count("verbose"))
			{
				verbose = true;
			}
			if (parsedArgs.count("heap"))
			{
				profileHeapFlag = true;
			}
			if (parsedArgs.count("runs"))
			{
				runs = parsedArgs["runs"].as<unsigned int>();
			}

			// Get all specified index structures, parsing strings to separate
			// structure type and the structure's additional arguments
			if (parsedArgs.count("index_structures"))
			{
				std::vector<std::string> structures = parsedArgs["index_structures"].as<std::vector<std::string> >();
				for (std::vector<std::string>::const_iterator str = structures.begin();
					(str != structures.end()); str++)
				{
					// Split the structure into a list of strings using ","
					std::vector<std::string> fields;
					boost::split(fields, (*str), boost::is_any_of(","));
					// If the type and dimensionality of the structure HAS NOT BEEN SPECIFIED,
					// then ignore the structure
					if (fields.size() < 2)
						continue;

					// Use first field as structure type and the rest as the structure's arguments
					IndexStructureSpecification spec;
					spec.type = fields[0];
					try
					{
						spec.numDimensions = lexical_cast<unsigned int>(fields[1]);
					}
					catch (const bad_lexical_cast &) // IF INVALID DIMENSIONS -- IGNORE STRUCTURE!
					{
						continue;
					}
					spec.arguments.reserve(fields.size() - 1);
					for (unsigned int i = 2; (i < fields.size()); i++)
						spec.arguments.push_back(fields[i]);
					specifiedIndexStructures.push_back(spec);
				}
			}
			// Get all specified test datasets and operations
			if (parsedArgs.count("datasets"))
				datasets = parsedArgs["datasets"].as<std::vector<std::string> >();
			if (parsedArgs.count("test_operations"))
				testOperations = parsedArgs["test_operations"].as<std::vector<std::string> >();
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

	bool CommandLineArguments::isVerbose() const
	{
		return verbose;
	}

	bool CommandLineArguments::profileHeap() const
	{
		return profileHeapFlag;
	}

	unsigned int CommandLineArguments::testRunsToPerform() const
	{
		return runs;
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