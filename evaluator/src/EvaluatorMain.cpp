#include <iostream>
#include "SpecEvaluator.h"

using namespace mdsearch;

std::string timingFilename(const TestSuite& suite, const std::string& datasetName)
{
	std::string filename = suite.name() + "_" + datasetName + ".times";
	return joinPath(suite.outputDirectory(), filename);
}

int main(int argc, char* argv[])
{
	// Parse command line arguments
	if (argc < 2)
	{
		std::cout << "Usage: " << argv[0] << " <specificationFilename> {--stdout}" << std::endl;
		return 1;
	}
	bool fileOutput = true;
	// If --stdout flag is present in arguments,
	for (unsigned int i = 0; (i < argc); i++)
		if (std::string(argv[i]) == "--stdout")
			fileOutput = false;

	// Use specification file to construct test suite
	TestSuite suite = TestSuite::fromFile(argv[1]);
	// Evaluator required structures using required data
	SpecEvaluator evaluator;
	DatasetTimings timings = evaluator.evaluatePerformance(suite);
	// Output results
	TimingWriter writer;
	if (fileOutput)
	{
		const std::vector<DatasetSpecification>& datasetSpecs = suite.datasetSpecs();
		for (DatasetTimings::const_iterator ds = timings.begin(); (ds != timings.end()); ds++)
		{
			std::string filename = timingFilename(suite, ds->first);
			std::string title = ds->first + " Timings";
			// Find varying parameter of dataset and use as X-axis label
			std::string xAxis;
			for (std::vector<DatasetSpecification>::const_iterator it = datasetSpecs.begin();
				(it != datasetSpecs.end()); it++)
			{
				if (it->name == ds->first)
				{
					xAxis = it->varyingParameter;
					break;
				}
			}
			writer.timesToFile(filename, title, xAxis, "Execution Time (in seconds)", ds->second);
		}
	}
	else
	{
		std::cout << "TEST SUITE: " << suite.name() << std::endl << std::endl;
		for (DatasetTimings::const_iterator ds = timings.begin(); (ds != timings.end()); ds++)
		{	
			std::cout << "DATASET: " << ds->first << std::endl;
			std::cout << "-----------------------------------------" << std::endl;
			std::cout << writer.timesToString(ds->second);
		}
	}

	return 0;
}