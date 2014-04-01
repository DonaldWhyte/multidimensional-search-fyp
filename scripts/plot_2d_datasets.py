import sys
import matplotlib.pyplot as plt
from matplotlib.backends.backend_pdf import PdfPages
from numpy.random import random

def extract2DCoordinates(points):
	return [ p[0] for p in points ], [ p[1] for p in points ]

def plotScatter(plot, points, colour, marker):
	xValues, yValues = extract2DCoordinates(points)
	return plot.scatter(xValues, yValues, color=colour, marker=marker)

def plotLine(plot, points, colour, marker):
	xValues, yValues = extract2DCoordinates(points)
	return plot.plot(xValues, yValues, color=colour, marker=marker)

class Dataset:

	def __init__(self, name, type, points, colour, marker):
		self.name = name
		self.type = type
		self.points = points
		self.colour = colour
		self.marker = marker

	def plot(self, plot):
		if self.type == "scatter":
			return plotScatter(plot, self.points, self.colour, self.marker)
		elif self.type == "line":
			return plotLine(plot, self.points, self.colour, self.marker)

def readPoints(filename):
	points = []
	with open(filename, "r") as f:
		for line in f.readlines():
			points.append( line.split() )
	return points


NUM_MANDATORY_ARGS = 6
COLOURS = [ 'b', 'c', 'y', 'm', 'r' ]
MARKERS = [ 'o', 'v', 's', 'D', 'x' ]
#MARKERS = [ " " for i in range(20 )]

if __name__ == "__main__":
	# Parse command line arguments
	if len(sys.argv) < NUM_MANDATORY_ARGS:
		sys.exit("python %s <outputFile> <xAxisLabel> <yAxisLabel> <dataset1Name> <dataset1Filename> [<dataset2Name> <dataset2Filename> ...]" % sys.argv[0])
	outputFilename = sys.argv[1]
	xAxisLabel = sys.argv[2]
	yAxisLabel = sys.argv[3]
	datasets = [ (sys.argv[4], sys.argv[5]) ]
	if len(sys.argv) > NUM_MANDATORY_ARGS:
		remainingArgs = sys.argv[NUM_MANDATORY_ARGS:]
		if len(remainingArgs) % 2 == 0: # if the arguments come in pairs
			index = 0
			while index < len(remainingArgs):
				datasets.append( (remainingArgs[index], remainingArgs[index + 1]) )
				index += 2

	# Load dataset points
	datasetPoints = []
	for ds in datasets:
		datasetPoints.append( readPoints(ds[1]) )
	# Plot datasets
	datasetPlots = []
	for i in range(len(datasets)):
		datasetPlots.append(Dataset(
			datasets[i][0], "line", datasetPoints[i],
			COLOURS[i % len(COLOURS)], MARKERS[i % len(MARKERS)]
		))
	datasetNames = []
	plotObjects = []
	for ds in datasetPlots:
		datasetNames.append(ds.name)
		plotObjects.append( ds.plot(plt) )
	plt.legend(plotObjects, datasetNames, "upper right", ncol=1, prop={'size':12})
	plt.xlabel(xAxisLabel)
	plt.ylabel(yAxisLabel)
	# Save plot to output file
	pp = PdfPages(outputFilename)
	plt.savefig(pp, format="pdf")
	pp.close()