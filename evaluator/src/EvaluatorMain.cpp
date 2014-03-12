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
	std::vector<PointList> datasets;

	DatasetFileLoader datasetLoader;
	const StringList& datasetFilenames = args.datasetFilenames();
	for (unsigned int i = 0; (i < datasetFilenames.size()); i++)
	{
		datasets.push_back( datasetLoader.load(datasetFilenames[i]) );
	}

	// Load specified test operations
	std::vector<TestOperationList> testOperationLists;

	TestOperationLoader testOpLoader;
	const StringList& testOperationFilenames = args.testOperationFilenames();
	for (unsigned int i = 0; (i < testOperationFilenames.size()); i++)
	{
		testOperationLists.push_back(testOpLoader.loadFromFile(
			testOperationFilenames[i]) );
	}

	// Create evaluator object and fill it with loaded structures
	Evaluator evaluator(structures);
	// Run evaluation on loaded datasets and operation lists
	std::vector<std::vector<TimingList> > timings = evaluator.timePerformance(datasets, testOperationLists);

	// Display timing results
	for (unsigned int datasetIndex = 0; (datasetIndex < timings.size()); datasetIndex++)
	{
		std::cout << "Dataset " << datasetIndex << ":" << std::endl;

		const std::vector<TimingList>& operationListTimings = timings[datasetIndex];
		for (unsigned int operationListIndex = 0; (operationListIndex < operationListTimings.size()); operationListIndex++)
		{
			std::cout << "\tOperation List " << operationListIndex << ": " << std::endl;

			const TimingList& structureTimings = operationListTimings[operationListIndex];
			for (unsigned int structureIndex = 0; (structureIndex < structureTimings.size()); structureIndex++)
			{
				long elapsedTime = structureTimings[structureIndex];
				std::cout << "\t\tStructure " << structureIndex << ": " << elapsedTime << std::endl;
			}
		}
	}

	return 0;
}