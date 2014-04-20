#include "Timing.h"
#include <fstream>
#include <sstream>

namespace mdsearch
{

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