#ifndef MDSEARCH_PARALLELSEQUENTIALSCAN_H
#define MDSEARCH_PARALLELSEQUENTIALSCAN_H

#include "SequentialScan.h"
#include <boost/bind.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/thread/thread.hpp>

namespace mdsearch
{

	/* Functor used to */
	class SubSearcher
	{

	public:
		SubSearcher();
		void setSearchTask(const Point* targetPoint, const Point* start, unsigned int numElementsToSearch);		
		void search();

		const Point* targetPoint; // point to search for
		const Point* start; // start of array to search
		unsigned int numElementsToSearch;
		volatile bool finished; // set to true if this object has finished the search
		volatile bool found; // set to true if the point does exist in this sub-array

	};

	class ParallelSequentialScan : public SequentialScan
	{

	public:
		ParallelSequentialScan(int numDimensions, int numThreads);
		virtual ~ParallelSequentialScan();

		bool pointExists(const Point& p);

	private:
		// Used for the underlying thread pool used for parallel search
		boost::asio::io_service ioService;
		boost::asio::io_service::work work;
		boost::thread_group threadPool;
		int numThreads;
		// Contains all searcher objects which are posted to worker threads
		std::vector<SubSearcher> searchers;

	};

}

#endif