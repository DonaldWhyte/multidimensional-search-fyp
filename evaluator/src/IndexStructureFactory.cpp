#include "IndexStructureFactory.h"

#include "SequentialScan.h"
#include "Octree.h"
#include "PyramidTree.h"
#include "IndexPyramidTree.h"
#include "SplayPyramidTree.h"
#include "PseudoPyramidTree.h"
#include "BoundaryDistanceHashing.h"
#include "KDTree.h"
#include "UniqueHashTable.h"
#include "DuplicateHashTable.h"
#include "BucketKDTree.h"

#include <boost/lexical_cast.hpp>

namespace mdsearch
{

	/* ALL INDEX STRUCTURE GENERATORS ARE DEFINED HERE */
	IndexStructure* generateSequentialScan(unsigned int numDimensions,
		const Region& boundary, const std::vector<std::string>& args)
	{
		return new SequentialScan(numDimensions);
	}

	IndexStructure* generateOctree(unsigned int numDimensions,
		const Region& boundary, const std::vector<std::string>& args)
	{
		return new Octree(numDimensions, boundary);		
	}

	IndexStructure* generatePyramidTree(unsigned int numDimensions,
		const Region& boundary, const std::vector<std::string>& args)
	{
		return new PyramidTree(numDimensions, boundary);		
	}		

	IndexStructure* generateIndexPyramidTree(unsigned int numDimensions,
		const Region& boundary, const std::vector<std::string>& args)
	{
		try
		{
			// Parse optional "MAX EMPTY ELEMENTS ALLOWED" argument
			int maxEmptyElements = -1; // default is NO CLEANING UP EMPTY ELEMENTS
			if (args.size() >= 1)
			{
				const std::string& emptyElementArg = args[0];
				maxEmptyElements = boost::lexical_cast<int>(emptyElementArg);
				// If negative maximum given (other than -1), return NULL
				// as it's invalid!
				if (maxEmptyElements < -2)
					return NULL;
			}
			// Parse optional 'CLEANUP PROCEDURE' ARGUMENT
			IndexPyramidTree::CleanupProcedure cleanupProc = IndexPyramidTree::CLEANUP_PROC_DEFRAGMENT; // default is defragmentation
			if (args.size() >= 2)
			{
				const std::string& procStr = args[1];
				if (procStr == "defragment")
					cleanupProc = IndexPyramidTree::CLEANUP_PROC_DEFRAGMENT;
				else if (procStr == "rebuild")
					cleanupProc = IndexPyramidTree::CLEANUP_PROC_REBUILD;
			}

			return new IndexPyramidTree(numDimensions, boundary, maxEmptyElements, cleanupProc);
		}	
		catch (const boost::bad_lexical_cast& ex)
		{
			return NULL;
		}
	}

	IndexStructure* generateSplayPyramidTree(unsigned int numDimensions,
		const Region& boundary, const std::vector<std::string>& args)
	{
		return new SplayPyramidTree(numDimensions, boundary);
	}

	IndexStructure* generatePseudoPyramidTree(unsigned int numDimensions,
		const Region& boundary, const std::vector<std::string>& args)
	{
		return new PseudoPyramidTree(numDimensions, boundary);	
	}

	IndexStructure* generateBoundaryDistanceHashing(unsigned int numDimensions,
		const Region& boundary, const std::vector<std::string>& args)
	{
		return new BoundaryDistanceHashing(numDimensions, boundary);
	}

	IndexStructure* generateKDTree(unsigned int numDimensions,
		const Region& boundary, const std::vector<std::string>& args)
	{
		return new KDTree(numDimensions);
	}

	IndexStructure* generateUniqueHashTable(unsigned int numDimensions,
		const Region& boundary, const std::vector<std::string>& args)
	{
		return new UniqueHashTable(numDimensions);
	}

	IndexStructure* generateDuplicateHashTable(unsigned int numDimensions,
		const Region& boundary, const std::vector<std::string>& args)
	{
		return new DuplicateHashTable(numDimensions);
	}

	IndexStructure* generateBucketKDTree(unsigned int numDimensions,
		const Region& boundary, const std::vector<std::string>& args)
	{
		return new BucketKDTree(numDimensions, 2, 1);
	}




	IndexStructureFactory::IndexStructureFactory()
	{
		// Add generators defined in this source file
		addGenerator("sequential_scan", generateSequentialScan);
		addGenerator("octree", generateOctree);
		addGenerator("pyramid_tree", generatePyramidTree);
		addGenerator("index_pyramid_tree", generateIndexPyramidTree);
		addGenerator("splay_pyramid_tree", generateSplayPyramidTree);
		addGenerator("pseudo_pyramid_tree", generatePseudoPyramidTree);
		addGenerator("bdh", generateBoundaryDistanceHashing);
		addGenerator("kdtree", generateKDTree);
		addGenerator("uht", generateUniqueHashTable);
		addGenerator("dht", generateDuplicateHashTable);
		addGenerator("bucket_kdtree", generateBucketKDTree);
	}

	void IndexStructureFactory::addGenerator(const std::string& structureType,
		StructureGenerator generator)
	{
		std::pair<std::string, StructureGenerator> newEntry(structureType, generator);
		generators.insert(newEntry);
	}

	IndexStructure* IndexStructureFactory::constructIndexStructure(
		const std::string& structureType, unsigned int numDimensions,
		const Region& boundary, const std::vector<std::string>& arguments) const
	{
		// Cannot construct index structure with zero dimensions
		if (numDimensions == 0)
			return NULL;

		// Search map for a generator for the requested structure type
		GeneratorMap::const_iterator it = generators.find(structureType);
		if (it != generators.end())
		{
			StructureGenerator generator = it->second;
			return generator(numDimensions, boundary, arguments);
		}
		// Return NULL if the given structure type does not exist
		else
		{
			return NULL;
		}
	}

}