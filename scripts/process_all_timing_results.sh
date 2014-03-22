#!/bin/sh

if [ -z "$1" ]; then
	echo "Usage: process_all_timing_results.sh <outputDirectory>"
	exit 1
fi

python process_timing_results.py "$1" results/ insert randuniform sequentialscan octree pyramidtreenodefrag pyramidtree
python process_timing_results.py "$1" results/ insert skewed sequentialscan octree pyramidtreenodefrag pyramidtree
python process_timing_results.py "$1" results/ insert clustered sequentialscan octree pyramidtreenodefrag pyramidtree

python process_timing_results.py "$1" results/ delete randuniform sequentialscan octree pyramidtreenodefrag pyramidtree
python process_timing_results.py "$1" results/ delete skewed sequentialscan octree pyramidtreenodefrag pyramidtree
python process_timing_results.py "$1" results/ delete clustered sequentialscan octree pyramidtreenodefrag pyramidtree

python process_timing_results.py "$1" results/ pquery randuniform sequentialscan octree pyramidtreenodefrag pyramidtree
python process_timing_results.py "$1" results/ pquery skewed sequentialscan octree pyramidtreenodefrag pyramidtree
python process_timing_results.py "$1" results/ pquery clustered sequentialscan octree pyramidtreenodefrag pyramidtree