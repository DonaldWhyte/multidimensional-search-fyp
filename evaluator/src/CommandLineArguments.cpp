#include "CommandLineArguments.h"
#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>

namespace mdsearch
{

	/* Parse given list of strings based and retrieve boundaries
	 * for each dimension from it. */
	Region parseBoundary(const std::string& args)
	{
		// Split given argument string into individual fields
		std::vector<std::string> fields;
		boost::split(fields, args, boost::is_any_of(","));
		// Determine number of dimensions by field count
		unsigned int numDimensions = fields.size() / 2;

		Region boundary(numDimensions);
		for (unsigned int d = 0; (d < numDimensions); d++)
		{
			// Parse string arguments into real numbers
			Real min = boost::lexical_cast<Real>(fields[d * 2]);
			Real max = boost::lexical_cast<Real>(fields[d * 2 + 1]);
			boundary[d] = Interval(min, max);
		}
		return boundary;		
	}

	const unsigned int CommandLineArguments::DEFAULT_TEST_RUNS = 1;

	CommandLineArguments::CommandLineArguments(int argc, char* argv[]) :
		validArguments(true), verbose(false), profileCPUFlag(false),
		profileHeapFlag(false), runs(DEFAULT_TEST_RUNS),
		boundary(0)
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
				("cpu", "CPU is profiled if this flag is set")
				("heap", "heap is profiled if this flag is set")
				("boundary", po::value<std::string>(), "spatial boundary to use for each structure based on spatial decomposition (2d comma-separated numbers should be provided)")
				("runs,r", po::value<unsigned int>(), "if flag set,  boundaries of spatial decomposition structures will be computed automatically using the first given dataset")
				("index_structures,s", po::value<std::vector<std::string> >(), "index structure to evaluate")
				("preloaded_dataset,p", po::value<std::string>(), "dataset of points which are pre-loaded into structure before running timed tests")
				("test_operations,t", po::value<std::vector<std::string> >(), "list of test operations to perform")
				("individual_optest_datasets,i", po::value<std::vector<std::string> >(), "list of datasets to use for individual operation performance tests")
				("point_counts,c", po::value<std::vector<int> >(), "different structure sizes (in points stored) to sample individual operation times at");

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
			if (parsedArgs.count("cpu"))
			{
				profileCPUFlag = true;
			}
			if (parsedArgs.count("heap"))
			{
				profileHeapFlag = true;
			}
			if (parsedArgs.count("boundary"))
			{
				boundary = parseBoundary(parsedArgs["boundary"].as<std::string>());
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
					// If the type of the structure HAS NOT BEEN SPECIFIED, then ignore this spec
					if (fields.size() < 1)
						continue;

					// Use first field as structure type and the rest as the structure's arguments
					IndexStructureSpecification spec;
					spec.type = fields[0];
					spec.arguments.reserve(fields.size() - 1);
					for (unsigned int i = 1; (i < fields.size()); i++)
						spec.arguments.push_back(fields[i]);
					specifiedIndexStructures.push_back(spec);
				}
			}
			// Check if a dataset to preloaded has been specified
			if (parsedArgs.count("preloaded_dataset"))
				preloadedDatasetFilename = parsedArgs["preloaded_dataset"].as<std::string>();
			// Get all specified test operation lists
			if (parsedArgs.count("test_operations"))
				testOperations = parsedArgs["test_operations"].as<StringList>();
			// Get all specified individual operation test datasets
			if (parsedArgs.count("individual_optest_datasets"))
				individualOpDatasets = parsedArgs["individual_optest_datasets"].as<StringList>();
			// Get all point counts to time individual operations at
			if (parsedArgs.count("point_counts"))
				pCountsToSample = parsedArgs["point_counts"].as<std::vector<int> >();
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

	bool CommandLineArguments::profileCPU() const
	{
		return profileCPUFlag;
	}

	bool CommandLineArguments::profileHeap() const
	{
		return profileHeapFlag;
	}

	unsigned int CommandLineArguments::testRunsToPerform() const
	{
		return runs;
	}	

	const std::vector<IndexStructureSpecification>& CommandLineArguments::indexStructures() const
	{
		return specifiedIndexStructures;
	}

	const std::string& CommandLineArguments::datasetToPreloadFilename() const
	{
		return preloadedDatasetFilename;
	}

	const StringList& CommandLineArguments::testOperationFilenames() const
	{
		return testOperations;
	}

	const std::string& CommandLineArguments::resultFilename() const
	{
		return outputFilename;
	}

	const StringList& CommandLineArguments::CommandLineArguments::individualOpDatasetFilenames() const
	{
		return individualOpDatasets;
	}

	const std::vector<int>& CommandLineArguments::pointCountsToSample() const
	{
		return pCountsToSample;
	}

	bool CommandLineArguments::boundaryProvided() const
	{
		return (boundary.numDimensions() != 0);
	}

	const Region& CommandLineArguments::structureBoundary() const
	{
		return boundary;
	}

}
