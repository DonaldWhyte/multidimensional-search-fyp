#include "TestOperation.h"

namespace mdsearch
{

	TestOperation::TestOperation(TestOperation::Type type, const Point& value) : type(type), value(value)
	{
	}

	bool TestOperation::operator==(const TestOperation& other) const
	{
		return (type == other.type && value == other.value);
	}

	std::ostream& operator<<(std::ostream& out, const TestOperation& op)
	{
		out << "{" << op.type << ": " << op.value << "}";
		return out;
	}	

}