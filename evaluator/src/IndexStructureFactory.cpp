#include "IndexStructureFactory.h"

#include "SequentialScan.h"
#include "Octree.h"
#include "PyramidTree.h"
#include <boost/lexical_cast.hpp>

namespace mdsearch
{

	Region parseBoundary(unsigned int numDimensions,
		const std::vector<std::string>& args)
	{
		Region boundary(numDimensions);
		for (unsigned int d = 0; (d < numDimensions); d++)
		{
			// Parse string arguments into real numbers
			Real min = boost::lexical_cast<Real>(args[d * 2]);
			Real max = boost::lexical_cast<Real>(args[d * 2 + 1]);
			boundary[d] = Interval(min, max);
		}
		return boundary;		
	}

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

		try
		{
			Region boundary = parseBoundary(numDimensions, args);
			return new Octree(numDimensions, boundary);
		}	
		catch (const boost::bad_lexical_cast& ex)
		{
			return NULL;
		}		
	}	

	IndexStructure* generatePyramidTree(unsigned int numDimensions,
		const std::vector<std::string>& args)
	{
		unsigned int requiredArgCount = numDimensions * 2;
		if (args.size() < requiredArgCount)
			return NULL;

		try
		{
			Region boundary = parseBoundary(numDimensions, args);
			// Parse optional "MAX EMPTY ELEMENTS ALLOWED" argument
			int maxEmptyElements = -1;
			if (args.size() >= requiredArgCount + 1)
			{
				const std::string& emptyElementArg = args[requiredArgCount];
				maxEmptyElements = boost::lexical_cast<int>(emptyElementArg);
				// If negative maximum given, return NULL as it's invalid!
				if (maxEmptyElements < 0)
					return NULL;
			}
			if (maxEmptyElements != -1)
				return new PyramidTree(numDimensions, boundary, maxEmptyElements);
			else
				return new PyramidTree(numDimensions, boundary);
		}	
		catch (const boost::bad_lexical_cast& ex)
		{
			return NULL;
		}
	}




	IndexStructureFactory::IndexStructureFactory()
	{
		// Add generators defined in this source file
		addGenerator("sequential_scan", generateSequentialScan);
		addGenerator("octree", generateOctree);
		addGenerator("pyramid_tree", generatePyramidTree);
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