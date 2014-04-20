#ifndef MDSEARCH_SPECIFICATIONS_H
#define MDSEARCH_SPECIFICATIONS_H

#include <vector>
#include <string>
#include<sstream>

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
		std::vector<SubDatasetSpecification> subDatasets;

		StringList outputFilenames(const std::string& suiteName)
		{
			StringList filenames;

			std::stringstream ss;
			for (std::vector<SubDatasetSpecification>::const_iterator it = subDatasets.begin();
				(it != subDatasets.end()); it++)
			{
				ss << suiteName << "_" << name << "_" << it->name << ".times";
				filenames.push_back(ss.str());
				ss.str(""); // clear stream
			}

			return filenames;
		}

	};

}

#endif