import sys
import random
import time
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

from generate_dataset_operations import loadDataset
from plot_2d_datasets import COLOURS, MARKERS

def extract3DCoordinates(points):
	return [ p[0] for p in points ], [ p[1] for p in points ], [ p[2] for p in points ]

def plotScatter(plot, points, colour, marker):
	xValues, yValues, zValues = extract3DCoordinates(points)
	return plot.scatter(xValues, yValues, zValues, color=colour, marker=marker)

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
	if "--unit-axis" in sys.argv:
		unitAxis = True
		sys.argv.remove("--unit-axis")
	else:
		unitAxis = False

	if len(sys.argv) < NUM_MANDATORY_ARGS:
		sys.exit("python %s  <xAxisLabel> <yAxisLabel> <zAxisLabel> <maxPoints> <dataset1> [<dataset2> ... <datasetN>] {--random-pick} {--unit-axis}" % sys.argv[0])
	xAxisLabel = sys.argv[1]
	yAxisLabel = sys.argv[2]
	zAxisLabel = sys.argv[3]
	maxPoints = int(sys.argv[4])
	datasetFilenames = sys.argv[5:]

	# Load datasets into memory
	datasets = []
	for filename in datasetFilenames:
		numDimensions, numPoints, dataset = loadDataset(filename)
		if maxPoints >= 0: # enforce max number of points
			if randomPick:
				removeAtRandom(dataset, len(dataset) - maxPoints)
			else:
				dataset = dataset[:maxPoints]
		datasets.append(dataset)

	# Initialise 3D axes
	fig = plt.figure(1)
	fig.clf()
	ax = Axes3D(fig)	
	# Create scatter plot for each dataset
	plotObjects = []
	for i in range(len(datasets)):
		plotObjects.append(plotScatter(
			ax, datasets[i], COLOURS[i % len(COLOURS)], MARKERS[i % len(MARKERS)]
		))
	# Set axis labels
	if unitAxis:
		ax.set_zlim3d([0, 1])
		ax.set_ylim3d([0, 1])
		ax.set_xlim3d([0, 1])
	# Display 3D plot
	plt.draw()
	plt.show()