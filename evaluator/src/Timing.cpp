#include "Timing.h"

namespace mdsearch
{

	unsigned int TimingEntries::addDataset()
	{
		// Create dataset entry which has 
		std::vector<TimingList> newDSEntry;
		TimingList opListEntry(structures.size());
		for (unsigned int i = 0; (i < ))
		newDSEntry.push_back(opListEntry);

		timings.push_back();
		return timings.size() - 1;
	}

	unsigned int TimingEntries::addOpList()
	{
		// TODO
		return 0;
	}

	unsigned int TimingEntries::addStructure()
	{
		// TODO
		return 0;
	}

	bool TimingEntries::addTiming(unsigned int datasetIndex,
		unsigned int opListIndex, unsigned int structureIndex,
		long elapsedTime)
	{
		// TODO
		return false;
	}

	const std::vector<std::vector<TimingList> >& TimingEntries::getAllTimings() const
	{
		return timings;
	}

	const std::vector<TimingList>& TimingEntries::getDatasetTimings(unsigned int datasetIndex) const
	{
		return timings[datasetIndex];
	}

	const TimingList& TimingEntries::getDatasetOpListTimings(unsigned int datasetIndex,
		unsigned int opListIndex) const
	{
		return getDatasetTimings(datasetIndex)[opListIndex];
	}

	Timing TimingEntries::getTiming(unsigned int datasetIndex, unsigned int opListIndex,
		unsigned int structureIndex) const
	{
		const TimingList& dsOpTimings = getDatasetTimings(datasetIndex, opListIndex);
		return dsOpTimings[structureIndex];
	}

	/* Return string that contains all stored timings for
	 * dataset/oplist/structure combinations. */
	std::string TimingEntries::generateTimingReport() const;

}