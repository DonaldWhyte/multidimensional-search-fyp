#ifndef MDSEARCH_EVALUATOR_H
#define MDSEARCH_EVALUATOR_H

#include <vector>
#include "IndexStructure.h"
#include "Point.h"
#include "TestOperation.h"
#include "Timing.h"

namespace mdsearch
{

	// TODO: comment this file a fair bit
	class Evaluator
	{

	public:
		Evaluator(const std::vector<IndexStructure*>& structures);

		std::vector< timePerformance(const std::vector<PointList>& datasets,
			const std::vector<TestOperationList>& testOperationLists) const;

	private:
		long runOperations(IndexStructure* structure, const PointList& dataset,
			const TestOperationList& operations) const;

		std::vector<IndexStructure*> structures;

	};

}

#endif