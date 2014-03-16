import sys

NUM_X_VALUES = 600
NUM_Y_VALUES=  248
NUM_Z_VALUES=  248

def skipLines(f, numLines):
	for i in range(numLines):
		f.readline()

if __name__ == "__main__":
	# Parse command line arguments
	if len(sys.argv) < 3:
		sys.exit("python {} <datasetFilename> <sampleRate>".format(sys.argv[0]))
	datasetFilename = sys.argv[1]
	sampleRate = int(sys.argv[2])

	# Open the dataset file
	with open(datasetFilename, "r") as f:
		sampleX = int(NUM_X_VALUES / sampleRate)
		sampleY = int(NUM_Y_VALUES / sampleRate)
		sampleZ = int(NUM_Z_VALUES / sampleRate)
		for z in range(sampleZ):
			for y in range(sampleY):
				for x in range(sampleX):
					# Sample next point by outputting next line of file
					print(f.readline()[:-1])
					skipLines(f, (sampleRate - 1))
				skipLines(f, ((sampleRate - 1) * NUM_X_VALUES))
			skipLines(f, ((sampleRate - 1) * NUM_X_VALUES * NUM_Y_VALUES))