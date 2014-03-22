import sys
import time
import random
import math
from generate_random_operations import *
from generate_random_dataset import generatePoint

def getOperationMaximums(args):
	maximums = [ -1, -1, -1, -1 ]
	maxIndex = min(4, len(args))
	for i in range(maxIndex):
		maximums[i] = int(args[i])
	return maximums

def parsePointStr(pointStr):
	fields = pointStr.split()
	return [ float(x) for x in fields ]

def generateDatasetOperation(numDimensions, cumulativeOpProbs, dataset):
	# Randomly choose operation
	val = random.random()
	if val >= 0 and val < cumulativeOpProbs[0]:
		opType = "I"
	elif val >= cumulativeOpProbs[0] and val < cumulativeOpProbs[1]:
		opType = "D"
	elif val >= cumulativeOpProbs[1] and val < cumulativeOpProbs[2]:
		opType = "U"
	elif val >= cumulativeOpProbs[2] and val <= cumulativeOpProbs[3]:
		opType = "P"
	else:
		raise RuntimeError("Unknown random value generated in generateRandomOperation()")

	return generateDatasetOperationWithType(numDimensions, opType, dataset)

def generateDatasetOperationWithType(numDimensions, opType, dataset):
	# Choose random point from dataset
	randomIndex = random.randint(0, len(dataset) - 1)
	point = list(dataset[randomIndex])
	# Select another point if the operation is an Update
	if opType == "U":
		randomIndex = random.randint(0, len(dataset) - 1)
		point += dataset[randomIndex]
		
	return (opType, point)	

def printSingleOperationTypeList(opType, numDimensions, dataset):
	print("{} {}".format(numDimensions, len(dataset)))
	for p in dataset:
		op = (opType, list(p))
		print(operationToStr(op))


if __name__ == "__main__":
	# Seed random number generator
	random.seed(time.time())	

	# Parse command line arguments	
	if len(sys.argv) < 3:
		sys.exit("Usage: {} <datasetFilename> [all|random|equal|insert|delete|update|pquery] {{<numOperations> <insertProbability> <deleteProbability> <updateProbability> <pqueryProbability> <maxInsert> <maxDelete> <maxUpdate> <maxPQuery>}}".format(sys.argv[0]))
	datasetFilename = sys.argv[1]
	listType = sys.argv[2]
	if not listType in [ "all", "random", "equal", "insert", "delete", "update", "pquery" ]:
		sys.exit("Invalid list type '{}' given".format(listType))
	numOperations = 0
	if listType in ["random", "equal"]:
		if len(sys.argv) < 4:
			sys.exit("The number of operations to generate must be given if 'all' mode is node used")
		else:
			numOperations = int(sys.argv[3])

	operationProbabilities = getOperationProbabilities(sys.argv[4:])
	operationProbabilities = normaliseProbabilities(operationProbabilities)
	cumulativeOpProbs = computeCumulativeProbabilities(operationProbabilities)

	operationMaximums = getOperationMaximums(sys.argv[8:])
	
	# Load specified dataset into memory
	numDimensions = 0
	dataset = []
	with open(datasetFilename, "r") as f:
		headerFields = f.readline().split()
		if len(headerFields) < 2:
			sys.exit("Dataset header file does not have both #dimensions and #points")	
		numDimensions, numDatasetPoints = int(headerFields[0]), int(headerFields[1])

		dataset = [ parsePointStr(line) for line in f.readlines() ]
		dataset = dataset[:numDatasetPoints]

	# If generate mode is 'all', then all points are inserted, queries, then deleted in bulk
	if listType == "all":
		print("{} {}".format(numDimensions, len(dataset) * 3))
		for p in dataset:
			print("I {}".format( pointToStr(p) ))
		for p in dataset:
			print("P {}".format( pointToStr(p) ))
		for p in dataset:
			print("D {}".format( pointToStr(p) ))
	# If 'random', randomly generate operations that use points FROM THE DATASET using given probabilities
	elif listType == "random":
		print("{} {}".format(numDimensions, numOperations))
		for i in range(numOperations):
			op = generateDatasetOperation(numDimensions, cumulativeOpProbs, dataset)
			print(operationToStr(op))
	elif listType == "equal":
		print("{} {}".format(numDimensions, numOperations))

		# Compute how many of each operation to generate
		opsPerType = int(numOperations / 4) + 1
		# Generate the same amount of operations for each type
		operations = [ generateDatasetOperationWithType(numDimensions, "I", dataset) for i in range(opsPerType) ]
		operations += [ generateDatasetOperationWithType(numDimensions, "D", dataset) for i in range(opsPerType) ]
		operations += [ generateDatasetOperationWithType(numDimensions, "U", dataset) for i in range(opsPerType) ]
		operations += [ generateDatasetOperationWithType(numDimensions, "P", dataset) for i in range(opsPerType) ]
		operations = operations[:numOperations]
		# Shuffle all operations
		random.shuffle(operations)
		# Output all opewrations
		for op in operations:
			print(operationToStr(op))
	# Generate olists which have a single type of operation, each operation being for a single point in the dataset
	elif listType == "insert":
		printSingleOperationTypeList("I", numDimensions, dataset)
	elif listType == "delete":
		printSingleOperationTypeList("D", numDimensions, dataset)
	elif listType == "update":
		print("{} {}".format(numDimensions, len(dataset)))
		for p in dataset:
			updateArgs = list(p)
			# Generate random point to update the given point with
			updateArgs += generatePoint(numDimensions)
			op = ("U", updateArgs)
			print(operationToStr(op))
	elif listType == "pquery":
		printSingleOperationTypeList("P", numDimensions, dataset)