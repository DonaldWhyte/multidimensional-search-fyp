#!/bin/bash

if [ -z "$1" ] ; then
	echo "Usage: plot_all_timings.sh <timingsDirectory>"
	exit 1
fi

function plotTiming {
	# Arguments: <resultsDirectory> <timingBaseFilename>
	gnuplot -e "outputFilename='$1/$2.ps'" -e "inputFilename='$1/$2.times'" -e "plotTitle='$2" plot_timing.plg
}

function plotMultipleTimings {
	# Arguments: <resultsDirectory> <operationType> <dataset>
	gnuplot -e "inputDirectory='$1/'" -e "operation='$2'" -e "dataset='$3'" multiplot_timings.plg
}

# Plotting all four structures in one graph, for each operation-dataset pair
plotMultipleTimings "$1" "insert" "randuniform"
plotMultipleTimings "$1" "insert" "skewed"
plotMultipleTimings "$1" "insert" "clustered"
plotMultipleTimings "$1" "delete" "randuniform"
plotMultipleTimings "$1" "delete" "skewed"
plotMultipleTimings "$1" "delete" "clustered"
plotMultipleTimings "$1" "pquery" "randuniform"
plotMultipleTimings "$1" "pquery" "skewed"
plotMultipleTimings "$1" "pquery" "clustered"