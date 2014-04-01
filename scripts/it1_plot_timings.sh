#!/bin/bash

if [ -z "$1" ] ; then
	echo "Usage: plot_all_timings.sh <timingsDirectory>"
	exit 1
fi
timingsDirectory="$1"

function plotMultipleTimings {
	# Arguments: <resultsDirectory> <operationType> <dataset>
	outputFilename="$1/all_$2_$3.pdf"
	python2 plot_2d_datasets.py "$outputFilename" "Dimensions" "Execution Time (in seconds)" "Sequential Scan" $1/sequentialscan_$2_$3.times "Octree" $1/octree_$2_randuniform.times "Index Pyramid Tree" $1/indexpyramidtree_$2_$3.times "Index Pyramid Tree (Defragmented)" $1/indexpyramidtree_defragment_$2_$3.times 
}

function plotSizeTimings {
	# Arguments: <resultsDirectory> <operationType> {total|average}
	outputFilename="$1/all_$2_sizevary_$3.pdf"
	if [ "$3" = "total" ] ; then
		prefix="Total"
	else
		prefix="Average"
	fi
	python2 plot_2d_datasets.py "$outputFilename" "Number of Points (n)" "$prefix Execution Time (in seconds)" "Sequential Scan" $1/sequentialscan_$2_sizevary_$3.times "Index Pyramid Tree" $1/indexpyramidtree_$2_sizevary_$3.times "Index Pyramid Tree (Defragmented)" $1/indexpyramidtree_defragment_$2_sizevary_$3.times
}

# Plotting all four structures in one graph, for each operation-dataset pair
plotMultipleTimings "$timingsDirectory" "insert" "randuniform"
plotMultipleTimings "$timingsDirectory" "insert" "skewed"
plotMultipleTimings "$timingsDirectory" "insert" "clustered"
plotMultipleTimings "$timingsDirectory" "delete" "randuniform"
plotMultipleTimings "$timingsDirectory" "delete" "skewed"
plotMultipleTimings "$timingsDirectory" "delete" "clustered"
plotMultipleTimings "$timingsDirectory" "pquery" "randuniform"
plotMultipleTimings "$timingsDirectory" "pquery" "skewed"
plotMultipleTimings "$timingsDirectory" "pquery" "clustered"
plotSizeTimings "$timingsDirectory" "insert" "total"
plotSizeTimings "$timingsDirectory" "delete" "total"
plotSizeTimings "$timingsDirectory" "pquery" "total"
plotSizeTimings "$timingsDirectory" "insert" "average"
plotSizeTimings "$timingsDirectory" "delete" "average"
plotSizeTimings "$timingsDirectory" "pquery" "average"