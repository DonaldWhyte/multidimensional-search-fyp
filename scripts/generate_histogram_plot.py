import sys
import glob

import matplotlib.pyplot as plt
from matplotlib.backends.backend_pdf import PdfPages
from numpy.random import normal

class Histogram:

	DEFAULT_NUM_BINS = 100

	def __init__(self, title, numBins, data):
		self.title = title
		self.numBins = numBins
		self.dataColumns = data

	def plotColumnToFile(self, filename, colNumber, plt, title=True):
		# Plot the histogram, setting the labels appropriately
		plt.hist(self.dataColumns[colNumber], bins=self.numBins)
		if title:
			plt.title(self.title)
		else:
			plt.title("")
		plt.xlabel("Dimension %d" % (colNumber + 1))
		plt.ylabel("Frequency")
		# Save plot to output file
		pp = PdfPages(filename)
		plt.savefig(pp, format="pdf")
		pp.close()
		# Clear histogram from plot object now that it has been saved
		plt.clf()

def parseHistogramFile(filename):
	with open(filename, "r") as f:
		title = f.readline()
		numBins = 0
		try:
			numBinStr = f.readline()
			numBins = int(numBinStr)
			if numBins <= 0:
				raise ValueError
		except ValueError:
			raise RuntimeError("Number of bins is not a positive integer, got '%s' instead" % n)
		# Now parse the rest of the lines as data, where field X of a
		# line is a data item for column X
		line = f.readline()
		fields = line[:-1].split()
		data = [ [ float(col) ] for col in fields ] 
		columnRanges = [ [data[i][0], data[i][1]] for i in range(len(data)) ]
		# Use first line to get number of coloumns
		for line in f.readlines():
			fields = line[:-1].split()
			for i in range(len(fields)):
				val = float(fields[i])
				data[i].append(val)
				if val < columnRanges[i][0]:
					columnRanges[i][0] = val
				elif val > columnRanges[i][1]:
					columnRanges[i][1] = val

	return Histogram(title, xlLabel, numBins, data)

def parseDataset(filename):
	with open(filename, "r") as f:
		# Get number of dimensions and points
		fields = f.readline()[:-1].split()
		data = [ [ ] for i in range(int(fields[0])) ] 
		#columnRanges = [ [  ] for ]
		for line in f.readlines():
			fields = line[:-1].split()
			for i in range(len(fields)):
				data[i].append( float(fields[i]) )

	return Histogram(filename, Histogram.DEFAULT_NUM_BINS, data)			

if __name__ == "__main__":
	# Parse command line options
	if len(sys.argv) < 2:
		sys.exit("python %s \"<timingFilenameGlob>\" {--no-title}" % sys.argv[0])
	if "--no-title" in sys.argv:
		title = False
		sys.argv.remove("--no-title")
	else:
		title = False
	timingFilenameGlob = sys.argv[1]

	# Use glob to find all desired files
	filenames = glob.glob(timingFilenameGlob)
	# Process each histogram file in turn
	for fname in filenames:
		# Parse as dataset if extension is ".dat" instead
		if fname.endswith(".dat"):
			data = parseDataset(fname)
		else:
			data = parseHistogramFile(fname)
		for i in range(len(data.dataColumns)):
			outputFilename = "%s_%d.pdf" % (data.title, i)
			print(outputFilename)
			data.plotColumnToFile(outputFilename, i, plt, title=title)