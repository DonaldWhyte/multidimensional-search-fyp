"""Convert IEEE 2004 Visualisation Contest dataset (separated into
multiple binary files, one for each dimension) into a single
multi-dimensional file."""

import sys
import array
import os

if __name__ == "__main__":
	# Parse command line arguments
	if len(sys.argv) < 2:
		sys.exit("python {} <datasetDirectory>".format(sys.argv[0]))
	datasetDirectory = sys.argv[1]

	# Get filenames of all files with extension "bin"
	attributeFilenames = []
	for filename in os.listdir(datasetDirectory):
		fullPath = os.path.join(datasetDirectory, filename) 
		if os.path.isfile(fullPath):
			if filename.endswith(".bin"):
				attributeFilenames.append(fullPath)
	numAttributes = len(attributeFilenames)

	# Read each attribute 
	attributeData = []
	for i in range(numAttributes):
		with open(attributeFilenames[i], "rb") as f:
			valueArray = array.array("f", f.read())
			# if this is a little endian machine -- SWAP BYTES TO GET CORRECT 
			if sys.byteorder != "big":
				valueArray.byteswap()
		attributeData.append(valueArray)

	numPoints = len(attributeData[0])

	# Print header containing number of dimensions and points
	print("{} {}".format(numAttributes, numPoints))
	# Print each point as a single line
	for i in range(numPoints):
		line = ""
		for j in range(numAttributes - 1):
			line += "{} ".format(attributeData[j][i])
		line += str(attributeData[numAttributes - 1][i])
		print(line)