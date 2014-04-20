#include "SpecEvaluator.h"
#include "IndexStructureFactory.h"
#include "DatasetFileLoader.h"

namespace mdsearch
{

	std::string joinPath(const std::string& a, const std::string& b)
	{
		// TODO: make more cross-platform
		return a + "/" + b;
	}

	// Contains timings for each operation, typically using
	// Insert-Query-Delete operation list, for one structure-subdataset pair
	struct OperationTimings
	{
		Timing insert;
		Timing remove;
		Timing pointQuery;
	};	

	/* If this is greater than 0, then the tight boundary around a
	 * dataset's points is expanded slightly to prevent some points
	 * being on the structure boundary.
	 * EXAMPLE: A value of 0.1 means that the MIN boundary values
	 * are 0.9 times the original value and the MAX boundary
	 * values are 1.1 times the original. */
	static const Real TIGHT_BOUNDARY_PADDING_FACTOR = 0.0f;

	Region computeDatasetBoundary(const PointList& dataset)
	{
		if (!dataset.empty())
		{
			// Use first point for dimensionality and initial boundary
			const Point& firstPoint = dataset[0];
			unsigned int numDimensions = firstPoint.numDimensions();
			Region boundary(numDimensions);
			for (unsigned int d = 0; (d < numDimensions); d++)
			{
				boundary[d].min = firstPoint[d];
				boundary[d].max = firstPoint[d];
			}
			// Now search through rest of points in dataset to find
			// minimum and maximum values for each dimension
			for (unsigned int i = 1; (i < dataset.size()); i++)
			for (PointList::const_iterator p = dataset.begin() + 1; (p != dataset.end()); p++)
			{
				for (unsigned int d = 0; (d < numDimensions); d++)
				{
					if ((*p)[d] < boundary[d].min)
						boundary[d].min = (*p)[d];
					else if ((*p)[d] > boundary[d].max)
						boundary[d].max = (*p)[d];
				}
			}
			// Pad out the boundary to leave some space between the points
			// at the boundaries of the dataset and the structures' boundaries
			for (unsigned int d = 0; (d < numDimensions); d++)
			{
				boundary[d].min = (1.0f - TIGHT_BOUNDARY_PADDING_FACTOR) * boundary[d].min;
				boundary[d].max = (1.0f + TIGHT_BOUNDARY_PADDING_FACTOR) * boundary[d].max;
			}
			return boundary;
		}
		else
		{
			return Region(0);
		}
	}

	DatasetTimings SpecEvaluator::evaluatePerformance(const TestSuite& suite) const
	{
		DatasetFileLoader datasetLoader;
		IndexStructureFactory structureFactory;
		const std::vector<DatasetSpecification>& dsSpecs = suite.datasetSpecs();
		const std::vector<IndexStructureSpecification>& structSpecs = suite.structureSpecs();

		DatasetTimings allTimings;
		
		// Iterate through each dataset, running tests for each
		for (std::vector<DatasetSpecification>::const_iterator dsSpec = dsSpecs.begin();
			(dsSpec != dsSpecs.end()); dsSpec++)
		{
			// Initialise empty timing collections for all structures and operations
			TimingCollection dsTimings;
			for (std::vector<IndexStructureSpecification>::const_iterator structSpec = structSpecs.begin();
				(structSpec != structSpecs.end()); structSpec++)
			{
				dsTimings[structSpec->type + " insert"] = TimingMap();
				dsTimings[structSpec->type + " delete"] = TimingMap();
				dsTimings[structSpec->type + " pquery"] = TimingMap();
			}

			const std::vector<SubDatasetSpecification>& subSpecs = dsSpec->subDatasets;
			for (std::vector<SubDatasetSpecification>::const_iterator subSpec = subSpecs.begin();
				(subSpec != subSpecs.end()); subSpec++)
			{
				// Load sub-dataset into main memory
				std::string fullPath = joinPath(suite.inputDirectory(), subSpec->filename);
				PointList dataset = datasetLoader.load(fullPath);
				// Determine dimensionality and bounding box of points
				Region boundary = computeDatasetBoundary(dataset);
				
				// For each structure, perform Insert-Query-Delete operation list and time each operation type
				for (std::vector<IndexStructureSpecification>::const_iterator structSpec = structSpecs.begin();
					(structSpec != structSpecs.end()); structSpec++)
				{					
					OperationTimings totalOpTimings = { 0 }; // initialise all elements to 0

					// HACK FOR OCTREE:
					// If this structure is an Octree, and either the dimensionalityof the points is > 10
					// or the number of points in the dataset is > 10000, then DON'T RUN THE TEST!
					// Simply assign -1 to all execution times and move onto next strucutre.
					// This is to avoid out-of-memory errors
					if (structSpec->type == "octree" && (boundary.numDimensions() > 10 || dataset.size() > 10000))
					{
						dsTimings[structSpec->type + " insert"][subSpec->name] = -1;
						dsTimings[structSpec->type + " delete"][subSpec->name] = -1;
						dsTimings[structSpec->type + " pquery"][subSpec->name] = -1;
						continue; // go to next structure
					}

					for (unsigned int i = 0; (i < suite.numRunsPerTiming()); i++)
					{
						IndexStructure* structure = structureFactory.constructIndexStructure(
							structSpec->type, boundary.numDimensions(), boundary, structSpec->arguments);

						// TIME INSERT
						Timing startTime = getTime();
						for (PointList::const_iterator p = dataset.begin(); (p != dataset.end()); p++)
							structure->insert(*p);
						totalOpTimings.insert += getTime() - startTime;
						// TIME PQUERY
						startTime = getTime();
						for (PointList::const_iterator p = dataset.begin(); (p != dataset.end()); p++)
							structure->pointExists(*p);
						totalOpTimings.pointQuery += getTime() - startTime;
						// TIME DELETE
						startTime = getTime();
						for (PointList::const_iterator p = dataset.begin(); (p != dataset.end()); p++)
							structure->remove(*p);
						totalOpTimings.remove += getTime() - startTime;

						delete structure;
					}
					// Compute and store AVERAGE of all runs
					dsTimings[structSpec->type + " insert"][subSpec->name] = totalOpTimings.insert / suite.numRunsPerTiming();
					dsTimings[structSpec->type + " delete"][subSpec->name] = totalOpTimings.remove / suite.numRunsPerTiming();
					dsTimings[structSpec->type + " pquery"][subSpec->name] = totalOpTimings.pointQuery / suite.numRunsPerTiming();
					
					
				}
			}

			allTimings[dsSpec->name] = dsTimings;
		}

		return allTimings;
	}

}