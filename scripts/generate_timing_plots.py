import sys
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
		print self.name, self.points
		return plot.plot(xValues, yValues, color=self.colour, marker=self.marker)

def generatePlot(filename, xAxisLabel, yAxisLabel, operation, structures):
	# Maintain sorted listgof structures and generate dataset object
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
		sys.exit("python %s <timingFilename>" % sys.argv[0])
	timingFilename = sys.argv[1]
	# Retrieve contents of timing file
	data = parseTimingFile(timingFilename)
	# Generate a plot for each operation
	for operation, structures in data.data.items():
		filename = "%s_%s.pdf" % (data.title, operation)
		generatePlot(filename, data.xAxis, data.yAxis, operation, structures)