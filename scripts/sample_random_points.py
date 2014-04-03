import sys
import random
import time
from generate_dataset_operations import loadDataset
from generate_random_dataset import pointToStr

def generateRandomIndices(numPoints, pointsToSample):
	# Just in case points to sample is larger than available points
	pointsToSample = min(pointsToSample, numPoints)

	allIndices = list(range(numPoints))
	chosenIndices = []
	for i in range(pointsToSample):
		# Generate random index into dataset index list
		# and store dataset index
		index = random.randint(0, len(allIndices) - 1)
		chosenIndices.append(allIndices[index])
		# Remove selected index  so it won't be chosen again
		del allIndices[index]
	chosenIndices.sort()
	return chosenIndices

def sampleDataset(datasetFilename, pointsToSample):
	with open(datasetFilename, "r") as f:
		# Read header line to get dimensions and number of points
		line = f.readline()
		fields = line.split()
		if len(fields) < 2:
			raise RuntimeError("Invalid file header")
		numDimensions = int(fields[0])
		numPoints = int(fields[1])
		# Generate random sorted list of point indices which indicate
		# which points to sample
		indices = generateRandomIndices(numPoints, pointsToSample)

		# Output new header file
		print("{} {}".format(numDimensions, len(indices)))
		# Now go through each line, outputting points to sample
		currentLine = 0
		nextIndexToSample = indices.pop(0)
		line = f.readline()
		while line != "":
			if currentLine == nextIndexToSample:
				# Parse all fields of into floats and output new format
				point = [float(x) for x in line[:-1].split()] # [:-1] to remove trailing newline
				print(pointToStr(point))
				if len(indices) == 0: # no more points to sample, break the loop!
					break				
				else:
					nextIndexToSample = indices.pop(0) # otherwise, get next index to sample
			
			line = f.readline()
			currentLine += 1

if __name__ == "__main__":
	random.seed(time.time())

	# Parse command line arguments
	if len(sys.argv) < 3:
		sys.exit("python {} <datasetFilename> <pointsToSample>".format(sys.argv[0]))
	datasetFilename = sys.argv[1]
	pointsToSample = int(sys.argv[2])

	sampleDataset(datasetFilename, pointsToSample)