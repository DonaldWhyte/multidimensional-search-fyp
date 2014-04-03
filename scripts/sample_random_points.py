import sys
import random
import time
from generate_dataset_operations import loadDataset
from generate_random_dataset import pointToStr

def randomlySampleDataset(dataset, pointsToSample):
	fullDataset = list(dataset) # copied since it will modified
	sampledDataset = []
	for i in range(pointsToSample):
		# Generate random index into full dataset and place
		# point at given index into sampled dataset
		index = random.randint(0, len(fullDataset) - 1)
		sampledDataset.append(fullDataset[index])
		# Remove selected point from the full dataset so it
		# won't be chosen again
		del fullDataset[index]
	return sampledDataset

def outputDataset(numDimensions, dataset):
	print("{} {}".format(numDimensions, len(dataset)))
	for ds in dataset:
		print(pointToStr(ds))

if __name__ == "__main__":
	random.seed(time.time())

	# Parse command line arguments
	if len(sys.argv) < 3:
		sys.exit("python {} <datasetFilename> <pointsToSample>".format(sys.argv[0]))
	datasetFilename = sys.argv[1]
	pointsToSample = int(sys.argv[2])

	numDimensions, numPoints, dataset = loadDataset(datasetFilename)
	# Done in case there are less points in dataset than points to sample
	pointsToSample = min(pointsToSample, len(dataset))
	dataset = randomlySampleDataset(dataset, pointsToSample)
	outputDataset(numDimensions, dataset)