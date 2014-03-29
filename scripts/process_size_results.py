#!/export/mailgrp3_g/sc10dw/python3.4/bin/python3.4

import sys
import os
import re
from process_timing_results import writeTimings

TIMING_FILENAME_FORMAT = r"times_{}_sizevary_n(\d+)\.txt"
STRUCTURE_TIMING_LINE_REGEX = re.compile( r"\t\tStructure \((\d+)\): ([\d\.]+(e[+-]{1}\d+)?) seconds" )
OUTPUT_STRUCTURE_TIMING_FILENAME_FORMAT = "{}_{}_sizevary.times"

def getFilesToProcess(resultsDirectory, operation):
	filenameFormatRegex = re.compile( TIMING_FILENAME_FORMAT.format(operation) )
	filesToProcess = []
	for filename in os.listdir(resultsDirectory):
		fullName = os.path.join(resultsDirectory, filename)
		if os.path.isfile(fullName):
			match = filenameFormatRegex.match(filename)
			if match:
				n = match.group(1)
				filesToProcess.append( (int(n), fullName ) )
	return filesToProcess

def getStructureTimings(timingFiles, structureNames, testOpListIndex = 0):
	structureTimings = {}
	for name in structureNames:
		structureTimings[name] = []

	for n, filePath in timingFiles:
		with open(filePath, "r") as f:
			# Scan through file until required test operation list index is found
			testOpListToRead = "Test Operations ({}):".format(testOpListIndex)
			line = ""
			while line != testOpListToRead:
				line = f.readline()[:-1] # [:-1] there to remove trailing newline
			# Process remaining lines as timings
			for line in f.readlines():
				match = STRUCTURE_TIMING_LINE_REGEX.match(line)
				if match:
					index = int(match.group(1))
					timingValue = match.group(2)
					structureTimings[structureNames[index]].append( (n, timingValue) )
				else:
					break
	# Sort structure timing lists by dimensionality
	for structureName, timings in structureTimings.items():
		timings.sort(key=lambda x : x[0])
		if len(timings) > 0 and timings[0][0] != 0:
			timings = [(0, 0)] + timings # if not added, add zero-value for 0 on x-axis
	return structureTimings

if __name__ == "__main__":
	# Parse command line arguments
	if len(sys.argv) < 4:
		sys.exit("Usage: python {} <outputDirectory> <timingsDirectory> <operation> [<structure1Name> <structure2Name> ... <structureNName>]".format(sys.argv[0]))
	outputDirectory = sys.argv[1]
	resultsDirectory = sys.argv[2]
	operation = sys.argv[3]
	structureNames = sys.argv[4:]

	structureTimings = getStructureTimings(
		getFilesToProcess(resultsDirectory, operation),
		structureNames
	)

	for structureName, timings in structureTimings.items():
		filename = OUTPUT_STRUCTURE_TIMING_FILENAME_FORMAT.format(structureName, operation)
		outputPath = os.path.join(outputDirectory, filename)
		writeTimings(outputPath, timings)
