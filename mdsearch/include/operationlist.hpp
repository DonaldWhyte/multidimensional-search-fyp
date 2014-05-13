/******************************************************************************

mdsearch - Lightweight C++ library implementing a collection of
           multi-dimensional search structures

File:        timing.hpp
Description: TODO

*******************************************************************************

The MIT License (MIT)

Copyright (c) 2014 Donald Whyte

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

******************************************************************************/

#ifndef MDSEARCH_OPERATIONLIST_H
#define MDSEARCH_OPERATIONLIST_H

#include <fstream>
#include <boost/algorithm/string.hpp> // for split()
#include <boost/lexical_cast.hpp>
#include "types.hpp"

namespace mdsearch
{

	enum OperationType
	{
		OPERATION_TYPE_INSERT = 0,
		OPERATION_TYPE_REMOVE,
		OPERATION_TYPE_QUERY,
		OPERATION_TYPE_CLEAR,
		OPERATION_TYPE_UNKNOWN
	};

	template<int D>
	struct Operation
	{
		OperationType type;
		Point<D> operand;
		Boundary<D> boundary;
	};

	template<int D>
	std::ostream& operator<<(std::ostream& out, const Operation<D>& op)
	{
		if (op.type == OPERATION_TYPE_CLEAR)
		{
			out << "(" << op.type << ": " << op.boundary << ")";
		}
		else
		{
			out << "(" << op.type << ": " << op.operand << ")";
		}
		return out;
	}

	template<int D>
	class OperationList
	{

	public:
		void load(const std::vector<Operation<D> >& newOperations)
		{
			operations.reserve(operations.size() + newOperations.size());
			// Append given points to end of current point list
			operations.insert(operations.end(), newOperations.begin(), newOperations.end());
		}

		void load(const std::string& filename)
		{
			std::ifstream file(filename.c_str());
			if (!file.is_open())
				return;

			// Read header information
			std::string line;
			std::vector<std::string> fields;
			std::getline(file, line);
			boost::split(fields, line, boost::is_any_of(" "));

			int numDimensions;
			int numOperations;
			try
			{
				numDimensions = boost::lexical_cast<int>(fields[0]);
				numOperations = boost::lexical_cast<int>(fields[1]);
			}
			catch (boost::bad_lexical_cast& ex) // not integers -- invalid file!!
			{
				return;
			}
			if (numDimensions < 1 || numOperations < 1)
				return;

			operations.reserve(operations.size() + numOperations);

			// Treat the rest of lines as operations
			std::getline(file, line);
			while (!file.eof())
			{
				// Split line into fields
				boost::split(fields, line, boost::is_any_of(" "));
				// Determine operation type
				OperationType opType = OPERATION_TYPE_UNKNOWN;
				if (fields[0] == "I")
					opType = OPERATION_TYPE_INSERT;
				else if (fields[0] == "D")
					opType = OPERATION_TYPE_REMOVE;
				else if (fields[0] == "P")
					opType = OPERATION_TYPE_QUERY;
				else if (fields[0] == "C")
					opType = OPERATION_TYPE_CLEAR;
				// Ignore operation if not recognised type
				if (opType == OPERATION_TYPE_UNKNOWN)
					continue;
				// Read operand values
				Point<D> operand;
				Boundary<D> boundary;

				if (opType == OPERATION_TYPE_CLEAR) // parse operand as a boundary, not a point
				{
					std::vector<std::string> interval;
					for (unsigned int d = 0; (d < numDimensions); d++)	
					{
						boost::split(interval, fields[d + 1], boost::is_any_of(","));
						boundary.minVal(d) = boost::lexical_cast<Real>(interval[0]);
						boundary.maxVal(d) = boost::lexical_cast<Real>(interval[1]);
					}
				}
				else
				{
					for (unsigned int d = 0; (d < numDimensions); d++)
					{
						operand[d] =  boost::lexical_cast<Real>(fields[d + 1]);
					}
				}
				// Add operation
				Operation<D> op = { opType, operand };
				operations.push_back(op);

				std::getline(file, line);
			}
		}

		const std::vector<Operation<D> >& getOperations() const
		{
			return operations;
		}

	private:
		std::vector<Operation<D> > operations;

	};

	template<typename STRUCTURE_TYPE, int D>
	class OperationListExecutor
	{

	public:
		void execute(STRUCTURE_TYPE* structure, const OperationList<D>& opList)
		{
			typedef std::vector<Operation<D> > OpVector;
			const OpVector& operations = opList.getOperations();
			for (typename OpVector::const_iterator op = operations.begin();
				(op != operations.end()); ++op)
			{
				switch (op->type)
				{
				case OPERATION_TYPE_INSERT:
					structure->insert(op->operand);
					break;
				case OPERATION_TYPE_QUERY:
					structure->query(op->operand);
					break;
				case OPERATION_TYPE_REMOVE:
					structure->remove(op->operand);
					break;
				case OPERATION_TYPE_CLEAR:
					structure->clear();
					break;
				}
			}
		}

		void executeWithBoundary(STRUCTURE_TYPE* structure, const OperationList<D>& opList)
		{
			typedef std::vector<Operation<D> > OpVector;
			const OpVector& operations = opList.getOperations();
			for (typename OpVector::const_iterator op = operations.begin();
				(op != operations.end()); ++op)
			{
				switch (op->type)
				{
				case OPERATION_TYPE_INSERT:
					structure->insert(op->operand);
					break;
				case OPERATION_TYPE_QUERY:
					structure->query(op->operand);
					break;
				case OPERATION_TYPE_REMOVE:
					structure->remove(op->operand);
					break;
				case OPERATION_TYPE_CLEAR:
					structure->clear(op->boundary);
					break;
				}
			}
		}

	};

}

#endif