import sys

if __name__ == "__main__":
	# Parse command line arguments
	if len(sys.argv) < 2:
		sys.exit("python {} <datasetFilename> {{<maxPoints>}}".format(sys.argv[0]))
	datasetFilename = sys.argv[1]
	if len(sys.argv) >= 3:
		maxPoints = int(sys.argv[2])
	else:
		maxPoints = None

	# Perform initial pass through file to determine line count (i.e. # of points)
	lineCount = 0
	with open(datasetFilename, "r") as f:
		line = f.readline()
		while line:
			lineCount += 1
			line = f.readline()
	# Read first line and use to make assumption about the dimensionality of each point
	numDimensions = 0
	with open(datasetFilename, "r") as f:
		firstLine = f.readline()
		numDimensions = len(firstLine.split())
	# If dimensionality of dataset is 0, print error message and exit
	if numDimensions == 0:
		sys.exit("Could not determine dimensionality of dataset")

	# Print initial header at END of file (so we have number of points already)
	if maxPoints:
		numPoints = min(lineCount, maxPoints)
	else:
		numPoints = lineCount
	print("{} {}".format(numDimensions, numPoints))
	# Output dataset header which defines dimensionality of data and number of points
	# Read entire file line-by-line, printing out each line as a point
	with open(datasetFilename, "r") as f:
		pointsRead = 0
		line = f.readline()
		while line:
			fields = line.split()
			floatFields = [ str(float(x)) for x in fields ]
			print(" ".join(floatFields))
			# Stop reading file is maximum number of points have been read
			pointsRead += 1
			if maxPoints and pointsRead >= maxPoints:
				break
			# Read next line of file
			line = f.readline()