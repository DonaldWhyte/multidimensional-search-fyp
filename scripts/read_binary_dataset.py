import sys
import os
import struct

def readBinaryDataset(filename):
	dataset = [ ]
	with open(filename, "rb") as f:
		rawByte = f.read(1)
		while rawByte:
			dataset.append( struct.unpack("B", rawByte)[0] )
			rawByte = f.read(1)
		
	return dataset

if __name__ == "__main__":
	# Parse command line arguments
	if len(sys.argv) < 2:
		sys.exit("python {} <datasetFilename>".format(sys.argv[0]))
	datasetFilename = sys.argv[1]

	dataset = readBinaryDataset(datasetFilename)
	dataset = [ float(x) / 256.0 for x in dataset ]
	print(dataset[:100])
	print(len(dataset))