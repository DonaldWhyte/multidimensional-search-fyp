import sys
import time
import random

def generatePoint(numDimensions):
	return [ random.random() for d in range(numDimensions) ]

def pointToStr(point):
	pointStr = ""
	for i in range(len(point) - 1):
		pointStr += "{} ".format(point[i])
	pointStr += str(point[-1])
	return pointStr

if __name__ == "__main__":
	# Seed random number generator
	random.seed(time.time())

	# Parse command line arguments
	if len(sys.argv) < 3:
		sys.exit("Usage: {} <numDimensions> <numPoints>".format(sys.argv[0]))
	numDimensions = 0
	numPoints = 0
	try:
		numDimensions = int(sys.argv[1])
		numPoints = int(sys.argv[2])
		if numDimensions <= 0:
			raise ValueError("Cannot have less than one dimension for points")
		if numPoints < 0:
			raise ValueError("Negative amount of points requested")
	except ValueError as e:
		sys.exit("Error: {}".format(e))

	# Output header (number of dimensikns and points)
	print("{} {}".format(numDimensions, numPoints))
	# Generate points and print them on-the-fly
	for i in range(numPoints):
		print pointToStr( generatePoint(numDimensions) )