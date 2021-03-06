import sys
import glob
from timing_file_parser import parseTimingFile
from maps import *

import matplotlib.pyplot as plt
import numpy as np
from matplotlib.backends.backend_pdf import PdfPages

def barPlot(outputFilename, groupData, groupLabels, barLabels, plot, maxYValue = None, legend = True):
	# Number of GROUPS is size of inner list
	numGroups = len(groupData[0])
	# This needs to be a numpy range for xdata calculations to work
	ind = np.arange(numGroups)
	# Bar graphs expect a total width of "1.0" per group
	# Thus, you should make the sum of the two margins
	# plus the sum of the width for each entry equal 1.0.
	# One way of doing that is shown below. You can make
	# The margins smaller if they're still too big.
	margin = 0.3
	width = (1. - 2. * margin) / numGroups

	# Create a subplot
	s = plot.subplot(1,1,1)
	plotObjects = []
	for num, vals in enumerate(groupData):
	    # The position of the xdata must be calculated for each of the two data series
	    xdata = ind + margin + (num * width)
	    barRects = plot.bar(xdata, vals, width, color=COLOURS[num % len(COLOURS)], hatch=PATTERNS[num % len(PATTERNS)])
	    plotObjects.append(barRects[0])

	if legend:
		plot.legend(plotObjects, barLabels, loc=2)
	plot.xlabel("Structure")
	plot.ylabel("Execution Time (in seconds)")
	if maxYValue:
		plot.ylim([0, maxYValue])

	# You should no longer need to manually set the plot limit since everything 
	# is scaled to one.
	# Also the ticks should be much simpler now that each group of bars extends from
	# 0.0 to 1.0, 1.0 to 2.0, and so forth and, thus, are centered at 0.5, 1.5, etc.
	s.set_xticks(ind+0.5)
	s.set_xticklabels(groupLabels)

	# Save plot to output file
	pp = PdfPages(outputFilename)
	plt.savefig(pp, format="pdf")
	pp.close()
	# Clear plot object now that it has been saved
	plt.clf()

def plotTimingData(outputFilename, timingData, maxYValue, legend):
	# Sort operation keys and structure keys for consistent output
	opNames = sorted(timingData.keys())
	structureNames = sorted(timingData[opNames[0]].keys())
	# Retrieve human-readable labels for data
	groupLabels = [ STRUCTURE_NAME_MAP[op] for op in structureNames ]
	barLabels = [ OPERATION_NAME_MAP[op] for op in opNames ]
	# TODO
	groupData = []
	for op in opNames:
		opData = []
		for struct in structureNames:
			times = timingData[op][struct]
			if len(times) > 0: # safety check
				opData.append( timingData[op][struct][0][1] )
		groupData.append(opData)
	# Plot the bar chart
	barPlot(outputFilename, groupData, groupLabels, barLabels, plt, maxYValue=maxYValue, legend=legend)

if __name__ == "__main__":
	# Parse command line options
	if len(sys.argv) < 2:
		sys.exit("python %s \"<timingFilenameGlob>\" {<maxYValue>} {--no-legend}" % sys.argv[0])
	if "--no-legend" in sys.argv:
		legend = False
		sys.argv.remove("--no-legend")
	else:
		legned = True
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
		# Ignore file if it does not contain a SINGLE set of timings for each structure
		skipFile = False
		for operation, structures in data.data.items():
			for struct, dataItems in structures.items():
				if len(dataItems) >= 2:
					skipFile = True
					break
			if skipFile:
				break
		if skipFile:
			continue

		outputFilename = "%s_times.pdf" % data.title
		plotTimingData(outputFilename, data.data, maxYValue=maxYValue, legend=legend)
