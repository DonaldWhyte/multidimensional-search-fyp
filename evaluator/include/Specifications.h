#ifndef MDSEARCH_SPECIFICATIONS_H
#define MDSEARCH_SPECIFICATIONS_H

#include <vector>
#include <string>

namespace mdsearch
{

	typedef std::vector<std::string> StringList;

	struct IndexStructureSpecification
	{
		std::string type;
		StringList arguments;
	};

	struct OperationListSpecification
	{
		std::string name;
		std::string filename;
	};

	struct DatasetSpecification
	{
		std::string name;
		std::vector<OperationListSpecification> opListFilenames;
	};

}

#endif