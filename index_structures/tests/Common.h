#ifndef MDSEARCH_TESTS_INDEXSTRUCTURE_COMMON_H
#define MDSEARCH_TESTS_INDEXSTRUCTURE_COMMON_H

#include "IndexStructure.h"

namespace mdsearch { namespace tests {

	/* Used to perform generic tests on ANY INDEX STRUCTURE, using
	 * the same tests for each. */
	class IndexStructureTester
	{

	public:
		static const unsigned int NUM_TEST_DIMENSIONS = 3;

		IndexStructureTester();

		/* NOTE: Structure should be completely empty before passing it
		 * to these tests. The structure will be modified (with points
		 * being added) after these are called. */
		void testUpdates(IndexStructure* structure);
		void testPointQueries(IndexStructure* structure);

		const PointList& getTestPoints() const;

	private:
		static const unsigned int NUM_TEST_POINTS = 10;	

		PointList testPoints;

	};


	/* Returns true if both vectors have elements, EVEN IF THEY ARE
	 * IN A DIFFERENT ORDER. */
	template<typename T>
	bool vectorsContainSameElements(const std::vector<T>& a, const std::vector<T>& b)
	{
		if (a.size() != b.size())
		{
			return false;
		}
		for (unsigned int i = 0; (i < a.size()); i++)
		{
			bool exists = false;
			for (unsigned int j = 0; (j < b.size()); j++)
			{
				if (a[i] == b[j])
				{
					exists = true;
					break;
				}
			}
			if (!exists)
			{
				return false;
			}
		}
		return true;
	}

} }

#endif