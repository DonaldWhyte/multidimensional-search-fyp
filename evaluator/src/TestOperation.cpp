#include "TestOperation.h"

namespace mdsearch
{

	TestOperation::TestOperation(Type type, const Point& operandOne) :
		type(type), operand1(operandOne), operand2(Point(operandOne.numDimensions(), 0.0f))
	{
	}

	TestOperation::TestOperation(Type type, const Point& operandOne, const Point& operandTwo) :
		type(type), operand1(operandOne), operand2(operandTwo)
	{
	}

	bool TestOperation::operator==(const TestOperation& other) const
	{
		return (type == other.type && operand1 == other.operand1
			&& operand2 == other.operand2);
	}

	std::ostream& operator<<(std::ostream& out, const TestOperation& op)
	{
		out << "{" << op.type << ": " << op.operand1 << "," << op.operand2 << "}";
		return out;
	}	

}