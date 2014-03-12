#ifndef MDSEARCH_TEST_OPERATION_H
#define MDSEARCH_TEST_OPERATION_H

#include <vector>
#include "Point.h"

namespace mdsearch
{

	struct TestOperation
	{

		enum Type
		{
			OPERATION_TYPE_INSERT = 0,
			OPERATION_TYPE_DELETE,
			OPERATION_TYPE_UPDATE,
			OPERATION_TYPE_POINTQUERY,
		};

		TestOperation(Type type, const Point& value) : type(type), value(value)
		{
		}

		Type type;
		Point value;

	};

	typedef std::vector<TestOperation> TestOperationList;

}

#endif