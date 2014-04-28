#include "ClusteredDatasetGenerator.h"
#include "SkewedDatasetGenerator.h"
#include "UniformDatasetGenerator.h"
#include "Util.h"
#include <sstream>
#include <cmath>

using namespace mdsearch;

static const unsigned int DIMENSIONS[] = {
	1, 2, 3, 5, 8, 10, 30, 50, 100, 200
};
static const unsigned int NUM_DIMENSIONALITIES = 10;

static const unsigned int NUM_POINTS_PER_DATASET = 500000;
static const Real MIN_VALUE = 0.0f;
static const Real MAX_VALUE = 1.0f;

static const Real SKEW_SEVERITY = 2.0f;
static const SkewDirection SKEW_DIR = SKEW_DIRECTION_LEFT;

static const Interval UNIFORM_CLUSTER_INTERVALS[] = { // first elem 'min', second elem 'max'
	Interval(0.0f, 0.4f), // larger cluster
	Interval(0.7f, 0.75f)// small cluster
};
static const unsigned int NUM_CLUSTERS = 2;
static const unsigned int NUM_POINTS_PER_CLUSTER = NUM_POINTS_PER_DATASET / NUM_CLUSTERS;

static const unsigned int N_VARYING_DIMENSIONALITY = 16;
static const unsigned int POINT_COUNTS[] = {
	10, 100, 1000, 5000, 10000, 50000, 100000, 500000, 1000000
};
static const unsigned int NUM_POINT_COUNTS = 9;

static const std::string RANDOM_UNIFORM_DATASET_PREFIX = "randuniform";
static const std::string SKEWED_DATASET_PREFIX = "skewed_500000";
static const std::string CLUSTERED_DATASET_PREFIX = "clustered_500000";
static const std::string N_VARYING_DATASET_PREFIX = "sizevary";
static const std::string EXTENSION = "dat";

std::string datasetFilename(const std::string& prefix, unsigned int numDimensions)
{
	// Convert number of dimensions to string
	std::stringstream ss;
	ss << numDimensions;
	std::string numDimensionsStr = ss.str();
	return prefix + "_" + numDimensionsStr + "d." + EXTENSION;
}

std::string pointCountDatasetFilename(const std::string& prefix, unsigned int numPoints)
{
	std::stringstream ss;
	ss << numPoints;
	std::string numPointsStr = ss.str();
	return prefix + "_n" + numPointsStr + "." + EXTENSION;
}

int main(int argc, char* argv[])
{
	// Generate skewed datasets
	std::cout << "Generating skewed datasets..." << std::endl;
	SkewedDatasetGenerator skewedGenerator;
	for (unsigned int i = 0; (i < NUM_DIMENSIONALITIES); i++)
	{
		unsigned int d = DIMENSIONS[i];
		std::string filename = datasetFilename(SKEWED_DATASET_PREFIX, d);
		std::cout << "\tGenerating " << d << "D dataset..." << std::endl;

		Point minPoint(d, MIN_VALUE);
		Point maxPoint(d, MAX_VALUE);
		PointList dataset = skewedGenerator.generate(d, minPoint, maxPoint,		
			NUM_POINTS_PER_DATASET, SKEW_SEVERITY, SKEW_DIR);
		writePointsToFile(filename, d, dataset);
		dataset.clear();

		std::cout << "\t...generated" << std::endl;
	}

	// Generate clustered dataset
	std::cout << "Generating clustered datasets..." << std::endl;
	ClusteredDatasetGenerator clusteredGenerator;
	for (unsigned int i = 0; (i < NUM_DIMENSIONALITIES); i++)
	{
		unsigned int d = DIMENSIONS[i];
		std::string filename = datasetFilename(CLUSTERED_DATASET_PREFIX, d);
		std::cout << "\tGenerating " << d << "D dataset..." << std::endl;

		// Construct cluster specifications
		std::vector<ClusterSpecification> specifications;
		for (unsigned int i = 0; (i < NUM_CLUSTERS); i++)
		{

			Region region(d, UNIFORM_CLUSTER_INTERVALS[i]);
			specifications.push_back(
				ClusterSpecification(region, NUM_POINTS_PER_CLUSTER)
			);
		}
		// Generate the clustered data using the specifications
		PointList dataset = clusteredGenerator.generate(d, specifications);

		writePointsToFile(filename, d, dataset);
		dataset.clear();

		std::cout << "\tGenerated " << d << "D dataset" << std::endl;
	}/*

	// Generate uniformly random datasets
	std::cout << "Generating uniformly random datasets..." << std::endl;
	for (unsigned int i = 0; (i < NUM_DIMENSIONALITIES); i++)
	{
		unsigned int d = DIMENSIONS[i];
		std::string filename = datasetFilename(RANDOM_UNIFORM_DATASET_PREFIX, d);
		std::cout << "\tGenerating " << d << "D dataset..." << std::endl;

		// Construct cluster specification that corresponds to uniformly random
		// in entire dataspace
		std::vector<ClusterSpecification> specifications;
		Region region(d, Interval(MIN_VALUE, MAX_VALUE));
		specifications.push_back(ClusterSpecification(region, NUM_POINTS_PER_DATASET));
		PointList dataset = clusteredGenerator.generate(d, specifications);

		writePointsToFile(filename, d, dataset);
		dataset.clear();

		std::cout << "\tGenerated " << d << "D dataset" << std::endl;
	}

	std::cout << "Generating " << N_VARYING_DIMENSIONALITY << "D datasets of varying size" << std::endl;
	// First generate uniformly randomly distributed 100D dataset with highest number of points
	Region region(N_VARYING_DIMENSIONALITY, Interval(MIN_VALUE, MAX_VALUE));
	int maxPoints = POINT_COUNTS[NUM_POINT_COUNTS - 1];
	std::vector<ClusterSpecification> specifications;
	specifications.push_back(ClusterSpecification(region, maxPoints));
	PointList allPoints = clusteredGenerator.generate(N_VARYING_DIMENSIONALITY, specifications);
	for (unsigned int i = 0; (i < NUM_POINT_COUNTS); i++)
	{
		unsigned int n = POINT_COUNTS[i];
		std::cout << "\tGenerating " << n << " point dataset..." << std::endl;
		std::string filename = pointCountDatasetFilename(N_VARYING_DATASET_PREFIX, n);
		PointList dataset(allPoints.begin(), allPoints.begin() + n);
		std::cout << "\t\t" << dataset.size() << std::endl;
		writePointsToFile(filename, N_VARYING_DIMENSIONALITY, dataset);
	}*/

	return 0;
}