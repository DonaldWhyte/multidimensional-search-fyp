import sys
import time
import random
from generate_random_dataset import generatePoint, pointToStr

OPERATION_TYPE_INSERT = "I"
OPERATION_TYPE_DELETE = "D"
OPERATION_TYPE_UPDATE = "U"
OPERATION_TYPE_POINTQUERY = "P"

def getOperationProbabilities(args):
	operationProbabilities = [ 1, 1, 1, 1 ]
	maxIndex = min(4, len(args))
	for i in range(maxIndex):
		operationProbabilities[i] = float(args[i])
	return operationProbabilities

def normaliseProbabilities(probabilities):
	total = float(sum(operationProbabilities))
	return [ p / total for p in operationProbabilities ]

def computeCumulativeProbabilities(probabilities):
	cumulativeSum = 0
	cumulativeOpProbs = []
	for p in probabilities:
		cumulativeSum += p
		cumulativeOpProbs.append(cumulativeSum)
	return cumulativeOpProbs

def generateRandomOperation(numDimensions, cumulativeOpProbs):
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
	# Randomly generatr a point
	point = generatePoint(numDimensions)
	return (opType, point)

def operationToStr(operation):
	return "{} {}".format(operation[0], pointToStr(operation[1]))

if __name__ == "__main__":
	# Seed random number generator
	random.seed(time.time())

	# Parse command line arguments
	if len(sys.argv) < 3:
		sys.exit("Usage: {} <numDimensions> <numOperations> <insertProbability> <deleteProbability> <updateProbability> <pqueryProbability".format(sys.argv[0]))
	numDimensions = 0
	numOperations = 0
	try:
		numDimensions = int(sys.argv[1])
		numOperations = int(sys.argv[2])
		if numDimensions <= 0:
			raise ValueError("Cannot have less than one dimension for points")
		if numOperations < 0:
			raise ValueError("Negative amount of operations requested")
	except ValueError as e:
		sys.exit("Error: {}".format(e))
	operationProbabilities = getOperationProbabilities(sys.argv[3:])

	# Normalise probabilities so they're in range 0-1 and sum up to 1
	operationProbabilities = normaliseProbabilities(operationProbabilities)
	# Construct CUMULATIVE probabilities for operations
	cumulativeOpProbs = computeCumulativeProbabilities(operationProbabilities)
	# Output header (number of dimensions and operations)
	print("{} {}".format(numDimensions, numOperations))
	# Generate points and print them on-the-fly
	for i in range(numOperations):
		print operationToStr( generateRandomOperation(numDimensions, cumulativeOpProbs) )