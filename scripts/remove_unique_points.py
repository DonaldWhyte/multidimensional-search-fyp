import sys

if __name__ == "__main__":
	if len(sys.argv) < 2:
		sys.exit("python {} <datasetFilename> {--count}".format(sys.argv[0]))
	datasetFilename = sys.argv[1]
	justCount = ("--count" in sys.argv)

	uniquePoints = set()
	duplicates = 0
	total = 0

	with open(datasetFilename, "r") as f:
		f.readline() # ignore first line
		for line in f.readlines():
			total += 1
			if line in uniquePoints:
				duplicates += 1
			else:
				uniquePoints.add(line)

	if justCount:
		print("Unique points: {}\nDuplicate points: {}\nTotal points: {}".format(total - duplicates, duplicates, total))
	else:
		# Take ANY point from unique points and determine dimensionality from it
		numDimensions = 0
		for point in uniquePoints:
			numDimensions = len(point.split())
			break
		# Print out dataset header and points
		print("{} {}".format(numDimensions, len(uniquePoints)))
		for point in uniquePoints:
			print(point[:-1]) # NOTE: [:-1] to remove newline