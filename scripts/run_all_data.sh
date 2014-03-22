#!/bin/sh

# Ensure directory to store results is there
mkdir results -p

# INSERT OPERTATIONS (ALL)
timeout 10m ../build/evaluator --runs=2 --output=results/times_insert_1d.txt --index_structures=sequential_scan,1 --index_structures=octree,1,0,1 --index_structures=pyramid_tree,1,0,1 -t../test_data/operation_lists/randuniform_1d_insert.ops -t../test_data/operation_lists/skewed_1d_insert.ops -t../test_data/operation_lists/randuniform_1d_insert.ops --index_structures=pyramid_tree,1,0,1,6000
timeout 10m ../build/evaluator --runs=2 --output=results/times_insert_2d.txt --index_structures=sequential_scan,2 --index_structures=octree,2,0,1 --index_structures=pyramid_tree,2,0,1 -t../test_data/operation_lists/randuniform_2d_insert.ops -t../test_data/operation_lists/skewed_2d_insert.ops -t../test_data/operation_lists/randuniform_2d_insert.ops --index_structures=pyramid_tree,2,0,1,6000
timeout 10m ../build/evaluator --runs=2 --output=results/times_insert_3d.txt --index_structures=sequential_scan,3 --index_structures=octree,3,0,1 --index_structures=pyramid_tree,3,0,1 -t../test_data/operation_lists/randuniform_3d_insert.ops -t../test_data/operation_lists/skewed_3d_insert.ops -t../test_data/operation_lists/randuniform_3d_insert.ops --index_structures=pyramid_tree,3,0,1,6000
timeout 10m ../build/evaluator --runs=2 --output=results/times_insert_5d.txt --index_structures=sequential_scan,5 --index_structures=octree,5,0,1 --index_structures=pyramid_tree,5,0,1 -t../test_data/operation_lists/randuniform_5d_insert.ops -t../test_data/operation_lists/skewed_5d_insert.ops -t../test_data/operation_lists/randuniform_5d_insert.ops --index_structures=pyramid_tree,5,0,1,6000
timeout 10m ../build/evaluator --runs=2 --output=results/times_insert_8d.txt --index_structures=sequential_scan,8 --index_structures=octree,8,0,1 --index_structures=pyramid_tree,8,0,1 -t../test_data/operation_lists/randuniform_8d_insert.ops -t../test_data/operation_lists/skewed_8d_insert.ops -t../test_data/operation_lists/randuniform_8d_insert.ops --index_structures=pyramid_tree,8,0,1,6000
timeout 10m ../build/evaluator --runs=2 --output=results/times_insert_10d.txt --index_structures=sequential_scan,10 --index_structures=octree,10,0,1 --index_structures=pyramid_tree,10,0,1 -t../test_data/operation_lists/randuniform_10d_insert.ops -t../test_data/operation_lists/skewed_10d_insert.ops -t../test_data/operation_lists/randuniform_10d_insert.ops --index_structures=pyramid_tree,10,0,1,6000
timeout 10m ../build/evaluator --runs=2 --output=results/times_insert_30d.txt --index_structures=sequential_scan,30 --index_structures=octree,30,0,1 --index_structures=pyramid_tree,30,0,1 -t../test_data/operation_lists/randuniform_30d_insert.ops -t../test_data/operation_lists/skewed_30d_insert.ops -t../test_data/operation_lists/randuniform_30d_insert.ops --index_structures=pyramid_tree,30,0,1,6000
timeout 10m ../build/evaluator --runs=2 --output=results/times_insert_50d.txt --index_structures=sequential_scan,50 --index_structures=octree,50,0,1 --index_structures=pyramid_tree,50,0,1 -t../test_data/operation_lists/randuniform_50d_insert.ops -t../test_data/operation_lists/skewed_50d_insert.ops -t../test_data/operation_lists/randuniform_50d_insert.ops --index_structures=pyramid_tree,50,0,1,6000
timeout 10m ../build/evaluator --runs=2 --output=results/times_insert_100d.txt --index_structures=sequential_scan,100 --index_structures=octree,100,0,1 --index_structures=pyramid_tree,100,0,1 -t../test_data/operation_lists/randuniform_100d_insert.ops -t../test_data/operation_lists/skewed_100d_insert.ops -t../test_data/operation_lists/randuniform_100d_insert.ops --index_structures=pyramid_tree,100,0,1,6000
timeout 10m ../build/evaluator --runs=2 --output=results/times_insert_200d.txt --index_structures=sequential_scan,200 --index_structures=octree,200,0,1 --index_structures=pyramid_tree,200,0,1 -t../test_data/operation_lists/randuniform_200d_insert.ops -t../test_data/operation_lists/skewed_200d_insert.ops -t../test_data/operation_lists/randuniform_200d_insert.ops --index_structures=pyramid_tree,200,0,1,6000

# DELETE OPERTATIONS (RANDOM UNIFORM)
timeout 10m ../build/evaluator --runs=2 --output=results/times_delete_randuniform_1d.txt --index_structures=sequential_scan,1 --index_structures=octree,1,0,1 --index_structures=pyramid_tree,1,0,1 -t../test_data/operation_lists/randuniform_1d_delete.ops -p../test_data/datasets/randuniform_1d.dat --index_structures=pyramid_tree,1,0,1,6000
timeout 10m ../build/evaluator --runs=2 --output=results/times_delete_randuniform_2d.txt --index_structures=sequential_scan,2 --index_structures=octree,2,0,1 --index_structures=pyramid_tree,2,0,1 -t../test_data/operation_lists/randuniform_2d_delete.ops -p../test_data/datasets/randuniform_2d.dat --index_structures=pyramid_tree,2,0,1,6000
timeout 10m ../build/evaluator --runs=2 --output=results/times_delete_randuniform_3d.txt --index_structures=sequential_scan,3 --index_structures=octree,3,0,1 --index_structures=pyramid_tree,3,0,1 -t../test_data/operation_lists/randuniform_3d_delete.ops -p../test_data/datasets/randuniform_3d.dat --index_structures=pyramid_tree,3,0,1,6000
timeout 10m ../build/evaluator --runs=2 --output=results/times_delete_randuniform_5d.txt --index_structures=sequential_scan,5 --index_structures=octree,5,0,1 --index_structures=pyramid_tree,5,0,1 -t../test_data/operation_lists/randuniform_5d_delete.ops -p../test_data/datasets/randuniform_5d.dat --index_structures=pyramid_tree,5,0,1,6000
timeout 10m ../build/evaluator --runs=2 --output=results/times_delete_randuniform_8d.txt --index_structures=sequential_scan,8 --index_structures=octree,8,0,1 --index_structures=pyramid_tree,8,0,1 -t../test_data/operation_lists/randuniform_8d_delete.ops -p../test_data/datasets/randuniform_8d.dat --index_structures=pyramid_tree,8,0,1,6000
timeout 10m ../build/evaluator --runs=2 --output=results/times_delete_randuniform_10d.txt --index_structures=sequential_scan,10 --index_structures=octree,10,0,1 --index_structures=pyramid_tree,10,0,1 -t../test_data/operation_lists/randuniform_10d_delete.ops -p../test_data/datasets/randuniform_10d.dat --index_structures=pyramid_tree,10,0,1,6000
timeout 10m ../build/evaluator --runs=2 --output=results/times_delete_randuniform_30d.txt --index_structures=sequential_scan,30 --index_structures=octree,30,0,1 --index_structures=pyramid_tree,30,0,1 -t../test_data/operation_lists/randuniform_30d_delete.ops -p../test_data/datasets/randuniform_30d.dat --index_structures=pyramid_tree,30,0,1,6000
timeout 10m ../build/evaluator --runs=2 --output=results/times_delete_randuniform_50d.txt --index_structures=sequential_scan,50 --index_structures=octree,50,0,1 --index_structures=pyramid_tree,50,0,1 -t../test_data/operation_lists/randuniform_50d_delete.ops -p../test_data/datasets/randuniform_50d.dat --index_structures=pyramid_tree,50,0,1,6000
timeout 10m ../build/evaluator --runs=2 --output=results/times_delete_randuniform_100d.txt --index_structures=sequential_scan,100 --index_structures=octree,100,0,1 --index_structures=pyramid_tree,100,0,1 -t../test_data/operation_lists/randuniform_100d_delete.ops -p../test_data/datasets/randuniform_100d.dats --index_structures=pyramid_tree,100,0,1,6000
timeout 10m ../build/evaluator --runs=2 --output=results/times_delete_randuniform_200d.txt --index_structures=sequential_scan,200 --index_structures=octree,200,0,1 --index_structures=pyramid_tree,200,0,1 -t../test_data/operation_lists/randuniform_200d_delete.ops -p../test_data/datasets/randuniform_200d.dat --index_structures=pyramid_tree,200,0,1,6000

# DELETE OPERTATIONS (SKEWED)
timeout 10m ../build/evaluator --runs=2 --output=results/times_delete_skewed_1d.txt --index_structures=sequential_scan,1 --index_structures=octree,1,0,1 --index_structures=pyramid_tree,1,0,1 -t../test_data/operation_lists/skewed_1d_delete.ops -p../test_data/datasets/skewed_1d.dat --index_structures=pyramid_tree,1,0,1,6000
timeout 10m ../build/evaluator --runs=2 --output=results/times_delete_skewed_2d.txt --index_structures=sequential_scan,2 --index_structures=octree,2,0,1 --index_structures=pyramid_tree,2,0,1 -t../test_data/operation_lists/skewed_2d_delete.ops -p../test_data/datasets/skewed_2d.dat --index_structures=pyramid_tree,2,0,1,6000
timeout 10m ../build/evaluator --runs=2 --output=results/times_delete_skewed_3d.txt --index_structures=sequential_scan,3 --index_structures=octree,3,0,1 --index_structures=pyramid_tree,3,0,1 -t../test_data/operation_lists/skewed_3d_delete.ops -p../test_data/datasets/skewed_3d.dat --index_structures=pyramid_tree,3,0,1,6000
timeout 10m ../build/evaluator --runs=2 --output=results/times_delete_skewed_5d.txt --index_structures=sequential_scan,5 --index_structures=octree,5,0,1 --index_structures=pyramid_tree,5,0,1 -t../test_data/operation_lists/skewed_5d_delete.ops -p../test_data/datasets/skewed_5d.dat --index_structures=pyramid_tree,5,0,1,6000
timeout 10m ../build/evaluator --runs=2 --output=results/times_delete_skewed_8d.txt --index_structures=sequential_scan,8 --index_structures=octree,8,0,1 --index_structures=pyramid_tree,8,0,1 -t../test_data/operation_lists/skewed_8d_delete.ops -p../test_data/datasets/skewed_8d.dat --index_structures=pyramid_tree,8,0,1,6000
timeout 10m ../build/evaluator --runs=2 --output=results/times_delete_skewed_10d.txt --index_structures=sequential_scan,10 --index_structures=octree,10,0,1 --index_structures=pyramid_tree,10,0,1 -t../test_data/operation_lists/skewed_10d_delete.ops -p../test_data/datasets/skewed_10d.dat --index_structures=pyramid_tree,10,0,1,6000
timeout 10m ../build/evaluator --runs=2 --output=results/times_delete_skewed_30d.txt --index_structures=sequential_scan,30 --index_structures=octree,30,0,1 --index_structures=pyramid_tree,30,0,1 -t../test_data/operation_lists/skewed_30d_delete.ops -p../test_data/datasets/skewed_30d.dat --index_structures=pyramid_tree,30,0,1,6000
timeout 10m ../build/evaluator --runs=2 --output=results/times_delete_skewed_50d.txt --index_structures=sequential_scan,50 --index_structures=octree,50,0,1 --index_structures=pyramid_tree,50,0,1 -t../test_data/operation_lists/skewed_50d_delete.ops -p../test_data/datasets/skewed_50d.dat --index_structures=pyramid_tree,50,0,1,6000
timeout 10m ../build/evaluator --runs=2 --output=results/times_delete_skewed_100d.txt --index_structures=sequential_scan,100 --index_structures=octree,100,0,1 --index_structures=pyramid_tree,100,0,1 -t../test_data/operation_lists/skewed_100d_delete.ops -p../test_data/datasets/skewed_100d.dats --index_structures=pyramid_tree,100,0,1,6000
timeout 10m ../build/evaluator --runs=2 --output=results/times_delete_skewed_200d.txt --index_structures=sequential_scan,200 --index_structures=octree,200,0,1 --index_structures=pyramid_tree,200,0,1 -t../test_data/operation_lists/skewed_200d_delete.ops -p../test_data/datasets/skewed_200d.dat --index_structures=pyramid_tree,200,0,1,6000

# DELETE OPERTATIONS (CLUSTERED)
timeout 10m ../build/evaluator --runs=2 --output=results/times_delete_clustered_1d.txt --index_structures=sequential_scan,1 --index_structures=octree,1,0,1 --index_structures=pyramid_tree,1,0,1 -t../test_data/operation_lists/clustered_1d_delete.ops -p../test_data/datasets/clustered_1d.dat --index_structures=pyramid_tree,1,0,1,6000
timeout 10m ../build/evaluator --runs=2 --output=results/times_delete_clustered_2d.txt --index_structures=sequential_scan,2 --index_structures=octree,2,0,1 --index_structures=pyramid_tree,2,0,1 -t../test_data/operation_lists/clustered_2d_delete.ops -p../test_data/datasets/clustered_2d.dat --index_structures=pyramid_tree,2,0,1,6000
timeout 10m ../build/evaluator --runs=2 --output=results/times_delete_clustered_3d.txt --index_structures=sequential_scan,3 --index_structures=octree,3,0,1 --index_structures=pyramid_tree,3,0,1 -t../test_data/operation_lists/clustered_3d_delete.ops -p../test_data/datasets/clustered_3d.dat --index_structures=pyramid_tree,3,0,1,6000
timeout 10m ../build/evaluator --runs=2 --output=results/times_delete_clustered_5d.txt --index_structures=sequential_scan,5 --index_structures=octree,5,0,1 --index_structures=pyramid_tree,5,0,1 -t../test_data/operation_lists/clustered_5d_delete.ops -p../test_data/datasets/clustered_5d.dat --index_structures=pyramid_tree,5,0,1,6000
timeout 10m ../build/evaluator --runs=2 --output=results/times_delete_clustered_8d.txt --index_structures=sequential_scan,8 --index_structures=octree,8,0,1 --index_structures=pyramid_tree,8,0,1 -t../test_data/operation_lists/clustered_8d_delete.ops -p../test_data/datasets/clustered_8d.dat --index_structures=pyramid_tree,8,0,1,6000
timeout 10m ../build/evaluator --runs=2 --output=results/times_delete_clustered_10d.txt --index_structures=sequential_scan,10 --index_structures=octree,10,0,1 --index_structures=pyramid_tree,10,0,1 -t../test_data/operation_lists/clustered_10d_delete.ops -p../test_data/datasets/clustered_10d.dat --index_structures=pyramid_tree,10,0,1,6000
timeout 10m ../build/evaluator --runs=2 --output=results/times_delete_clustered_30d.txt --index_structures=sequential_scan,30 --index_structures=octree,30,0,1 --index_structures=pyramid_tree,30,0,1 -t../test_data/operation_lists/clustered_30d_delete.ops -p../test_data/datasets/clustered_30d.dat --index_structures=pyramid_tree,30,0,1,6000
timeout 10m ../build/evaluator --runs=2 --output=results/times_delete_clustered_50d.txt --index_structures=sequential_scan,50 --index_structures=octree,50,0,1 --index_structures=pyramid_tree,50,0,1 -t../test_data/operation_lists/clustered_50d_delete.ops -p../test_data/datasets/clustered_50d.dat --index_structures=pyramid_tree,50,0,1,6000
timeout 10m ../build/evaluator --runs=2 --output=results/times_delete_clustered_100d.txt --index_structures=sequential_scan,100 --index_structures=octree,100,0,1 --index_structures=pyramid_tree,100,0,1 -t../test_data/operation_lists/clustered_100d_delete.ops -p../test_data/datasets/clustered_100d.dats --index_structures=pyramid_tree,100,0,1,6000
timeout 10m ../build/evaluator --runs=2 --output=results/times_delete_clustered_200d.txt --index_structures=sequential_scan,200 --index_structures=octree,200,0,1 --index_structures=pyramid_tree,200,0,1 -t../test_data/operation_lists/clustered_200d_delete.ops -p../test_data/datasets/clustered_200d.dat --index_structures=pyramid_tree,200,0,1,6000

# POINT QUERY OPERTATIONS (RANDOM UNIFORM)
timeout 10m ../build/evaluator --runs=2 --output=results/times_pquery_randuniform_1d.txt --index_structures=sequential_scan,1 --index_structures=octree,1,0,1 --index_structures=pyramid_tree,1,0,1 -t../test_data/operation_lists/randuniform_1d_pquery.ops -p../test_data/datasets/randuniform_1d.dat --index_structures=pyramid_tree,1,0,1,6000
timeout 10m ../build/evaluator --runs=2 --output=results/times_pquery_randuniform_2d.txt --index_structures=sequential_scan,2 --index_structures=octree,2,0,1 --index_structures=pyramid_tree,2,0,1 -t../test_data/operation_lists/randuniform_2d_pquery.ops -p../test_data/datasets/randuniform_2d.dat --index_structures=pyramid_tree,2,0,1,6000
timeout 10m ../build/evaluator --runs=2 --output=results/times_pquery_randuniform_3d.txt --index_structures=sequential_scan,3 --index_structures=octree,3,0,1 --index_structures=pyramid_tree,3,0,1 -t../test_data/operation_lists/randuniform_3d_pquery.ops -p../test_data/datasets/randuniform_3d.dat --index_structures=pyramid_tree,3,0,1,6000
timeout 10m ../build/evaluator --runs=2 --output=results/times_pquery_randuniform_5d.txt --index_structures=sequential_scan,5 --index_structures=octree,5,0,1 --index_structures=pyramid_tree,5,0,1 -t../test_data/operation_lists/randuniform_5d_pquery.ops -p../test_data/datasets/randuniform_5d.dat --index_structures=pyramid_tree,5,0,1,6000
timeout 10m ../build/evaluator --runs=2 --output=results/times_pquery_randuniform_8d.txt --index_structures=sequential_scan,8 --index_structures=octree,8,0,1 --index_structures=pyramid_tree,8,0,1 -t../test_data/operation_lists/randuniform_8d_pquery.ops -p../test_data/datasets/randuniform_8d.dat --index_structures=pyramid_tree,8,0,1,6000
timeout 10m ../build/evaluator --runs=2 --output=results/times_pquery_randuniform_10d.txt --index_structures=sequential_scan,10 --index_structures=octree,10,0,1 --index_structures=pyramid_tree,10,0,1 -t../test_data/operation_lists/randuniform_10d_pquery.ops -p../test_data/datasets/randuniform_10d.dat --index_structures=pyramid_tree,10,0,1,6000
timeout 10m ../build/evaluator --runs=2 --output=results/times_pquery_randuniform_30d.txt --index_structures=sequential_scan,30 --index_structures=octree,30,0,1 --index_structures=pyramid_tree,30,0,1 -t../test_data/operation_lists/randuniform_30d_pquery.ops -p../test_data/datasets/randuniform_30d.dat --index_structures=pyramid_tree,30,0,1,6000
timeout 10m ../build/evaluator --runs=2 --output=results/times_pquery_randuniform_50d.txt --index_structures=sequential_scan,50 --index_structures=octree,50,0,1 --index_structures=pyramid_tree,50,0,1 -t../test_data/operation_lists/randuniform_50d_pquery.ops -p../test_data/datasets/randuniform_50d.dat --index_structures=pyramid_tree,50,0,1,6000
timeout 10m ../build/evaluator --runs=2 --output=results/times_pquery_randuniform_100d.txt --index_structures=sequential_scan,100 --index_structures=octree,100,0,1 --index_structures=pyramid_tree,100,0,1 -t../test_data/operation_lists/randuniform_100d_pquery.ops -p../test_data/datasets/randuniform_100d.dats --index_structures=pyramid_tree,100,0,1,6000
timeout 10m ../build/evaluator --runs=2 --output=results/times_pquery_randuniform_200d.txt --index_structures=sequential_scan,200 --index_structures=octree,200,0,1 --index_structures=pyramid_tree,200,0,1 -t../test_data/operation_lists/randuniform_200d_pquery.ops -p../test_data/datasets/randuniform_200d.dat --index_structures=pyramid_tree,200,0,1,6000

# POINT QUERY OPERTATIONS (SKEWED)
timeout 10m ../build/evaluator --runs=2 --output=results/times_pquery_skewed_1d.txt --index_structures=sequential_scan,1 --index_structures=octree,1,0,1 --index_structures=pyramid_tree,1,0,1 -t../test_data/operation_lists/skewed_1d_pquery.ops -p../test_data/datasets/skewed_1d.dat --index_structures=pyramid_tree,1,0,1,6000
timeout 10m ../build/evaluator --runs=2 --output=results/times_pquery_skewed_2d.txt --index_structures=sequential_scan,2 --index_structures=octree,2,0,1 --index_structures=pyramid_tree,2,0,1 -t../test_data/operation_lists/skewed_2d_pquery.ops -p../test_data/datasets/skewed_2d.dat --index_structures=pyramid_tree,2,0,1,6000
timeout 10m ../build/evaluator --runs=2 --output=results/times_pquery_skewed_3d.txt --index_structures=sequential_scan,3 --index_structures=octree,3,0,1 --index_structures=pyramid_tree,3,0,1 -t../test_data/operation_lists/skewed_3d_pquery.ops -p../test_data/datasets/skewed_3d.dat --index_structures=pyramid_tree,3,0,1,6000
timeout 10m ../build/evaluator --runs=2 --output=results/times_pquery_skewed_5d.txt --index_structures=sequential_scan,5 --index_structures=octree,5,0,1 --index_structures=pyramid_tree,5,0,1 -t../test_data/operation_lists/skewed_5d_pquery.ops -p../test_data/datasets/skewed_5d.dat --index_structures=pyramid_tree,5,0,1,6000
timeout 10m ../build/evaluator --runs=2 --output=results/times_pquery_skewed_8d.txt --index_structures=sequential_scan,8 --index_structures=octree,8,0,1 --index_structures=pyramid_tree,8,0,1 -t../test_data/operation_lists/skewed_8d_pquery.ops -p../test_data/datasets/skewed_8d.dat --index_structures=pyramid_tree,8,0,1,6000
timeout 10m ../build/evaluator --runs=2 --output=results/times_pquery_skewed_10d.txt --index_structures=sequential_scan,10 --index_structures=octree,10,0,1 --index_structures=pyramid_tree,10,0,1 -t../test_data/operation_lists/skewed_10d_pquery.ops -p../test_data/datasets/skewed_10d.dat --index_structures=pyramid_tree,10,0,1,6000
timeout 10m ../build/evaluator --runs=2 --output=results/times_pquery_skewed_30d.txt --index_structures=sequential_scan,30 --index_structures=octree,30,0,1 --index_structures=pyramid_tree,30,0,1 -t../test_data/operation_lists/skewed_30d_pquery.ops -p../test_data/datasets/skewed_30d.dat --index_structures=pyramid_tree,30,0,1,6000
timeout 10m ../build/evaluator --runs=2 --output=results/times_pquery_skewed_50d.txt --index_structures=sequential_scan,50 --index_structures=octree,50,0,1 --index_structures=pyramid_tree,50,0,1 -t../test_data/operation_lists/skewed_50d_pquery.ops -p../test_data/datasets/skewed_50d.dat --index_structures=pyramid_tree,50,0,1,6000
timeout 10m ../build/evaluator --runs=2 --output=results/times_pquery_skewed_100d.txt --index_structures=sequential_scan,100 --index_structures=octree,100,0,1 --index_structures=pyramid_tree,100,0,1 -t../test_data/operation_lists/skewed_100d_pquery.ops -p../test_data/datasets/skewed_100d.dats --index_structures=pyramid_tree,100,0,1,6000
timeout 10m ../build/evaluator --runs=2 --output=results/times_pquery_skewed_200d.txt --index_structures=sequential_scan,200 --index_structures=octree,200,0,1 --index_structures=pyramid_tree,200,0,1 -t../test_data/operation_lists/skewed_200d_pquery.ops -p../test_data/datasets/skewed_200d.dat --index_structures=pyramid_tree,200,0,1,6000

# POINT QUERY OPERTATIONS (CLUSTERED)
timeout 10m ../build/evaluator --runs=2 --output=results/times_pquery_clustered_1d.txt --index_structures=sequential_scan,1 --index_structures=octree,1,0,1 --index_structures=pyramid_tree,1,0,1 -t../test_data/operation_lists/clustered_1d_pquery.ops -p../test_data/datasets/clustered_1d.dat --index_structures=pyramid_tree,1,0,1,6000
timeout 10m ../build/evaluator --runs=2 --output=results/times_pquery_clustered_2d.txt --index_structures=sequential_scan,2 --index_structures=octree,2,0,1 --index_structures=pyramid_tree,2,0,1 -t../test_data/operation_lists/clustered_2d_pquery.ops -p../test_data/datasets/clustered_2d.dat --index_structures=pyramid_tree,2,0,1,6000
timeout 10m ../build/evaluator --runs=2 --output=results/times_pquery_clustered_3d.txt --index_structures=sequential_scan,3 --index_structures=octree,3,0,1 --index_structures=pyramid_tree,3,0,1 -t../test_data/operation_lists/clustered_3d_pquery.ops -p../test_data/datasets/clustered_3d.dat --index_structures=pyramid_tree,3,0,1,6000
timeout 10m ../build/evaluator --runs=2 --output=results/times_pquery_clustered_5d.txt --index_structures=sequential_scan,5 --index_structures=octree,5,0,1 --index_structures=pyramid_tree,5,0,1 -t../test_data/operation_lists/clustered_5d_pquery.ops -p../test_data/datasets/clustered_5d.dat --index_structures=pyramid_tree,5,0,1,6000
timeout 10m ../build/evaluator --runs=2 --output=results/times_pquery_clustered_8d.txt --index_structures=sequential_scan,8 --index_structures=octree,8,0,1 --index_structures=pyramid_tree,8,0,1 -t../test_data/operation_lists/clustered_8d_pquery.ops -p../test_data/datasets/clustered_8d.dat --index_structures=pyramid_tree,8,0,1,6000
timeout 10m ../build/evaluator --runs=2 --output=results/times_pquery_clustered_10d.txt --index_structures=sequential_scan,10 --index_structures=octree,10,0,1 --index_structures=pyramid_tree,10,0,1 -t../test_data/operation_lists/clustered_10d_pquery.ops -p../test_data/datasets/clustered_10d.dat --index_structures=pyramid_tree,10,0,1,6000
timeout 10m ../build/evaluator --runs=2 --output=results/times_pquery_clustered_30d.txt --index_structures=sequential_scan,30 --index_structures=octree,30,0,1 --index_structures=pyramid_tree,30,0,1 -t../test_data/operation_lists/clustered_30d_pquery.ops -p../test_data/datasets/clustered_30d.dat --index_structures=pyramid_tree,30,0,1,6000
timeout 10m ../build/evaluator --runs=2 --output=results/times_pquery_clustered_50d.txt --index_structures=sequential_scan,50 --index_structures=octree,50,0,1 --index_structures=pyramid_tree,50,0,1 -t../test_data/operation_lists/clustered_50d_pquery.ops -p../test_data/datasets/clustered_50d.dat --index_structures=pyramid_tree,50,0,1,6000
timeout 10m ../build/evaluator --runs=2 --output=results/times_pquery_clustered_100d.txt --index_structures=sequential_scan,100 --index_structures=octree,100,0,1 --index_structures=pyramid_tree,100,0,1 -t../test_data/operation_lists/clustered_100d_pquery.ops -p../test_data/datasets/clustered_100d.dats --index_structures=pyramid_tree,100,0,1,6000
timeout 10m ../build/evaluator --runs=2 --output=results/times_pquery_clustered_200d.txt --index_structures=sequential_scan,200 --index_structures=octree,200,0,1 --index_structures=pyramid_tree,200,0,1 -t../test_data/operation_lists/clustered_200d_pquery.ops -p../test_data/datasets/clustered_200d.dat --index_structures=pyramid_tree,200,0,1,6000




# ALL OPERATIONS
timeout 10m ../build/evaluator --runs=2 --output=results/times_1d.txt --index_structures=sequential_scan,1 --index_structures=octree,1,0,1 --index_structures=pyramid_tree,1,0,1 -t../test_data/operation_lists/randuniform_1d.ops -t../test_data/operation_lists/skewed_1d.ops -t../test_data/operation_lists/randuniform_1d.ops --index_structures=pyramid_tree,1,0,1,6000
timeout 10m ../build/evaluator --runs=2 --output=results/times_2d.txt --index_structures=sequential_scan,2 --index_structures=octree,2,0,1 --index_structures=pyramid_tree,2,0,1 -t../test_data/operation_lists/randuniform_2d.ops -t../test_data/operation_lists/skewed_2d.ops -t../test_data/operation_lists/randuniform_2d.ops --index_structures=pyramid_tree,2,0,1,6000
timeout 10m ../build/evaluator --runs=2 --output=results/times_3d.txt --index_structures=sequential_scan,3 --index_structures=octree,3,0,1 --index_structures=pyramid_tree,3,0,1 -t../test_data/operation_lists/randuniform_3d.ops -t../test_data/operation_lists/skewed_3d.ops -t../test_data/operation_lists/randuniform_3d.ops --index_structures=pyramid_tree,3,0,1,6000
timeout 10m ../build/evaluator --runs=2 --output=results/times_5d.txt --index_structures=sequential_scan,5 --index_structures=octree,5,0,1 --index_structures=pyramid_tree,5,0,1 -t../test_data/operation_lists/randuniform_5d.ops -t../test_data/operation_lists/skewed_5d.ops -t../test_data/operation_lists/randuniform_5d.ops --index_structures=pyramid_tree,5,0,1,6000
timeout 10m ../build/evaluator --runs=2 --output=results/times_8d.txt --index_structures=sequential_scan,8 --index_structures=octree,8,0,1 --index_structures=pyramid_tree,8,0,1 -t../test_data/operation_lists/randuniform_8d.ops -t../test_data/operation_lists/skewed_8d.ops -t../test_data/operation_lists/randuniform_8d.ops --index_structures=pyramid_tree,8,0,1,6000
timeout 10m ../build/evaluator --runs=2 --output=results/times_10d.txt --index_structures=sequential_scan,10 --index_structures=octree,10,0,1 --index_structures=pyramid_tree,10,0,1 -t../test_data/operation_lists/randuniform_10d.ops -t../test_data/operation_lists/skewed_10d.ops -t../test_data/operation_lists/randuniform_10d.ops --index_structures=pyramid_tree,10,0,1,6000
timeout 10m ../build/evaluator --runs=2 --output=results/times_30d.txt --index_structures=sequential_scan,30 --index_structures=octree,30,0,1 --index_structures=pyramid_tree,30,0,1 -t../test_data/operation_lists/randuniform_30d.ops -t../test_data/operation_lists/skewed_30d.ops -t../test_data/operation_lists/randuniform_30d.ops --index_structures=pyramid_tree,30,0,1,6000
timeout 10m ../build/evaluator --runs=2 --output=results/times_50d.txt --index_structures=sequential_scan,50 --index_structures=octree,50,0,1 --index_structures=pyramid_tree,50,0,1 -t../test_data/operation_lists/randuniform_50d.ops -t../test_data/operation_lists/skewed_50d.ops -t../test_data/operation_lists/randuniform_50d.ops --index_structures=pyramid_tree,50,0,1,6000
timeout 10m ../build/evaluator --runs=2 --output=results/times_100d.txt --index_structures=sequential_scan,100 --index_structures=octree,100,0,1 --index_structures=pyramid_tree,100,0,1 -t../test_data/operation_lists/randuniform_100d.ops -t../test_data/operation_lists/skewed_100d.ops -t../test_data/operation_lists/randuniform_100d.ops --index_structures=pyramid_tree,100,0,1,6000
timeout 10m ../build/evaluator --runs=2 --output=results/times_200d.txt --index_structures=sequential_scan,200 --index_structures=octree,200,0,1 --index_structures=pyramid_tree,200,0,1 -t../test_data/operation_lists/randuniform_200d.ops -t../test_data/operation_lists/skewed_200d.ops -t../test_data/operation_lists/randuniform_200d.ops --index_structures=pyramid_tree,200,0,1,6000

# RANDOM OPERATIONS
timeout 10m ../build/evaluator --runs=2 --output=results/times_1d_random.txt --index_structures=sequential_scan,1 --index_structures=octree,1,0,1 --index_structures=pyramid_tree,1,0,1 -t../test_data/operation_lists/randuniform_1d_random.ops -t../test_data/operation_lists/skewed_1d_random.ops -t../test_data/operation_lists/randuniform_1d_random.ops --index_structures=pyramid_tree,1,0,1,6000
timeout 10m ../build/evaluator --runs=2 --output=results/times_2d_random.txt --index_structures=sequential_scan,2 --index_structures=octree,2,0,1 --index_structures=pyramid_tree,2,0,1 -t../test_data/operation_lists/randuniform_2d_random.ops -t../test_data/operation_lists/skewed_2d_random.ops -t../test_data/operation_lists/randuniform_2d_random.ops --index_structures=pyramid_tree,2,0,1,6000
timeout 10m ../build/evaluator --runs=2 --output=results/times_3d_random.txt --index_structures=sequential_scan,3 --index_structures=octree,3,0,1 --index_structures=pyramid_tree,3,0,1 -t../test_data/operation_lists/randuniform_3d_random.ops -t../test_data/operation_lists/skewed_3d_random.ops -t../test_data/operation_lists/randuniform_3d_random.ops --index_structures=pyramid_tree,3,0,1,6000
timeout 10m ../build/evaluator --runs=2 --output=results/times_5d_random.txt --index_structures=sequential_scan,5 --index_structures=octree,5,0,1 --index_structures=pyramid_tree,5,0,1 -t../test_data/operation_lists/randuniform_5d_random.ops -t../test_data/operation_lists/skewed_5d_random.ops -t../test_data/operation_lists/randuniform_5d_random.ops --index_structures=pyramid_tree,5,0,1,6000
timeout 10m ../build/evaluator --runs=2 --output=results/times_8d_random.txt --index_structures=sequential_scan,8 --index_structures=octree,8,0,1 --index_structures=pyramid_tree,8,0,1 -t../test_data/operation_lists/randuniform_8d_random.ops -t../test_data/operation_lists/skewed_8d_random.ops -t../test_data/operation_lists/randuniform_8d_random.ops --index_structures=pyramid_tree,8,0,1,6000
timeout 10m ../build/evaluator --runs=2 --output=results/times_10d_random.txt --index_structures=sequential_scan,10 --index_structures=octree,10,0,1 --index_structures=pyramid_tree,10,0,1 -t../test_data/operation_lists/randuniform_10d_random.ops -t../test_data/operation_lists/skewed_10d_random.ops -t../test_data/operation_lists/randuniform_10d_random.ops --index_structures=pyramid_tree,10,0,1,6000
timeout 10m ../build/evaluator --runs=2 --output=results/times_30d_random.txt --index_structures=sequential_scan,30 --index_structures=octree,30,0,1 --index_structures=pyramid_tree,30,0,1 -t../test_data/operation_lists/randuniform_30d_random.ops -t../test_data/operation_lists/skewed_30d_random.ops -t../test_data/operation_lists/randuniform_30d_random.ops --index_structures=pyramid_tree,30,0,1,6000
timeout 10m ../build/evaluator --runs=2 --output=results/times_50d_random.txt --index_structures=sequential_scan,50 --index_structures=octree,50,0,1 --index_structures=pyramid_tree,50,0,1 -t../test_data/operation_lists/randuniform_50d_random.ops -t../test_data/operation_lists/skewed_50d_random.ops -t../test_data/operation_lists/randuniform_50d_random.ops --index_structures=pyramid_tree,50,0,1,6000
timeout 10m ../build/evaluator --runs=2 --output=results/times_100d_random.txt --index_structures=sequential_scan,100 --index_structures=octree,100,0,1 --index_structures=pyramid_tree,100,0,1 -t../test_data/operation_lists/randuniform_100d_random.ops -t../test_data/operation_lists/skewed_100d_random.ops -t../test_data/operation_lists/randuniform_100d_random.ops --index_structures=pyramid_tree,100,0,1,6000
timeout 10m ../build/evaluator --runs=2 --output=results/times_200d_random.txt --index_structures=sequential_scan,200 --index_structures=octree,200,0,1 --index_structures=pyramid_tree,200,0,1 -t../test_data/operation_lists/randuniform_200d_random.ops -t../test_data/operation_lists/skewed_200d_random.ops -t../test_data/operation_lists/randuniform_200d_random.ops --index_structures=pyramid_tree,200,0,1,6000

# ASTROPHYSICS DATASET (ALL AND RANDOM)
timeout 10m ../build/evaluator --runs=2 --output=results/times_astrophysics.txt --index_structures=sequential_scan,10 --index_structures=octree,10,0,1 --index_structures=pyramid_tree,10,0,1 -t../test_data/operation_lists/multifield.0099_75x31x31.ops --index_structures=pyramid_tree,10,0,1,6000
timeout 10m ../build/evaluator --runs=2 --output=results/times_astrophysics_random.txt --index_structures=sequential_scan,10 --index_structures=octree,10,0,1 --index_structures=pyramid_tree,10,0,1 -t../test_data/operation_lists/multifield.0099_75x31x31_random.ops --index_structures=pyramid_tree,10,0,1,6000