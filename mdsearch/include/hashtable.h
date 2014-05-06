#ifndef MDSEARCH_HASHTABLE_H
#define MDSEARCH_HASHTABLE_H

#include "hashstruct.h"
#include <boost/functional/hash.hpp>

namespace mdsearch
{

	template<int D>
	class HashTable : public HashStructure<D>
	{

	protected:
		virtual HashType hashPoint(const Point<D>& p)
		{
			size_t seed = 0;
			const Real* coord = &(p.values[0]);
			const Real* end = coord + D;
			for (coord; (coord != end); ++coord)
			{
				boost::hash_combine(seed, *coord);
			}
			return seed;			
		}

	};

}

#endif