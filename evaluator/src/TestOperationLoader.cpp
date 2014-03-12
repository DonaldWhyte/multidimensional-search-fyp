#include "TestOperationLoader.h"
#include <fstream>
#include <boost/algorithm/string.hpp> // for split()
#include <boost/lexical_cast.hpp>

namespace mdsearch
{

	TestOperationList TestOperationLoader::loadFromFile(const std::string& filename) const
	{
		TestOperationList operations;

		std::ifstream file(filename.c_str());
		if (!file.is_open())
			return operations;
		// Used to read file line-by-line
		std::string line;
		std::vector<std::string> fields;

		// Get number of test operations contained in file and dimensionality of points
		std::getline(file, line);
		boost::split(fields, line, boost::is_any_of(" "));
		if (fields.size() < 2)
			return operations;
		unsigned int numDimensions = 0;
		unsigned int numOps = 0;
		try
		{
			numDimensions = boost::lexical_cast<unsigned int>(fields[0]);
			numOps = boost::lexical_cast<unsigned int>(fields[1]);
		}
		// Return empty operations list if the header line is invalid
		catch (boost::bad_lexical_cast)
		{
			return operations;
		}
		// Use these counts to 
		std::vector<Real> pointValues(numDimensions);
		operations.reserve(numOps);

		// Go through each line, looking for another test operation in each
		while (!file.eof())
		{
			// Read next line in
			std::getline(file, line);
			boost::split(fields, line, boost::is_any_of(" "));
			// Ensure there are enough fields to define an operation type and a point
			// If there isn't, just ignore this line
			if (fields.size() < (numDimensions + 1))
				continue;

			// Read operation type
			TestOperation::Type type;
			if (fields[0] == "I")
				type = TestOperation::OPERATION_TYPE_INSERT;
			else if (fields[0] == "D")
				type = TestOperation::OPERATION_TYPE_DELETE;
			else if (fields[0] == "U")
				type = TestOperation::OPERATION_TYPE_UPDATE;
			else if (fields[0] == "P")
				type = TestOperation::OPERATION_TYPE_POINTQUERY;
			else // ignore operation if the type is not known
				continue;
			// Read the coordinates of the operation's point
			for (unsigned int d = 0; (d < numDimensions); d++)
			{
				// Try storing next field as a real number
				// If this fails, then the point's values are invalid so
				// we ignore this test operation and move onto the next
				try
				{
					pointValues[d] = boost::lexical_cast<Real>( fields[d + 1] );
				}
				catch (const boost::bad_lexical_cast& ex)
				{
					continue;
				}
			}
			Point point(numDimensions, &pointValues[0]);

			operations.push_back( TestOperation(type, point) );
		}

		return operations;
	}

}