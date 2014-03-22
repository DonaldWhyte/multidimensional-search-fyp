#!/bin/bash

if [ -z "$1" ] ; then
	echo "Usage: plot_all_timings.sh <timingsDirectory>"
	exit 1
fi

function plotTiming {
	gnuplot -e "outputFilename='$1/$2.ps'" -e "inputFilename='$1/$2.times'" -e "plotTitle='$2" plot_timing.plg
}

# TODO: plot all four structures in one graph!!! one graph for each operation-dataset pair

plotTiming "$1" "sequentialscan_insert_randuniform"
plotTiming "$1" "sequentialscan_insert_skewed"
plotTiming "$1" "sequentialscan_insert_clustered"
plotTiming "$1" "octree_insert_randuniform"
plotTiming "$1" "octree_insert_skewed"
plotTiming "$1" "octree_insert_clustered"
plotTiming "$1" "pyramidtreenodefrag_insert_randuniform"
plotTiming "$1" "pyramidtreenodefrag_insert_skewed"
plotTiming "$1" "pyramidtreenodefrag_insert_clustered"
plotTiming "$1" "pyramidtree_insert_randuniform"
plotTiming "$1" "pyramidtree_insert_skewed"
plotTiming "$1" "pyramidtree_insert_clustered"

plotTiming "$1" "sequentialscan_delete_randuniform"
plotTiming "$1" "sequentialscan_delete_skewed"
plotTiming "$1" "sequentialscan_delete_clustered"
plotTiming "$1" "octree_delete_randuniform"
plotTiming "$1" "octree_delete_skewed"
plotTiming "$1" "octree_delete_clustered"
plotTiming "$1" "pyramidtreenodefrag_delete_randuniform"
plotTiming "$1" "pyramidtreenodefrag_delete_skewed"
plotTiming "$1" "pyramidtreenodefrag_delete_clustered"
plotTiming "$1" "pyramidtree_delete_randuniform"
plotTiming "$1" "pyramidtree_delete_skewed"
plotTiming "$1" "pyramidtree_delete_clustered"

plotTiming "$1" "sequentialscan_pquery_randuniform"
plotTiming "$1" "sequentialscan_pquery_skewed"
plotTiming "$1" "sequentialscan_pquery_clustered"
plotTiming "$1" "octree_pquery_randuniform"
plotTiming "$1" "octree_pquery_skewed"
plotTiming "$1" "octree_pquery_clustered"
plotTiming "$1" "pyramidtreenodefrag_pquery_randuniform"
plotTiming "$1" "pyramidtreenodefrag_pquery_skewed"
plotTiming "$1" "pyramidtreenodefrag_pquery_clustered"
plotTiming "$1" "pyramidtree_pquery_randuniform"
plotTiming "$1" "pyramidtree_pquery_skewed"
plotTiming "$1" "pyramidtree_pquery_clustered"