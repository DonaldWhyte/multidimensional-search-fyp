#ifndef MDSEARCH_TESTSUITE_H
#define MDSEARCH_TESTSUITE_H

#include "Specifications.h"

namespace mdsearch
{

	class TestSuite
	{

	public:
		TestSuite(const std::string& suiteName, const std::string& suiteInputDir,
			const std::string& suiteOutputDir);
		static TestSuite fromFile(const std::string& filename);

		void addStructure(const IndexStructureSpecification& spec);
		void addDataset(const DatasetSpecification& spec);

		const std::string& name() const;
		const std::string& inputDirectory() const;
		const std::string& outputDirectory() const;
		const std::vector<IndexStructureSpecification>& structureSpecs() const;
		const std::vector<DatasetSpecification>& datasetSpecs() const;

	private:
		std::string suiteName;
		std::string suiteInputDir;
		std::string suiteOutputDir;
		std::vector<IndexStructureSpecification> structures;
		std::vector<DatasetSpecification> datasets;

	};

}

#endif