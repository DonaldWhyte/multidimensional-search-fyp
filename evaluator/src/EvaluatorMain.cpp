#include <iostream>
#include <fstream>
#include <sstream>
#include "CommandLineArguments.h"
#include "IndexStructureFactory.h"
#include "DatasetFileLoader.h"
#include "TestOperationLoader.h"
#include "Evaluator.h"

using namespace mdsearch;

Region computeOpListBoundary(const TestOperationList& opList)
{
	if (!opList.empty())
	{
		unsigned int numDimensions = opList[0].operand1.numDimensions();
		Region boundary(numDimensions);
		// Now search through all points in this operation list to find
		// minimum and maximum values for each dimension
		for (unsigned int i = 0; (i < opList.size()); i++)
		{
			const TestOperation& op = opList[i];
			for (unsigned int d = 0; (d < numDimensions); d++)
			{
				if (op.operand1[d] < boundary[d].min)
					boundary[d].min = op.operand1[d];
				else if (op.operand1[d] > boundary[d].max)
					boundary[d].max = op.operand1[d];
				if (op.operand2.numDimensions() > 0) // if there is a second operand
				{
					if (op.operand2[d] < boundary[d].min)
						boundary[d].min = op.operand2[d];
					else if (op.operand2[d] > boundary[d].max)
						boundary[d].max = op.operand2[d];
				}
			}
		}
		return boundary;
	}
	else
	{
		return Region(0);
	}
}

void writeResultsToFile(const std::string& filename,
	const OperationListTimings& timings,
	const CommandLineArguments& args,
	const std::vector<TestOperationList>& testOperationLists,
	const PointList& datasetToPreload,
	unsigned int globalNumDimensions)
{
	std::ofstream file(filename.c_str());
	// Write structures used
	file << "===STRUCTURES===\n";
	const std::vector<CommandLineArguments::IndexStructureSpecification> structureSpecs = args.indexStructures();
	for (unsigned int i = 0; (i < structureSpecs.size()); i++)
	{
		const CommandLineArguments::IndexStructureSpecification& spec = structureSpecs[i];
		file << "Structure (" << i << "):\n\tType: " << spec.type << "\n\tDimensionality: " << globalNumDimensions << "\n\t";
		file << "Arguments: ";
		if (spec.arguments.size() > 0)
		{
			for (unsigned int j = 0; (j < spec.arguments.size() -1); j++)
				file << spec.arguments[j] << " ";
			file << spec.arguments[spec.arguments.size() -1];
		}
		file << "\n";
	}
	file << "\n";
	// Information about pre-loaded data
	file << "===PRE-LOADED DATASET===\n";
	file << "Points Pre-Loaded for Each Test: " << datasetToPreload.size() << "\n";
	file << "\n";
	// Write test operation list information
	file << "===OPERATION LISTS===\n";
	for (unsigned int i = 0; (i < testOperationLists.size()); i++)
	{
		file << "Test Operation List (" << i << "):\n";
		file << "\tFilename: " << args.testOperationFilenames()[i] << "\n";
		file << "\tNumber of Operations: " << testOperationLists[i].size() << "\n";
	}
	file << "\n";
	// Write timing report
	file << "===TIMINGS===\n";
	file << generateTimingReport(timings);
	file.close();
}

std::string generateIndividualOpTimingFilename(unsigned int structureIndex,
	unsigned int datasetIndex, const std::string& operation)
{
	std::stringstream ss;
	ss << "individual_op_timing_" << structureIndex << "_"
		<< datasetIndex << "_" << operation << ".times";
	return ss.str();
}

void writeSizeTimingTable(const std::string& filename, const SizeTimingTable& table)
{
	std::ofstream file(filename.c_str());

	// NOTE: Guaranteed to iterate through keys in ascending order
	for (SizeTimingTable::const_iterator it = table.begin(); it != table.end(); it++)
	{
		file << it->first << " " << it->second << "\n";
	}

	file.close();
}

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

	// Load dataset that will be pre-loaded into structure for each test
	if (args.isVerbose())
		std::cout << "Loading dataset to pre-load into structures" << std::endl;
	DatasetFileLoader datasetLoader;
	PointList datasetToPreload = datasetLoader.load(
		args.datasetToPreloadFilename()
	);
	// Load datasets for individual operation counts
	if (args.isVerbose())
		std::cout << "Loading datasets to use for individual operation timings" << std::endl;	
	std::vector<PointList> datasetsForIndividualOpTests;
	const StringList& individualOpDatasetFilenames = args.individualOpDatasetFilenames();
	for (unsigned int i = 0; (i < individualOpDatasetFilenames.size()); i++)
	{
		datasetsForIndividualOpTests.push_back(
			datasetLoader.load(individualOpDatasetFilenames[i])
		);
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
	// Determine point dimensionality and boundaries to use for index structures
	// from first operation list if specified
	if (args.isVerbose())
		std::cout << "Finding dimensionality and minimum /maximum boundary of dataset used for first operation list" << std::endl;

	unsigned int globalNumDimensions = 0;
	Region globalBoundary(0);
	// If global boundary was provided in the command line arguments, use
	// that and DON'T automatically compute boundary
	if (args.boundaryProvided())
	{
		globalBoundary = args.structureBoundary();
		globalNumDimensions = globalBoundary.numDimensions();
	}
	else if (!testOperationLists.empty())
	{
		const TestOperationList& firstList = testOperationLists[0];
		globalBoundary = computeOpListBoundary(firstList);
		globalNumDimensions = globalBoundary.numDimensions();
	}
	else
	{
		return 0;
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
			spec.type, globalNumDimensions, globalBoundary, spec.arguments);
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



	// Create evaluator object and fill it with loaded structures
	Evaluator evaluator(structures, args.testRunsToPerform(),
		args.profileCPU(), args.profileHeap(), args.isVerbose());

	// If there are any test operation lists
	if (testOperationLists.size() > 0)
	{
		// Run evaluation on loaded datasets and operation lists
		OperationListTimings timings = evaluator.timePerformance(
			testOperationLists, datasetToPreload);
		std::cout << generateTimingReport(timings) << std::endl;
		// Write timing results to file if an output filename was given
		if (!args.resultFilename().empty())
		{
			writeResultsToFile(args.resultFilename(), timings,
				args, testOperationLists, datasetToPreload, globalNumDimensions);
		}
	}	
	// If there are any datasets to test indiviudal point counts,
	// run the tests and store the results
	for (unsigned int i = 0; (i < datasetsForIndividualOpTests.size()); i++)
	{
		std::vector<StructureOperationTimings> individualOpTimings =
			evaluator.timeIndividualOperations(
				datasetsForIndividualOpTests[i],
				args.pointCountsToSample());
		if (args.isVerbose())
			std::cout << "WRTITING TIMINGS TO FILES" << std::endl;

		for (unsigned int s = 0; (s < individualOpTimings.size()); s++)
		{
			const StructureOperationTimings& structureOpTimings = individualOpTimings[s];
			writeSizeTimingTable(
				 generateIndividualOpTimingFilename(s, i, "insert"),
				 getTimingTableForOperation(structureOpTimings, TestOperation::OPERATION_TYPE_INSERT));
			writeSizeTimingTable(
				 generateIndividualOpTimingFilename(s, i, "delete"),
				 getTimingTableForOperation(structureOpTimings, TestOperation::OPERATION_TYPE_DELETE));
			writeSizeTimingTable(
				 generateIndividualOpTimingFilename(s, i, "pquery"),
				 getTimingTableForOperation(structureOpTimings, TestOperation::OPERATION_TYPE_POINTQUERY));
		}
	}

	return 0;
}