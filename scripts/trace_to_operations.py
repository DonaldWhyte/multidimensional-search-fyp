"""Convert D. J. Duke's application traces of operation lists
that can be used by the evaluation framework."""

import sys

def getLineCount(filename):
	count = 0
	with open(traceFilename) as f:
		line = f.readline()
		while line:
			count += 1
			line = f.readline()	
	return count

def getDimensions(filename):
	with open(traceFilename) as f:
		# Find first non-INIT line and use it determine dimensionality
		line = f.readline()
		while line:
			fields = line[:-1].split()
			if fields[0] != "INIT":
				return len(fields)
			line = f.readline()
	return 0

if __name__ == "__main__":
	# Parse command line arguments
	if len(sys.argv) < 2:
		sys.exit("python {} <traceFilename>".format(sys.argv[0]))
	traceFilename = sys.argv[1]

	# First count the number of operations there are in the trace
	# and compute the dimensionality
	numOperations = getLineCount(traceFilename)
	numDimensions = getDimensions(traceFilename)
	if numDimensions == 0:
		sys.exit("Could not determine dimensionality of dataset")
	# Output header information
	print("{} {}".format(numDimensions, numOperations))
	# Go through each ind
	with open(traceFilename) as f:
		line = f.readline()
		while line:
			fields = line[:-1].split() # [:-1] to remove trailing newline
			if fields[0] == "INIT":
				# Parse boundaries and store them in list of tuples
				fields = fields[1:]
				if len(fields) % 2 == 0:
					boundaries = []
					for i in range(0, len(fields), 2):
						boundaries.append( (fields[i], fields[i + 1][:-1]) ) # [:-1] on MAX value to remove ';'
					boundaryStrings = [ ",".join(interval) for interval in boundaries]
					allBoundaryString = " ".join(boundaryStrings)
					print("C {}".format(allBoundaryString))
				else:
					sys.exit("Invalid number of arguments for reset boundary intervals")
			else:
				point = " ".join(fields)
				print("I {}".format(point))

			line = f.readline()