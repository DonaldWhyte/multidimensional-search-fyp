import sys
import glob
from timing_file_parser import parseTimingFile
from maps import *

import matplotlib.pyplot as plt
from matplotlib.backends.backend_pdf import PdfPages

COLOURS = [ 'b', 'c', 'y', 'm', 'r' ]
MARKERS = [ 'o', 'v', 's', 'D', 'x' ]

def extract2DCoordinates(points):
	return [ p[0] for p in points ], [ p[1] for p in points ]

class Dataset:

	def __init__(self, name, type, points, colour, marker):
		self.name = name
		self.type = type # NOTE: unused for now
		self.points = [ (0, 0) ] + points
		self.colour = colour
		self.marker = marker

	def plot(self, plot):
		xValues, yValues = extract2DCoordinates(self.points)
		return plot.plot(xValues, yValues, color=self.colour, marker=self.marker)

	def maxXValue(self):
		return self.maxCoord(0)

	def maxYValue(self):
		return self.maxCoord(1)

	def maxCoord(self, dimension):
		if len(self.points) > 0:
			return max([ p[dimension] for p in self.points ])
		else:
			return 0

def generatePlot(filename, xAxisLabel, yAxisLabel, operation, structures, imposedMaxYValue = None):
	# Maintain sorted list of structures and generate dataset object
	# for each structure
	structureNames = sorted(structures.keys())
	datasets = []
	for i in range(len(structureNames)):
		# Convert timings into 2D points
		datasets.append(Dataset(
			structureNames[i], "line", structures[structureNames[i]],
			COLOURS[i % len(COLOURS)], MARKERS[i % len(MARKERS)]
		))
	# Determine maximum X value across all datasets
	maxXValue = max([ ds.maxXValue() for ds in datasets ])
	maxYValue = max([ ds.maxYValue() for ds in datasets ])
	# Plot each dataset object
	plotObjects = []
	for ds in datasets:
		plotObjects.append( ds.plot(plt) )
	# Configure plot
	# Make sure to replace 
	presentStructureNames = [ STRUCTURE_NAME_MAP[name] for name in structureNames ]
	plt.legend(plotObjects, presentStructureNames, "upper right", ncol=1, prop={'size':12})
	plt.xlabel(xAxisLabel)
	plt.ylabel(yAxisLabel)
	if maxYValue:
		if maxYValue > imposedMaxYValue:
			plt.ylim([0, imposedMaxYValue])
	# Save plot to output file
	pp = PdfPages(filename)
	plt.savefig(pp, format="pdf")
	pp.close()

	plt.clf()


if __name__ == "__main__":
	# Parse command line options
	if len(sys.argv) < 2:
		sys.exit("python %s \"<timingFilenameGlob>\" <maxYValue>" % sys.argv[0])
	timingFilenameGlob = sys.argv[1]
	if len(sys.argv) >= 3:
		maxYValue = float(sys.argv[2])
	else:
		maxYValue = None
	# Use glob to find all desired files
	filenames = glob.glob(timingFilenameGlob)
	# Process each timing file in turn
	for fname in filenames:
		# Retrieve contents of timing file
		data = parseTimingFile(fname, True)
		# Ignore file if it does not contain at least two data items
		# for AT LEAST ONE op
		skipFile = True
		for operation, structures in data.data.items():
			for struct, dataItems in structures.items():
				if len(dataItems) >= 2:
					skipFile = False
					break
			if not skipFile:
				break
		if skipFile:
			continue

		# Generate a plot for each operation
		for operation, structures in data.data.items():
			outputFilename = "%s_%s.pdf" % (data.title, operation)
			generatePlot(outputFilename, data.xAxis, data.yAxis, operation, structures, maxYValue)