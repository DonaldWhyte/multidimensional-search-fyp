#!/bin/bash

# Parse command line arguments
if [ $# -lt 2 ] ; then
	echo "Usage: process_all_timing_results.sh <inputDirectory> <outputDirectory>"
	exit 1
fi
inputDirectory="$1"
outputDirectory="$2"

./process_timing_results.py "$outputDirectory" "$inputDirectory" insert randuniform sequentialscan octree indexpyramidtree indexpyramidtree_defragment
./process_timing_results.py "$outputDirectory" "$inputDirectory" insert skewed sequentialscan octree indexpyramidtree indexpyramidtree_defragment
./process_timing_results.py "$outputDirectory" "$inputDirectory" insert clustered sequentialscan octree indexpyramidtree indexpyramidtree_defragment

./process_timing_results.py "$outputDirectory" "$inputDirectory" delete randuniform sequentialscan octree indexpyramidtree indexpyramidtree_defragment
./process_timing_results.py "$outputDirectory" "$inputDirectory" delete skewed sequentialscan octree indexpyramidtree indexpyramidtree_defragment
./process_timing_results.py "$outputDirectory" "$inputDirectory" delete clustered sequentialscan octree indexpyramidtree indexpyramidtree_defragment

./process_timing_results.py "$outputDirectory" "$inputDirectory" pquery randuniform sequentialscan octree indexpyramidtree indexpyramidtree_defragment
./process_timing_results.py "$outputDirectory" "$inputDirectory" pquery skewed sequentialscan octree indexpyramidtree indexpyramidtree_defragment
./process_timing_results.py "$outputDirectory" "$inputDirectory" pquery clustered sequentialscan octree indexpyramidtree indexpyramidtree_defragment

./process_size_results.py "$outputDirectory" "$inputDirectory" insert sequentialscan indexpyramidtree indexpyramidtree_defragment
./process_size_results.py "$outputDirectory" "$inputDirectory" delete sequentialscan indexpyramidtree indexpyramidtree_defragment
./process_size_results.py "$outputDirectory" "$inputDirectory" pquery sequentialscan indexpyramidtree indexpyramidtree_defragment