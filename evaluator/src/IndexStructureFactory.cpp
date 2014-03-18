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
		// Parse string arguments into real numbers that define
		// interval for each dimension
		Real min = boost::lexical_cast<Real>(args[0]);
		Real max = boost::lexical_cast<Real>(args[1]);
		// Construct Regio n object to represent boundary
		Region boundary(numDimensions);
		for (unsigned int d = 0; (d < numDimensions); d++)
		{
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
		if (args.size() < 2) // min + max args for every dimension
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
		unsigned int requiredArgCount = 2;
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
		// Cannot construct index structure with zero dimensions
		if (numDimensions == 0)
			return NULL;

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