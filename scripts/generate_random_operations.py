import sys
import time
import random
from generate_random_dataset import generatePoint, pointToStr

OPERATION_TYPE_INSERT = "I"
OPERATION_TYPE_DELETE = "D"
OPERATION_TYPE_UPDATE = "U"
OPERATION_TYPE_POINTQUERY = "P"

def generateRandomOperation(numDimensions, cumulativeOpProbs):
	# Randomly choose operation
	val = random.random()
	if val >= 0 and val < cumulativeOpProbs[0]:
		opType = "I"
	elif val >= cumulativeOpProbs[0] and val < cumulativeOpProbs[1]:
		opType = "D"
	elif val >= cumulativeOpProbs[1] and val < cumulativeOpProbs[2]:
		opType = "U"
	elif val >= cumulativeOpProbs[2] and val <= 1:
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
	operationProbabilities = [ 1, 1, 1, 1 ]
	for i in range(4):
		index = 3 + i
		if index < len(sys.argv):
			operationProbabilities[i] = float(sys.argv[index])
		else:
			break # no more arguments to process!

	# Normalise probabilities so they're in range 0-1 and sum up to 1
	total = float(sum(operationProbabilities))
	operationProbabilities = [ p / total for p in operationProbabilities ]
	# Construct CUMULATIVE probabilities for operations
	cumulativeOpProbs = [
		operationProbabilities[0],
		operationProbabilities[0] + operationProbabilities[1],
		operationProbabilities[0] + operationProbabilities[1] + operationProbabilities[2] 
	]
	# Output header (number of dimensions and operations)
	print("{} {}".format(numDimensions, numOperations))
	# Generate points and print them on-the-fly
	for i in range(numOperations):
		print operationToStr( generateRandomOperation(numDimensions, cumulativeOpProbs) )