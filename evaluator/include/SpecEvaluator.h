#ifndef MDSEARCH_SPECEVALUATOR_H
#define MDSEARCH_SPECEVALUATOR_H

#include "Timing.h"
#include "TestSuite.h"

namespace mdsearch
{

	std::string joinPath(const std::string& a, const std::string& b);

	// Key = dataset name, value = timing collection containing
	// times for each structure-subdataset combination (plus 3 ops)
	typedef std::map<std::string, TimingCollection> DatasetTimings;
	// Contains timings for each operation, typically using
	// INnert-Query-Delete operation list, for one structure-subdataset pair
	struct OperationTimings
	{
		Timing insert;
		Timing remove;
		Timing pointQuery;
	};

	class SpecEvaluator
	{

	public:
		DatasetTimings evaluatePerformance(const TestSuite& suite) const;

	};

}

#endif