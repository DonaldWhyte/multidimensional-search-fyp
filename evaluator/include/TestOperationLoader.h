#ifndef MDSEARCH_TESTOPERATIONLOADER_H
#define MDSEARCH_TESTOPERATIONLOADER_H

#include <string>
#include "TestOperation.h"

namespace mdsearch
{

	class TestOperationLoader
	{

	public:
		TestOperationList loadFromFile(const std::string& filename) const;

	};

}

#endif