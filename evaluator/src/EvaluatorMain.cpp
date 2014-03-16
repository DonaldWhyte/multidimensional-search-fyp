#include <iostream>
#include "CommandLineArguments.h"
#include "IndexStructureFactory.h"
#include "DatasetFileLoader.h"
#include "TestOperationLoader.h"
#include "Evaluator.h"

using namespace mdsearch;

int main(int argc, char* argv[])
{
	// Parse command line arguments
	CommandLineArguments args(argc, argv);
	// If the arguments are not valid, exit program with failure code 
	if (!args.isValid())
	{
		std::cout << "Invalid command line arguments (use -h or --help for accepted arguments)." << std::endl;
		return 1;
	}


	// Construct specified index structures
	if (args.isVerbose())
		std::cout << "Loading index structures" << std::endl;	
	std::vector<IndexStructure*> structures;

	IndexStructureFactory structureFactory;
	const std::vector<CommandLineArguments::IndexStructureSpecification> structureSpecs = args.indexStructures();
	for (unsigned int i = 0; (i < structureSpecs.size()); i++)
	{
		// Use specification to load index structure
		const CommandLineArguments::IndexStructureSpecification& spec = structureSpecs[i];
		IndexStructure* structure = structureFactory.constructIndexStructure(
			spec.type, spec.numDimensions, spec.arguments);
		// If structure could not be constructed, display error message and exist program
		if (!structure)
		{
			std::cout << "Could not load structure \"" << spec.type << "\" with arguments: [ ";
			for (unsigned int j = 0; (j < spec.arguments.size()); j++)
			{
				std::cout << "\"" << spec.arguments[j] << "\" ";
			}
			std::cout << "]" << std::endl;
			return 1;
		}
		structures.push_back(structure);
	}

	// Load specified datasets
	if (args.isVerbose())
		std::cout << "Loading datasets" << std::endl;	
	std::vector<PointList> datasets;

	DatasetFileLoader datasetLoader;
	const StringList& datasetFilenames = args.datasetFilenames();
	for (unsigned int i = 0; (i < datasetFilenames.size()); i++)
	{
		datasets.push_back( datasetLoader.load(datasetFilenames[i]) );
	}

	// Load specified test operations
	std::vector<TestOperationList> testOperationLists;
	if (args.isVerbose())
		std::cout << "Loading test operations" << std::endl;

	TestOperationLoader testOpLoader;
	const StringList& testOperationFilenames = args.testOperationFilenames();
	for (unsigned int i = 0; (i < testOperationFilenames.size()); i++)
	{
		testOperationLists.push_back(testOpLoader.loadFromFile(
			testOperationFilenames[i]) );
	}

	// Create evaluator object and fill it with loaded structures
	Evaluator evaluator(structures, args.isVerbose());
	// Run evaluation on loaded datasets and operation lists
	OperationListTimings timings = evaluator.timePerformance(testOperationLists);
	std::cout << generateTimingReport(timings) << std::endl;

	return 0;
}