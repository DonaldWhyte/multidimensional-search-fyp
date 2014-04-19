#include "TestSuite.h"

namespace mdsearch
{

	TestSuite::TestSuite()
	{
	}

	TestSuite TestSuite::fromFile(const std::string& filename)
	{
		return TestSuite();
	}

	void TestSuite::addStructure(const IndexStructureSpecification& spec)
	{
		structures.push_back(spec);
	}

	void TestSuite::addDataset(const DatasetSpecification& spec)
	{
		datasets.push_back(spec);
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