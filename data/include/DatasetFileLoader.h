#ifndef MDSEARCH_DATASETFILELOADER_H
#define MDSEARCH_DATASETFILELOADER_H

#include <string>
#include "Point.h"

namespace mdsearch
{

	class DatasetFileLoader
	{

	public:
		PointList load(const std::string& filename);

	};

}

#endif