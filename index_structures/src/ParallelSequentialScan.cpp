#include "ParallelSequentialScan.h"
#include <boost/function.hpp>

namespace mdsearch
{

	SubSearcher::SubSearcher() : targetPoint(NULL), start(NULL),
		numElementsToSearch(0), finished(false), found(false)
	{
	}

	void SubSearcher::setSearchTask(const Point* targetPoint, const Point* start,
		unsigned int numElementsToSearch)
	{
		this->targetPoint = targetPoint;
		this->start = start;
		this->numElementsToSearch = numElementsToSearch;
		finished = false;
		found = false;
	}

	void SubSearcher::search()
	{
		for (unsigned int i = 0; (i < numElementsToSearch); i++)
		{
			if (*start == *targetPoint)
			{
				found = true;
				break;
			}
			else
			{
				++start;
			}
		}
		finished = true;
	}

	ParallelSequentialScan::ParallelSequentialScan(int numDimensions, int numThreads)
		: SequentialScan(numDimensions), ioService(), work(ioService), numThreads(numThreads)
	{
		// Create the amount of worker threads specified
		for (unsigned int i = 0; (i < numThreads); i++)
		{
			threadPool.create_thread(
			    boost::bind(&boost::asio::io_service::run, &ioService)
			);
		}
		// Create empty searcher objects, one for each thread
		searchers.resize(numThreads);
	}

	ParallelSequentialScan::~ParallelSequentialScan()
	{
		// Stop IO service processing loop
		ioService.stop();
		// Wait for all threads to finish executing
		threadPool.join_all();
	}

	bool ParallelSequentialScan::pointExists(const Point& p)
	{
		// Partition point array into p segments, where
		// Any remaining elements are simply assigned to the last
		// thread. This will always be < p, so it shouldn't cause
		// too much idle time
		Point* start = &points[0];
		int pointsPerThread = points.size() / numThreads;
		int leftover = points.size() % numThreads;
		for (unsigned int i = 0; (i < numThreads - 1); i++) // all but last thread
		{
			searchers[i].setSearchTask(&p,
				start + (pointsPerThread * i),
				pointsPerThread);
		}
		// Assign all remaining work to last thread
		searchers[numThreads - 1].setSearchTask(&p,
			start + (pointsPerThread * (numThreads - 1)),
			pointsPerThread + leftover);
		
		// Post work to the threads
		for (unsigned int i = 0; (i < numThreads); i++)
		{
			ioService.post(
				boost::function<void (void)>( boost::bind(&SubSearcher::search, &searchers[i]) )
			);
		}
		// Continuously check if a thread has found the point
		while (true)
		{
			bool allFinished = true;
			for (unsigned int i = 0; (i < numThreads); i++)
			{
				if (searchers[i].finished)
				{
					if (searchers[i].found)
						return true;
				}
				else
				{
					allFinished = false;
				}
			}
			// If all threads are finished and none have found point,
			// then the point does not exist in the structure
			if (allFinished)
			{
				return false;
			}
		}
		return false; // shouldn't execute
	}

}