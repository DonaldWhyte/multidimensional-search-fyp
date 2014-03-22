import sys
import os
import re

TIMING_FILENAME_FORMAT = r"times_{}_{}_(\d+)d\.txt"
INSERT_TIMING_FILENAME_REGEX = re.compile( r"times_insert_(\d+)d\.txt" )
STRUCTURE_TIMING_LINE_REGEX = re.compile( r"\t\tStructure \((\d+)\): ([\d\.]+) seconds" )
DATASET_TEST_OP_INDICES = {
	"randuniform" : 0,
	"skewed" : 1,
	"clustered" : 2
}

def getInsertFilesToProcess(resultsDirectory):
	filesToProcess = []
	for filename in os.listdir(resultsDirectory):
		fullName = os.path.join(resultsDirectory, filename)
		if os.path.isfile(fullName):
			match = INSERT_TIMING_FILENAME_REGEX.match(filename)
			if match:
				dimensionality = match.group(1)
				filesToProcess.append( (int(dimensionality), fullName ) )
	return filesToProcess	

def getFilesToProcess(resultsDirectory, operation, dataset):
	filenameFormatRegex = re.compile( TIMING_FILENAME_FORMAT.format(operation, dataset) )
	filesToProcess = []
	for filename in os.listdir(resultsDirectory):
		fullName = os.path.join(resultsDirectory, filename)
		if os.path.isfile(fullName):
			match = filenameFormatRegex.match(filename)
			if match:
				dimensionality = match.group(1)
				filesToProcess.append( (int(dimensionality), fullName ) )
	return filesToProcess

def getStructureTimings(timingFiles, structureNames, testOpListIndex = 0):
	structureTimings = {}
	for name in structureNames:
		structureTimings[name] = []

	for d, filePath in timingFiles:
		with open(filePath, "r") as f:
			# Scan through file until required test operation list index is found
			testOpListToRead = "Test Operations ({}):".format(testOpListIndex)
			print testOpListToRead
			line = ""
			while line != testOpListToRead:
				line = f.readline()[:-1] # [:-1] there to remove trailing newline
			# Process remaining lines as timings
			for line in f.readlines():
				match = STRUCTURE_TIMING_LINE_REGEX.match(line)
				if match:
					index = int(match.group(1))
					timingValue = match.group(2)
					structureTimings[structureNames[index]].append( (d, timingValue) )
				else:
					break
	# Sort structure timing lists by dimensionality
	for structureName, timings in structureTimings.items():
		timings.sort(key=lambda x : x[0])
	return structureTimings

def writeTimings(filename, timings):
	with open(filename, "w") as f:
		# Iterate through each structure
		for dimensionality, timingValue in timings.items():
			f.write("{} {}\n".format(dimensionality, timingValue))

if __name__ == "__main__":
	# Parse command line arguments
	if len(sys.argv) < 4:
		sys.exit("Usage: python {} <directory> <operation> <dataset> [<structure1Name> <structure2Name> ... <structureNName>]".format(sys.argv[0]))
	resultsDirectory = sys.argv[1]
	operation = sys.argv[2]
	dataset = sys.argv[3]
	structureNames = sys.argv[4:]

	# If operation is insert, USE A SPECIALISED PROCESSOR 
	if operation == "insert":
		# Determine hich test operation list results must be read
		testOpListIndex = DATASET_TEST_OP_INDICES[dataset]
		structureTimings = getStructureTimings(
			getInsertFilesToProcess(resultsDirectory),
			structureNames, testOpListIndex
		)
	else:
		structureTimings = getStructureTimings(
			getFilesToProcess(resultsDirectory, operation, dataset),
			structureNames
		)
		
	for structureName, timings in structureTimings.items():
		print structureName
		for dimensionality, timingValue in timings:
			print "\t", dimensionality, timingValue
		#writeTimings(filename, timings)
