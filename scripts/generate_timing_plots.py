import sys
import glob
from timing_file_parser import parseTimingFile

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
		self.points = points
		self.colour = colour
		self.marker = marker

	def plot(self, plot):
		xValues, yValues = extract2DCoordinates(self.points)
		return plot.plot(xValues, yValues, color=self.colour, marker=self.marker)

def generatePlot(filename, xAxisLabel, yAxisLabel, operation, structures):
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
	# Plot each dataset object
	plotObjects = []
	for ds in datasets:
		plotObjects.append( ds.plot(plt) )
	# Configure plot
	plt.legend(plotObjects, structureNames, "upper right", ncol=1, prop={'size':12})
	plt.xlabel(xAxisLabel)
	plt.ylabel(yAxisLabel)
	# Save plot to output file
	pp = PdfPages(filename)
	plt.savefig(pp, format="pdf")
	pp.close()

	plt.clf()


if __name__ == "__main__":
	# Parse command line options
	if len(sys.argv) < 2:
		sys.exit("python %s \"<timingFilenameGlob>\"" % sys.argv[0])
	timingFilenameGlob = sys.argv[1]
	# Use glob to find all desired files
	filenames = glob.glob(timingFilenameGlob)
	# Process each timing file in turn
	for fname in filenames:
		# Retrieve contents of timing file
		data = parseTimingFile(fname)
		# Generate a plot for each operation
		for operation, structures in data.data.items():
			outputFilename = "%s_%s.pdf" % (data.title, operation)
			generatePlot(outputFilename, data.xAxis, data.yAxis, operation, structures)