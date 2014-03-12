#include "IndexStructureFactory.h"
// 
#include "SequentialScan.h"
#include "Octree.h"

namespace mdsearch
{

	/* ALL INDEX STRUCTURE GENERATORS ARE DEFINED HERE */
	IndexStructure* generateSequentialScan(unsigned int numDimensions,
		const std::vector<std::string>& args)
	{
		return new SequentialScan(numDimensions);
	}

	IndexStructure* generateOctree(unsigned int numDimensions,
		const std::vector<std::string>& args)
	{
		// TODO
		return NULL;
	}	




	IndexStructureFactory::IndexStructureFactory()
	{
		// Add generators defined in this source file
		addGenerator("sequential_scan", generateSequentialScan);
		addGenerator("octree", generateOctree);
	}

	void IndexStructureFactory::addGenerator(const std::string& structureType,
		StructureGenerator generator)
	{
		std::pair<std::string, StructureGenerator> newEntry(structureType, generator);
		generators.insert(newEntry);
	}

	IndexStructure* IndexStructureFactory::constructIndexStructure(
		const std::string& structureType, unsigned int numDimensions,
		const std::vector<std::string>& arguments) const
	{
		// Search map for a generator for the requested structure type
		GeneratorMap::const_iterator it = generators.find(structureType);
		if (it != generators.end())
		{
			StructureGenerator generator = it->second;
			return generator(numDimensions, arguments);
		}
		// Return NULL if the given structure type does not exist
		else
		{
			return NULL;
		}
	}


}