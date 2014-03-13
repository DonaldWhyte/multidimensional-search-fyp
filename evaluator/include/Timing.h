#ifndef MDSEARCH_TIMING_H
#define MDSEARCH_TIMING_H

#include <vector>

namespace mdsearch
{

	typedef long Timing;
	typedef std::vector<Timing> TimingList;

	class TimingEntries
	{

	public:
		/* Add dataset, operation list or index structure to entires.
		 * Return index to added dataset/oplist/structure. */
		unsigned int addDataset();
		unsigned int addOpList();
		unsigned int addStructure();
		/* Store timing of dataset-oplist-structure combination.
		 * Return true if timing value was successfully stored (indices were valid). */
		bool addTiming(unsigned int datasetIndex, unsigned int opListIndex,
			unsigned int structureIndex, long elapsedTime);

		/* Accessors to stored timing entries. */
		const std::vector<std::vector<TimingList> >& getAllTimings() const;
		const std::vector<TimingList>& getDatasetTimings(unsigned int datasetIndex) const;
		const TimingList& getDatasetOpListTimings(unsigned int datasetIndex,
			unsigned int opListIndex) const;	
		Timing getTiming(unsigned int datasetIndex, unsigned int opListIndex,
			unsigned int structureIndex) const;

		/* Return string that contains all stored timings for
		 * dataset/oplist/structure combinations. */
		std::string generateTimingReport() const;

	private:
		std::vector<std::vector<TimingList> > timings;

	};

}

#endif