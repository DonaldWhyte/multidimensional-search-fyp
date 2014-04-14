"""Remove certain dimensions from an operation list."""

import sys

if __name__ == "__main__":
	# Parse command line arguments
	if len(sys.argv) < 2:
		sys.exit("python {} <opListFilename> [<dimIndex1> <dimIndex2> ... <dimIndexN>]".format(sys.argv[0]))
	opListFilename = sys.argv[1]
	dimensionsToRemove = [ int(arg) for arg in sys.argv[2:] ]
	# Remove duplicates and reverse sort dimensions by index so latest
	# dimensions are removed first
	dimensionsToRemove = list(set(dimensionsToRemove))
	dimensionsToRemove.sort(reverse=True)

	with open(opListFilename) as f:
		# Read header to get number of dimensions
		line = f.readline()
		numDimensions, numOperations = [ int(x) for x in line[:-1].split() ]
		# Ensure given dimensions are valid
		if len(dimensionsToRemove) >= numDimensions:
			sys.exit(">= all dimensions were specified to remove")
		for dim in dimensionsToRemove:
			if dim >= numDimensions:
				sys.exit("Dimension {} not present in dataset".format(dim))
		# Print out header with new amount of dimensions
		newDimensions = numDimensions - len(dimensionsToRemove)
		print("{} {}".format(newDimensions, numOperations))
		# Print out each operation again, removing the specified dimensions
		line = f.readline()
		while line:
			fields = line[:-1].split()
			opType = fields[0]
			values = fields[1:]
			# Remove specified dimensions from point (or points for Update operations)
			if opType == "U":
				for index in dimensionsToRemove: # remove points from second operand so indices remain intact
					del values[index + numDimensions]
				for index in dimensionsToRemove: # now remove dimensions from first operand
					del values[index]
			else:
				for index in dimensionsToRemove:
					del values[index]

			print("{} {}".format(opType, " ".join(values)))
			line = f.readline()