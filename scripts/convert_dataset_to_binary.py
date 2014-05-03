import sys
import os
import struct
from generate_dataset_operations import loadDataset

def readNumberOfDimensions(filename):
	with open(filename, "r") as f:
		headerFields = f.readline()[:-1].split()
		if len(headerFields) < 2:
			sys.exit("Dataset header file does not have both #dimensions and #points")	
		return int(headerFields[0])

def computeBoundary(filename):
	with open(filename, "r") as f:
		headerFields = f.readline()[:-1].split()
		if len(headerFields) < 2:
			sys.exit("Dataset header file does not have both #dimensions and #points")	
		numDimensions, numDatasetPoints = int(headerFields[0]), int(headerFields[1])
		if numDatasetPoints <= 0:
			return []

		# Compute minimum bounding box to find boundary values
		# Use first point to initialise boundary
		firstPoint = [ float(x) for x in f.readline()[:-1].split() ]
		minBBox = [ [ firstPoint[d], firstPoint[d] ] for d in range(numDimensions) ]
		# Read remaining points to get entire boundary
		for line in f.readlines():
			point = [ float(x) for x in line[:-1].split() ]
			for d in range(numDimensions):
				if point[d] < minBBox[d][0]: # min val
					minBBox[d][0] = point[d]
				elif point[d] > minBBox[d][1]: # max val
					minBBox[d][1] = point[d]
		return minBBox

def writeBinaryFile(outputFilename, datasetFilename, boundary, dimension):
	with open(datasetFilename, "r") as inFile:
		with open(outputFilename, "wb") as outFile:
			# Skip header field
			inFile.readline()
			# Read remaining lines as points
			for line in inFile.readlines():
				point = [ float(x) for x in line[:-1].split() ]
				# Normalise the point's value for the chosen dimension into
				# a 0-255 range so it can fit into a single byte
				normalisedVal = (point[dimension] - boundary[dimension][0]) / (boundary[dimension][1] - boundary[dimension][0]) # 0-1 range
				byteRepresentation = int(normalisedVal * 255) # 0-255 range
				outFile.write( struct.pack("B", byteRepresentation) )

if __name__ == "__main__":
	# Parse command line arguments
	if len(sys.argv) < 2:
		sys.exit("python {} <datasetFilename>".format(sys.argv[0]))
	datasetFilename = sys.argv[1]

	# Compute boundary of file using initial pass through file
	numDimensions = readNumberOfDimensions(datasetFilename)
	boundary = computeBoundary(datasetFilename)
	# Make another pass through entire file for each dimension
	# WE DON'T WANT TO STORE THE DATASET IN MEMORY IN CASE IT'S VERY LARGE!
	for i in range(numDimensions):
		baseFilename = os.path.splitext(datasetFilename)[0]
		outputFilename = "{}_{}.bin".format(baseFilename, i)
		writeBinaryFile(outputFilename, datasetFilename, boundary, i)