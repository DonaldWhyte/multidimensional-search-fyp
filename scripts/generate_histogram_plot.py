import sys
import glob

import matplotlib.pyplot as plt
from matplotlib.backends.backend_pdf import PdfPages
from numpy.random import normal

class Histogram:

	DEFAULT_NUM_BINS = 100

	def __init__(self, title, data):
		self.title = title
		self.dataColumns = data

	def plotColumnToFile(self, filename, colNumber, plt, title=True, numBins=None, minVal=None, maxVal=None):
		if not numBins:
			numBins = Histogram.DEFAULT_NUM_BINS
		useCustomRange = ((minVal != None) and (maxVal != None))
		# Plot the histogram, setting the labels appropriately
		if useCustomRange:
			plt.hist(self.dataColumns[colNumber], bins=numBins, range=[minVal, maxVal])
		else:
			plt.hist(self.dataColumns[colNumber], bins=numBins)
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
		# Now parse the rest of the lines as data, where field X of a
		# line is a data item for column X
		line = f.readline()
		fields = line[:-1].split()
		data = [ [ float(col) ] for col in fields ] 
		# Use first line to get number of coloumns
		for line in f.readlines():
			fields = line[:-1].split()
			for i in range(len(fields)):
				val = float(fields[i])
				data[i].append(val)

	return Histogram(title, data)

def parseDataset(filename):
	with open(filename, "r") as f:
		# Get number of dimensions and points
		fields = f.readline()[:-1].split()
		data = [ [ ] for i in range(int(fields[0])) ] 
		for line in f.readlines():
			fields = line[:-1].split()
			for i in range(len(fields)):
				data[i].append( float(fields[i]) )

	return Histogram(filename, data)			

if __name__ == "__main__":
	# Parse command line options
	if "--no-title" in sys.argv:
		title = False
		sys.argv.remove("--no-title")
	else:
		title = False
	if len(sys.argv) < 3:
		sys.exit("python %s \"<timingFilenameGlob>\" <numBins> {<minVal> <maxVal>} {--no-title}" % sys.argv[0])
	timingFilenameGlob = sys.argv[1]
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
			print(outputFilename, numBins, minVal, maxVal)
			data.plotColumnToFile(outputFilename, i, plt, title=title,
				numBins=numBins, minVal=minVal, maxVal=maxVal)