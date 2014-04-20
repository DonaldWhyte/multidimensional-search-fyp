#include "Timing.h"
#include <fstream>
#include <sstream>

#if defined(_WIN32)
	#include <time.h>
#elif defined(__unix__)
	#include <sys/time.h>
#else
	#error No timing mechanism supported for this platform
#endif

namespace mdsearch
{

	Timing getTime()
	{
	#if defined(_WIN32)
		return static_cast<Timing>(time(NULL)); // in seconds already
	#elif defined(__unix__)
		timespec ts;
		clock_gettime(CLOCK_REALTIME, &ts); 
		return static_cast<Timing>(ts.tv_sec) + (static_cast<Timing>(ts.tv_nsec) / 1000000000.0);
	#else
		return 0.0;
	#endif
	}

	void TimingWriter::timesToFile(const std::string& filename,
		const std::string title, const std::string& xAxis,
		const std::string yAxis, const TimingCollection& timings) const
	{
		std::ofstream file(filename.c_str());
		file << title << "\n" << xAxis << " " << yAxis << "\n";
		file << timesToString(timings);
		file.close();
	}

	std::string TimingWriter::timesToString(const TimingCollection& timings) const
	{
		std::stringstream ss;
		for (TimingCollection::const_iterator it = timings.begin();
			(it != timings.end()); it++)
		{
			ss << it->first << "\n";
			for (TimingMap::const_iterator timing = it->second.begin();
				(timing != it->second.end()); timing++)
			{
				ss << timing->first << " " << timing->second << "\n";
			}
			ss << "\n";
		}
		return ss.str();
	}

}