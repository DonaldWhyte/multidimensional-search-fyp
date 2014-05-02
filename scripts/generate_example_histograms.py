from numpy.random import standard_normal, uniform, gamma
import matplotlib.pyplot as plt
from generate_histogram_plot import *

NUM_SAMPLES_PER_DISTRIBUTION = 5000000
NUM_BINS = 50

if __name__ == "__main__":
	# Generate samples for each distribution
	distributions = [
		uniform(-6, 6, NUM_SAMPLES_PER_DISTRIBUTION),
		standard_normal(NUM_SAMPLES_PER_DISTRIBUTION),
		gamma(5.0, size=NUM_SAMPLES_PER_DISTRIBUTION)
	]
	# Place distributions in histogram objects
	histograms = [
		Histogram("Uniform Distribution", [distributions[0]]),
		Histogram("Gaussian Distribution", [distributions[1]]),
		Histogram("Gamma Distribution", [distributions[2]])
	]
	# Plot histograms to files
	for hist in histograms:
		outputFilename = "%s.pdf" % hist.title
		hist.plotColumnToFile(outputFilename, 0, plt, title=False, numBins=NUM_BINS)