#ifndef MDSEARCH_INDEXSTRUCTUREFACTORY_H
#define MDSEARCH_INDEXSTRUCTUREFACTORY_H

#include <map>
#include <vector>
#include <string>
#include "IndexStructure.h"

namespace mdsearch
{

	// Pointer to a function which returns a pointer to a newly
	// constructed index structure. It requires the number of
	// dimensions and a list of string arguments in order
	// to construct the structure.
	//
	// If the construction of the structure fails because the
	// required arguments were not passed/valid (or for some
	// other reason), the function returns NULL.
	typedef IndexStructure* (*StructureGenerator)(unsigned int, const Region&, const std::vector<std::string>&);

	class IndexStructureFactory
	{

	public:
		IndexStructureFactory();

		/* Add generator with given name and function to factory. */
		void addGenerator(const std::string& structureType, StructureGenerator generator);

		/* Construct index structure of given type, using the specified number of
		 * dimensions and arguments. */
		IndexStructure* constructIndexStructure(const std::string& structureType,
			 unsigned int numDimensions, const Region& boundary,
			 const std::vector<std::string>& arguments) const;

	private:
		typedef std::map<std::string, StructureGenerator> GeneratorMap;

		GeneratorMap generators;


	};

}

#endif