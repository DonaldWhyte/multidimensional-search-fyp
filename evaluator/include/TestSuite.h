#ifndef MDSEARCH_TESTSUITE_H
#define MDSEARCH_TESTSUITE_H

#include "Specifications.h"

namespace mdsearch
{

	class TestSuite
	{

	public:
		TestSuite();
		static TestSuite fromFile(const std::string& filename);

		void addStructure(const IndexStructureSpecification& spec);
		void addDataset(const DatasetSpecification& spec);

		const std::vector<IndexStructureSpecification>& structureSpecs() const;
		const std::vector<DatasetSpecification>& datasetSpecs() const;
		

	private:
		std::vector<IndexStructureSpecification> structures;
		std::vector<DatasetSpecification> datasets;


	};

}

#endif