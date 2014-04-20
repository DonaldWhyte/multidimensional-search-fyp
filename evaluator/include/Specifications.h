#ifndef MDSEARCH_SPECIFICATIONS_H
#define MDSEARCH_SPECIFICATIONS_H

#include <vector>
#include <string>
#include <sstream>

namespace mdsearch
{

	typedef std::vector<std::string> StringList;

	struct IndexStructureSpecification
	{
		std::string type;
		StringList arguments;
	};

	struct SubDatasetSpecification
	{
		std::string name;
		std::string filename;
	};

	struct DatasetSpecification
	{ 
		std::string name;
		// empty string if no parameter is varied -- determines label of X axis!
		std::string varyingParameter;
		std::vector<SubDatasetSpecification> subDatasets;
	};

}

#endif