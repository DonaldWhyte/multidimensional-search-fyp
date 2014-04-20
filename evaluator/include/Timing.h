#ifndef MDSEARCH_TIMING_H
#define MDSEARCH_TIMING_H

#include <string>
#include <vector>
#include <map>

namespace mdsearch
{

	typedef double Timing;
	typedef std::map<std::string, Timing> TimingMap;
	typedef std::map<std::string, TimingMap> TimingCollection;

	class TimingWriter
	{

	public:
		void timesToFile(const std::string& filename, const std::string title,
			const std::string& xAxis, const std::string yAxis, 
			const TimingCollection& timings) const;
		std::string timesToString(const TimingCollection& timings) const;

	};

}

#endif