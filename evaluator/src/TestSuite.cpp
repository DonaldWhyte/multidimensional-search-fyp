#include "TestSuite.h"
#include <fstream>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

namespace mdsearch
{

	TestSuite::TestSuite(const std::string& suiteName, const std::string& suiteInputDir,
		const std::string& suiteOutputDir) :
		suiteName(suiteName), suiteInputDir(suiteInputDir), suiteOutputDir(suiteOutputDir)
	{
	}

	TestSuite TestSuite::fromFile(const std::string& filename)
	{
		std::ifstream file(filename.c_str());

		// Use first and second line as name and output direc tory for the suite
		std::string name, inputDir, outputDir;
		std::getline(file, name);
		std::getline(file, inputDir);
		std::getline(file, outputDir);
		TestSuite suite(name, inputDir, outputDir);

		// Now parse structures, treating each line as a structure
		// until a blank line is reached
		std::string line;
		std::getline(file, line); // skip first line
		std::getline(file, line);
		while (!line.empty())
		{
			// Split line unto fields using ' ' as delimiter
			std::vector<std::string> fields;
			boost::split(fields, line, boost::is_any_of(" "));
			// Use first field as structure type and the rest as the structure's arguments
			IndexStructureSpecification spec;
			spec.type = fields[0];
			spec.arguments.reserve(fields.size() - 1);
			for (unsigned int i = 1; (i < fields.size()); i++)
				spec.arguments.push_back(fields[i]);
			suite.addStructure(spec);

			std::getline(file, line);
		}

		// Parse dataset specifications until end of file
		std::getline(file, line);
		while (!file.eof())
		{
			// Parse line, treating first field as dataset name and 
			// second field as varying parameter (X axis label)
			std::vector<std::string> fields;
			boost::split(fields, line, boost::is_any_of(" "));
			DatasetSpecification dsSpec;
			dsSpec.name = fields[0];
			if (fields.size() >= 2)
				dsSpec.varyingParameter = fields[1];
			// Parse remaining lines as sub DBs until line with tab added
			std::getline(file, line);
			while (boost::starts_with(line, "\t"))
			{
				SubDatasetSpecification subSpec;
				// Split line to get name and filename
				boost::split(fields, line, boost::is_any_of(" "));
				if (fields.size() >= 2) // ignore line if not enough fields
				{
					subSpec.name = fields[0].substr(1, fields[0].size() - 1);
					subSpec.filename = fields[1];
					dsSpec.subDatasets.push_back(subSpec);
				}

 				// Break out of loop pre-maturely if end of file is reached
				if (file.eof())
					break;
				// Otherwise, get the next line
				else
					std::getline(file, line);
			}
			suite.addDataset(dsSpec);
		}

		return suite;
	}

	void TestSuite::addStructure(const IndexStructureSpecification& spec)
	{
		structures.push_back(spec);
	}

	void TestSuite::addDataset(const DatasetSpecification& spec)
	{
		datasets.push_back(spec);
	}

	const std::string& TestSuite::name() const
	{
		return suiteName;
	}

	const std::string& TestSuite::inputDirectory() const
	{
		return suiteInputDir;
	}

	const std::string& TestSuite::outputDirectory() const
	{
		return suiteOutputDir;
	}

	const std::vector<IndexStructureSpecification>& TestSuite::structureSpecs() const
	{
		return structures;
	}

	const std::vector<DatasetSpecification>& TestSuite::datasetSpecs() const
	{
		return datasets;
	}

}