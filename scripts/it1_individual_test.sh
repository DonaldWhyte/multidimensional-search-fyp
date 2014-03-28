#!/bin/bash

# Set base directories for input data and output results
inputDirectory="/media/Maxtor/DonaldFYP/test_data"

timeout 5m ../build/evaluator --runs=2 --index_structures=sequential_scan,10 --index_structures=octree,10,0,1 --index_structures=index_pyramid_tree,10,0,1 -t$inputDirectory/operation_lists/randuniform_10d_insert.ops --index_structures=index_pyramid_tree,10,0,1,3000,defragment
timeout 5m ../build/evaluator --runs=2 --index_structures=sequential_scan,10 --index_structures=octree,10,0,1 --index_structures=index_pyramid_tree,10,0,1 -t$inputDirectory/operation_lists/randuniform_10d_delete.ops -p$inputDirectory/datasets/randuniform_10d.dat --index_structures=index_pyramid_tree,10,0,1,3000,defragment
timeout 5m ../build/evaluator --runs=2 --index_structures=sequential_scan,10 --index_structures=octree,10,0,1 --index_structures=index_pyramid_tree,10,0,1 -t$inputDirectory/operation_lists/randuniform_10d_pquery.ops -p$inputDirectory/datasets/randuniform_10d.dat --index_structures=index_pyramid_tree,10,0,1,3000,defragment