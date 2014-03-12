#include "IndexStructureFactory.h"
// 
#include "SequentialScan.h"
#include "Octree.h"

#include <boost/lexical_cast.hpp>

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
		// Check there are enough arguments to construct n-dimensional boundary
		if (args.size() < numDimensions * 2) // min + max args needed per dimension
			return NULL;

		Region boundary(numDimensions);
		for (unsigned int d = 0; (d < numDimensions); d++)
		{
			// Parse string arguments into real numbers
			try
			{
				Real min = boost::lexical_cast<Real>(args[d * 2]);
				Real max = boost::lexical_cast<Real>(args[d * 2 + 1]);
				boundary[d] = Interval(min, max);
			}
			// If an argument could not be converted into a real number,
			// return NULL as the structure cannot be constructed 			
			catch (const boost::bad_lexical_cast& ex)
			{
				return NULL;
			}
		}

		return new Octree(numDimensions, boundary);
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