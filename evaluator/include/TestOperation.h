#ifndef MDSEARCH_TEST_OPERATION_H
#define MDSEARCH_TEST_OPERATION_H

#include <vector>
#include "Point.h"
#include "Region.h"

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
			OPERATION_TYPE_CLEAR
		};

		TestOperation(Type type, const Point& operandOne);
		TestOperation(Type type, const Point& operandOne, const Point& operandTwo);
		TestOperation(const Region& boundary); // constructor for Clear operations
		/* Equality test for two structure test operations. Created for unit tests. */
		bool operator==(const TestOperation& other) const;

		// Fields
		Type type;
		Point operand1;
		Point operand2; //only used for Update
		Region boundary; // only used for Clear

	};

	typedef std::vector<TestOperation> TestOperationList;

	/* Convenience function for printing out test operations. */
	std::ostream& operator<<(std::ostream& out, const TestOperation& op);

}

#endif