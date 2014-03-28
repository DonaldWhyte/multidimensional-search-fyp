#!/bin/bash

if [ -z "$1" ] ; then
	echo "Usage: plot_all_timings.sh <timingsDirectory>"
	exit 1
fi
timingsDirectory="$1"

function plotMultipleTimings {
	# Arguments: <resultsDirectory> <operationType> <dataset>
	gnuplot -e "inputDirectory='$timingsDirectory/'" -e "operation='$2'" -e "dataset='$3'" it2_plotscript.plg
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