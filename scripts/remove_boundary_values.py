import sys
from generate_dataset_operations import loadDataset
from generate_random_dataset import pointToStr

if __name__ == "__main__":
	# Parse command line arguments
	if len(sys.argv) < 2:
		sys.exit("python {} <datasetFilename>".format(sys.argv[0]))
	datasetFilename = sys.argv[1]

	numDimensions, numPoints, dataset = loadDataset(datasetFilename)
	if numPoints == 0:
		print("0 0\n")
	else:
		# Compute minimum bounding box to find boundary values
		minBBox = [ [ dataset[0][d], dataset[0][d] ] for d in range(numDimensions) ]
		for point in dataset[1:]:
			for d in range(numDimensions):
				if point[d] < minBBox[d][0]: # min val
					minBBox[d][0] = point[d]
				elif point[d] > minBBox[d][1]: # max val
					minBBox[d][1] = point[d]
		# Remove all points which have a coordinate at its respective boundary
		i = 0
		while i < len(dataset):
			atBoundary = False
			point = dataset[i]
			for d in range(numDimensions):
				if point[d] == minBBox[d][0] or point[d] == minBBox[d][1]:
					atBoundary = True
					break
			if atBoundary:
				del dataset[i]
			else:
				i += 1
		# Output new dataset
		print("{} {}".format(numDimensions, len(dataset)))
		for point in dataset:
			print(pointToStr(point))