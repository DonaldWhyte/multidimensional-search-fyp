import sys
import glob

import matplotlib.pyplot as plt
from matplotlib.backends.backend_pdf import PdfPages
import numpy
from numpy.random import normal

class Histogram:

	DEFAULT_NUM_BINS = 100

	def __init__(self, title, data, minVals, maxVals):
		self.title = title
		self.dataColumns = data
		self.minVals = minVals
		self.maxVals = maxVals

	def plotColumnToFile(self, filename, colNumber, plt, title=True, numBins=None, minVal=None, maxVal=None):
		if not numBins:
			numBins = Histogram.DEFAULT_NUM_BINS
		useCustomRange = ((minVal != None) and (maxVal != None))

		values = self.dataColumns[colNumber]
		minVal, maxVal = self.minVals[colNumber], self.maxVals[colNumber]
		# Plot the histogram, setting the labels appropriately
		if useCustomRange:
			plt.hist(values, bins=numBins, range=[minVal, maxVal])
		else:
			plt.hist(values, bins=numBins, range=[minVal, maxVal],
				weights=numpy.zeros_like(values) + 1. / len(values))
		if title:
			plt.title(self.title)
		else:
			plt.title("")
		plt.xlabel("Dimension %d" % (colNumber + 1))
		plt.ylabel("Relative Frequency")
		yMin, yMax = plt.ylim()
		plt.axis([self.minVals[colNumber], self.maxVals[colNumber],yMin,yMax])
		# Save plot to output file
		pp = PdfPages(filename)
		plt.savefig(pp, format="pdf")
		pp.close()
		# Clear histogram from plot object now that it has been saved
		plt.clf()

def parseHistogramFile(filename):
	with open(filename, "r") as f:
		title = f.readline()
		# Now parse the rest of the lines as data, where field X of a
		# line is a data item for column X
		line = f.readline()
		fields = line[:-1].split()
		data = [ [ float(col) ] for col in fields ] 
		minVals = [ data[i][0] for i in range(len(data)) ]
		maxVals = [ data[i][0] for i in range(len(data)) ]
		# Use first line to get number of columns
		for line in f.readlines():
			point = [ float(x) for x in line[:-1].split() ]
			# Add to each data column separately
			for i in range(len(point)):
				val = float(point[i])
				data[i].append(val)	
			# Update min/max values		
			for d in range(len(data)):
				minVals[d] = min(point[d], minVals[d])
				maxVals[d] = max(point[d], maxVals[d])

	return Histogram(title, data, minVal, maxVal)

def parseDataset(filename):
	with open(filename, "r") as f:
		# Get number of dimensions and points
		fields = f.readline()[:-1].split()
		data = [ [ ] for i in range(int(fields[0])) ] 
		# Read first point to initialise min and max vals
		line = f.readline()
		point = [ float(x) for x in line[:-1].split() ]
		minVals = list(point)
		maxVals = list(point)

		for line in f.readlines():
			point = [ float(x) for x in line[:-1].split() ]
			for d in range(len(data)): # check if any are minimum and maximum
				minVals[d] = min(point[d], minVals[d])
				maxVals[d] = max(point[d], maxVals[d])
			for i in range(len(point)):
				data[i].append(point[i])

		print minVals
		print maxVals
		return Histogram(filename, data, minVals, maxVals)

if __name__ == "__main__":
	# Parse command line options
	if "--no-title" in sys.argv:
		title = False
		sys.argv.remove("--no-title")
	else:
		title = False
	if len(sys.argv) < 3:
		sys.exit("python %s \"<datasetFilenameGlob>\" <numBins> {<minVal> <maxVal>} {--no-title}" % sys.argv[0])
	datasetFilenameGlob = sys.argv[1]
	numBins = int(sys.argv[2])
	if len(sys.argv) >= 4:
		minVal = float(sys.argv[3])
	else:
		minVal = None
	if len(sys.argv) >= 5:
		maxVal = float(sys.argv[4])
	else:
		maxVal = None

	# Use glob to find all desired files
	filenames = glob.glob(datasetFilenameGlob)
	# Process each histogram file in turn
	for fname in filenames:
		# Parse as dataset if extension is ".dat" instead
		if fname.endswith(".dat"):
			data = parseDataset(fname)
		else:
			data = parseHistogramFile(fname)
		for i in range(len(data.dataColumns)):
			outputFilename = "%s_%d.pdf" % (data.title, i)
			print(outputFilename, numBins, minVal, maxVal)
			data.plotColumnToFile(outputFilename, i, plt, title=title,
				numBins=numBins, minVal=minVal, maxVal=maxVal)