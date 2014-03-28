#!/bin/bash

# Parse command line arguments
if [ $# -lt 2 ] ; then
	echo "Usage: process_all_timing_results.sh <inputDirectory> <outputDirectory>"
	exit 1
fi
inputDirectory="$1"
outputDirectory="$2"

./process_timing_results.py "$outputDirectory" "$inputDirectory" insert randuniform indexpyramidtree indexpyramidtree_defragment indexpyramidtree_rebuild bucketpyramidtree splaypyramidtree
./process_timing_results.py "$outputDirectory" "$inputDirectory" insert skewed indexpyramidtree indexpyramidtree_defragment indexpyramidtree_rebuild bucketpyramidtree splaypyramidtree
./process_timing_results.py "$outputDirectory" "$inputDirectory" insert clustered indexpyramidtree indexpyramidtree_defragment indexpyramidtree_rebuild bucketpyramidtree splaypyramidtree

./process_timing_results.py "$outputDirectory" "$inputDirectory" delete randuniform indexpyramidtree indexpyramidtree_defragment indexpyramidtree_rebuild bucketpyramidtree splaypyramidtree
./process_timing_results.py "$outputDirectory" "$inputDirectory" delete skewed indexpyramidtree indexpyramidtree_defragment indexpyramidtree_rebuild bucketpyramidtree splaypyramidtree
./process_timing_results.py "$outputDirectory" "$inputDirectory" delete clustered indexpyramidtree indexpyramidtree_defragment indexpyramidtree_rebuild bucketpyramidtree splaypyramidtree

./process_timing_results.py "$outputDirectory" "$inputDirectory" pquery randuniform indexpyramidtree indexpyramidtree_defragment indexpyramidtree_rebuild bucketpyramidtree splaypyramidtree
./process_timing_results.py "$outputDirectory" "$inputDirectory" pquery skewed indexpyramidtree indexpyramidtree_defragment indexpyramidtree_rebuild bucketpyramidtree splaypyramidtree
./process_timing_results.py "$outputDirectory" "$inputDirectory" pquery clustered indexpyramidtree indexpyramidtree_defragment indexpyramidtree_rebuild bucketpyramidtree splaypyramidtree