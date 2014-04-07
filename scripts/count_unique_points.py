import sys

if __name__ == "__main__":
	if len(sys.argv) < 2:
		sys.exit("python {} <datasetFilename>".format(sys.argv[0]))
	datasetFilename = sys.argv[1]

	points = []

	duplicates = 0
	total = 0

	with open(datasetFilename, "r") as f:
		f.readline() # ignore first line
		for line in f.readlines():
			total += 1
			if line in points:
				duplicates += 1
			points.append(line)

	print("Unique points: {}\nDuplicate points: {}\nTotal points: {}".format(total - duplicates, duplicates, total))