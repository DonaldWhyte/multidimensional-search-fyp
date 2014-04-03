import sys
import random
import time
import matplotlib.pyplot as plt
from matplotlib.backends.backend_pdf import PdfPages

from generate_dataset_operations import loadDataset
from plot_2d_datasets import extract2DCoordinates, COLOURS, MARKERS

def plotScatter(plot, points, colour, marker):
	xValues, yValues = extract2DCoordinates(points)
	return plot.scatter(xValues, yValues, color=colour, marker=marker)

def removeAtRandom(dataset, numPointsToRemove):
	for i in range(numPointsToRemove):
		index = random.randint(0, len(dataset) - 1)
		del dataset[index]

NUM_MANDATORY_ARGS = 6

if __name__ == "__main__":
	random.seed(time.time()) # seed random number generator

	# Parse command line arguments
	if "--random-pick" in sys.argv:
		randomPick = True
		sys.argv.remove("--random-pick")
	else:
		randomPick = False

	if len(sys.argv) < NUM_MANDATORY_ARGS:
		sys.exit("python %s <outputFilename> <xAxisLabel> <yAxisLabel> <maxPoints> <dataset1> [<dataset2> ... <datasetN>] {--random-pick}" % sys.argv[0])
	outputFilename = sys.argv[1]
	xAxisLabel = sys.argv[2]
	yAxisLabel = sys.argv[3]
	maxPoints = int(sys.argv[4])
	datasetFilenames = sys.argv[5:]

	# Load datasets into memort
	datasets = []
	for filename in datasetFilenames:
		numDimensions, numPoints, dataset = loadDataset(filename)
		if maxPoints >= 0: # enforce max number of points
			if randomPick:
				removeAtRandom(dataset, len(dataset) - maxPoints)
			else:
				dataset = dataset[:maxPoints]
		datasets.append(dataset)

	# Create scatter plots for each dataset
	plotObjects = []
	for i in range(len(datasets)):
		plotObjects.append(plotScatter(
			plt, datasets[i], COLOURS[i % len(COLOURS)], MARKERS[i % len(MARKERS)]
		))
	# Set labels
	plt.xlabel(xAxisLabel)
	plt.ylabel(yAxisLabel)
	plt.axis([0, 1, 0, 1])
	# Save plot to output file
	pp = PdfPages(outputFilename)
	plt.savefig(pp, format="pdf")
	pp.close()