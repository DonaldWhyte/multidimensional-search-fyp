/******************************************************************************

mdsearch - Lightweight C++ library implementing a collection of
           multi-dimensional search structures

File:        bithash.hpp
Description: TODO

*******************************************************************************

The MIT License (MIT)

Copyright (c) 2014 Donald Whyte

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

******************************************************************************/

#ifndef MDSEARCH_HASHTABLE_H
#define MDSEARCH_HASHTABLE_H

#include "hashstruct.hpp"
#include <boost/functional/hash.hpp>

namespace mdsearch
{

	template<int D>
	class BitHash : public HashStructure<D>
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