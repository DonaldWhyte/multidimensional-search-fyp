#include "TestOperation.h"

namespace mdsearch
{

	TestOperation::TestOperation(Type type, const Point& operandOne) :
		type(type), operand1(operandOne), operand2(Point(operandOne.numDimensions(), 0.0f)),
		boundary(0)
	{
	}

	TestOperation::TestOperation(Type type, const Point& operandOne, const Point& operandTwo) :
		type(type), operand1(operandOne), operand2(operandTwo), boundary(0)
	{
	}

	TestOperation::TestOperation(const Region& boundary) :
		type(TestOperation::OPERATION_TYPE_CLEAR), operand1(0), operand2(0), boundary(boundary)
	{
	}

	bool TestOperation::operator==(const TestOperation& other) const
	{
		return (type == other.type && operand1 == other.operand1
			&& operand2 == other.operand2 && boundary == other.boundary);
	}

	std::ostream& operator<<(std::ostream& out, const TestOperation& op)
	{
		out << "{" << op.type << ": " << op.operand1 << "," << op.operand2 << "}";
		return out;
	}	

}