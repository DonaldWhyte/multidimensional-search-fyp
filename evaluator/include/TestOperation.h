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

		TestOperation(Type type, const Point& value);
		/* Equality test for two structure test operations. Created for unit tests. */
		bool operator==(const TestOperation& other) const;

		// Fields
		Type type;
		Point value;

	};

	typedef std::vector<TestOperation> TestOperationList;

	/* Convenience function for printing out test operations. */
	std::ostream& operator<<(std::ostream& out, const TestOperation& op);

}

#endif