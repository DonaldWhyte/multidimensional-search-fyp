#!/bin/sh

python generate_dataset_operations.py ../test_data/datasets/skewed_1d.dat insert > ../test_data/operation_lists/skewed_1d_insert.ops
python generate_dataset_operations.py ../test_data/datasets/skewed_2d.dat insert > ../test_data/operation_lists/skewed_2d_insert.ops
python generate_dataset_operations.py ../test_data/datasets/skewed_3d.dat insert > ../test_data/operation_lists/skewed_3d_insert.ops
python generate_dataset_operations.py ../test_data/datasets/skewed_5d.dat insert > ../test_data/operation_lists/skewed_5d_insert.ops
python generate_dataset_operations.py ../test_data/datasets/skewed_8d.dat insert > ../test_data/operation_lists/skewed_8d_insert.ops
python generate_dataset_operations.py ../test_data/datasets/skewed_10d.dat insert > ../test_data/operation_lists/skewed_10d_insert.ops
python generate_dataset_operations.py ../test_data/datasets/skewed_30d.dat insert > ../test_data/operation_lists/skewed_30d_insert.ops
python generate_dataset_operations.py ../test_data/datasets/skewed_50d.dat insert > ../test_data/operation_lists/skewed_50d_insert.ops
python generate_dataset_operations.py ../test_data/datasets/skewed_100d.dat insert > ../test_data/operation_lists/skewed_100d_insert.ops
python generate_dataset_operations.py ../test_data/datasets/skewed_200d.dat insert > ../test_data/operation_lists/skewed_200d_insert.ops

python generate_dataset_operations.py ../test_data/datasets/clustered_1d.dat insert > ../test_data/operation_lists/clustered_1d_insert.ops
python generate_dataset_operations.py ../test_data/datasets/clustered_2d.dat insert > ../test_data/operation_lists/clustered_2d_insert.ops
python generate_dataset_operations.py ../test_data/datasets/clustered_3d.dat insert > ../test_data/operation_lists/clustered_3d_insert.ops
python generate_dataset_operations.py ../test_data/datasets/clustered_5d.dat insert > ../test_data/operation_lists/clustered_5d_insert.ops
python generate_dataset_operations.py ../test_data/datasets/clustered_8d.dat insert > ../test_data/operation_lists/clustered_8d_insert.ops
python generate_dataset_operations.py ../test_data/datasets/clustered_10d.dat insert > ../test_data/operation_lists/clustered_10d_insert.ops
python generate_dataset_operations.py ../test_data/datasets/clustered_30d.dat insert > ../test_data/operation_lists/clustered_30d_insert.ops
python generate_dataset_operations.py ../test_data/datasets/clustered_50d.dat insert > ../test_data/operation_lists/clustered_50d_insert.ops
python generate_dataset_operations.py ../test_data/datasets/clustered_100d.dat insert > ../test_data/operation_lists/clustered_100d_insert.ops
python generate_dataset_operations.py ../test_data/datasets/clustered_200d.dat insert > ../test_data/operation_lists/clustered_200d_insert.ops

python generate_dataset_operations.py ../test_data/datasets/randuniform_1d.dat insert > ../test_data/operation_lists/randuniform_1d_insert.ops
python generate_dataset_operations.py ../test_data/datasets/randuniform_2d.dat insert > ../test_data/operation_lists/randuniform_2d_insert.ops
python generate_dataset_operations.py ../test_data/datasets/randuniform_3d.dat insert > ../test_data/operation_lists/randuniform_3d_insert.ops
python generate_dataset_operations.py ../test_data/datasets/randuniform_5d.dat insert > ../test_data/operation_lists/randuniform_5d_insert.ops
python generate_dataset_operations.py ../test_data/datasets/randuniform_8d.dat insert > ../test_data/operation_lists/randuniform_8d_insert.ops
python generate_dataset_operations.py ../test_data/datasets/randuniform_10d.dat insert > ../test_data/operation_lists/randuniform_10d_insert.ops
python generate_dataset_operations.py ../test_data/datasets/randuniform_30d.dat insert > ../test_data/operation_lists/randuniform_30d_insert.ops
python generate_dataset_operations.py ../test_data/datasets/randuniform_50d.dat insert > ../test_data/operation_lists/randuniform_50d_insert.ops
python generate_dataset_operations.py ../test_data/datasets/randuniform_100d.dat insert > ../test_data/operation_lists/randuniform_100d_insert.ops
python generate_dataset_operations.py ../test_data/datasets/randuniform_200d.dat insert > ../test_data/operation_lists/randuniform_200d_insert.ops

python generate_dataset_operations.py ../test_data/datasets/skewed_1d.dat delete > ../test_data/operation_lists/skewed_1d_delete.ops
python generate_dataset_operations.py ../test_data/datasets/skewed_2d.dat delete > ../test_data/operation_lists/skewed_2d_delete.ops
python generate_dataset_operations.py ../test_data/datasets/skewed_3d.dat delete > ../test_data/operation_lists/skewed_3d_delete.ops
python generate_dataset_operations.py ../test_data/datasets/skewed_5d.dat delete > ../test_data/operation_lists/skewed_5d_delete.ops
python generate_dataset_operations.py ../test_data/datasets/skewed_8d.dat delete > ../test_data/operation_lists/skewed_8d_delete.ops
python generate_dataset_operations.py ../test_data/datasets/skewed_10d.dat delete > ../test_data/operation_lists/skewed_10d_delete.ops
python generate_dataset_operations.py ../test_data/datasets/skewed_30d.dat delete > ../test_data/operation_lists/skewed_30d_delete.ops
python generate_dataset_operations.py ../test_data/datasets/skewed_50d.dat delete > ../test_data/operation_lists/skewed_50d_delete.ops
python generate_dataset_operations.py ../test_data/datasets/skewed_100d.dat delete > ../test_data/operation_lists/skewed_100d_delete.ops
python generate_dataset_operations.py ../test_data/datasets/skewed_200d.dat delete > ../test_data/operation_lists/skewed_200d_delete.ops

python generate_dataset_operations.py ../test_data/datasets/clustered_1d.dat delete > ../test_data/operation_lists/clustered_1d_delete.ops
python generate_dataset_operations.py ../test_data/datasets/clustered_2d.dat delete > ../test_data/operation_lists/clustered_2d_delete.ops
python generate_dataset_operations.py ../test_data/datasets/clustered_3d.dat delete > ../test_data/operation_lists/clustered_3d_delete.ops
python generate_dataset_operations.py ../test_data/datasets/clustered_5d.dat delete > ../test_data/operation_lists/clustered_5d_delete.ops
python generate_dataset_operations.py ../test_data/datasets/clustered_8d.dat delete > ../test_data/operation_lists/clustered_8d_delete.ops
python generate_dataset_operations.py ../test_data/datasets/clustered_10d.dat delete > ../test_data/operation_lists/clustered_10d_delete.ops
python generate_dataset_operations.py ../test_data/datasets/clustered_30d.dat delete > ../test_data/operation_lists/clustered_30d_delete.ops
python generate_dataset_operations.py ../test_data/datasets/clustered_50d.dat delete > ../test_data/operation_lists/clustered_50d_delete.ops
python generate_dataset_operations.py ../test_data/datasets/clustered_100d.dat delete > ../test_data/operation_lists/clustered_100d_delete.ops
python generate_dataset_operations.py ../test_data/datasets/clustered_200d.dat delete > ../test_data/operation_lists/clustered_200d_delete.ops

python generate_dataset_operations.py ../test_data/datasets/randuniform_1d.dat delete > ../test_data/operation_lists/randuniform_1d_delete.ops
python generate_dataset_operations.py ../test_data/datasets/randuniform_2d.dat delete > ../test_data/operation_lists/randuniform_2d_delete.ops
python generate_dataset_operations.py ../test_data/datasets/randuniform_3d.dat delete > ../test_data/operation_lists/randuniform_3d_delete.ops
python generate_dataset_operations.py ../test_data/datasets/randuniform_5d.dat delete > ../test_data/operation_lists/randuniform_5d_delete.ops
python generate_dataset_operations.py ../test_data/datasets/randuniform_8d.dat delete > ../test_data/operation_lists/randuniform_8d_delete.ops
python generate_dataset_operations.py ../test_data/datasets/randuniform_10d.dat delete > ../test_data/operation_lists/randuniform_10d_delete.ops
python generate_dataset_operations.py ../test_data/datasets/randuniform_30d.dat delete > ../test_data/operation_lists/randuniform_30d_delete.ops
python generate_dataset_operations.py ../test_data/datasets/randuniform_50d.dat delete > ../test_data/operation_lists/randuniform_50d_delete.ops
python generate_dataset_operations.py ../test_data/datasets/randuniform_100d.dat delete > ../test_data/operation_lists/randuniform_100d_delete.ops
python generate_dataset_operations.py ../test_data/datasets/randuniform_200d.dat delete > ../test_data/operation_lists/randuniform_200d_delete.ops

python generate_dataset_operations.py ../test_data/datasets/skewed_1d.dat pquery > ../test_data/operation_lists/skewed_1d_pquery.ops
python generate_dataset_operations.py ../test_data/datasets/skewed_2d.dat pquery > ../test_data/operation_lists/skewed_2d_pquery.ops
python generate_dataset_operations.py ../test_data/datasets/skewed_3d.dat pquery > ../test_data/operation_lists/skewed_3d_pquery.ops
python generate_dataset_operations.py ../test_data/datasets/skewed_5d.dat pquery > ../test_data/operation_lists/skewed_5d_pquery.ops
python generate_dataset_operations.py ../test_data/datasets/skewed_8d.dat pquery > ../test_data/operation_lists/skewed_8d_pquery.ops
python generate_dataset_operations.py ../test_data/datasets/skewed_10d.dat pquery > ../test_data/operation_lists/skewed_10d_pquery.ops
python generate_dataset_operations.py ../test_data/datasets/skewed_30d.dat pquery > ../test_data/operation_lists/skewed_30d_pquery.ops
python generate_dataset_operations.py ../test_data/datasets/skewed_50d.dat pquery > ../test_data/operation_lists/skewed_50d_pquery.ops
python generate_dataset_operations.py ../test_data/datasets/skewed_100d.dat pquery > ../test_data/operation_lists/skewed_100d_pquery.ops
python generate_dataset_operations.py ../test_data/datasets/skewed_200d.dat pquery > ../test_data/operation_lists/skewed_200d_pquery.ops

python generate_dataset_operations.py ../test_data/datasets/clustered_1d.dat pquery > ../test_data/operation_lists/clustered_1d_pquery.ops
python generate_dataset_operations.py ../test_data/datasets/clustered_2d.dat pquery > ../test_data/operation_lists/clustered_2d_pquery.ops
python generate_dataset_operations.py ../test_data/datasets/clustered_3d.dat pquery > ../test_data/operation_lists/clustered_3d_pquery.ops
python generate_dataset_operations.py ../test_data/datasets/clustered_5d.dat pquery > ../test_data/operation_lists/clustered_5d_pquery.ops
python generate_dataset_operations.py ../test_data/datasets/clustered_8d.dat pquery > ../test_data/operation_lists/clustered_8d_pquery.ops
python generate_dataset_operations.py ../test_data/datasets/clustered_10d.dat pquery > ../test_data/operation_lists/clustered_10d_pquery.ops
python generate_dataset_operations.py ../test_data/datasets/clustered_30d.dat pquery > ../test_data/operation_lists/clustered_30d_pquery.ops
python generate_dataset_operations.py ../test_data/datasets/clustered_50d.dat pquery > ../test_data/operation_lists/clustered_50d_pquery.ops
python generate_dataset_operations.py ../test_data/datasets/clustered_100d.dat pquery > ../test_data/operation_lists/clustered_100d_pquery.ops
python generate_dataset_operations.py ../test_data/datasets/clustered_200d.dat pquery > ../test_data/operation_lists/clustered_200d_pquery.ops

python generate_dataset_operations.py ../test_data/datasets/randuniform_1d.dat pquery > ../test_data/operation_lists/randuniform_1d_pquery.ops
python generate_dataset_operations.py ../test_data/datasets/randuniform_2d.dat pquery > ../test_data/operation_lists/randuniform_2d_pquery.ops
python generate_dataset_operations.py ../test_data/datasets/randuniform_3d.dat pquery > ../test_data/operation_lists/randuniform_3d_pquery.ops
python generate_dataset_operations.py ../test_data/datasets/randuniform_5d.dat pquery > ../test_data/operation_lists/randuniform_5d_pquery.ops
python generate_dataset_operations.py ../test_data/datasets/randuniform_8d.dat pquery > ../test_data/operation_lists/randuniform_8d_pquery.ops
python generate_dataset_operations.py ../test_data/datasets/randuniform_10d.dat pquery > ../test_data/operation_lists/randuniform_10d_pquery.ops
python generate_dataset_operations.py ../test_data/datasets/randuniform_30d.dat pquery > ../test_data/operation_lists/randuniform_30d_pquery.ops
python generate_dataset_operations.py ../test_data/datasets/randuniform_50d.dat pquery > ../test_data/operation_lists/randuniform_50d_pquery.ops
python generate_dataset_operations.py ../test_data/datasets/randuniform_100d.dat pquery > ../test_data/operation_lists/randuniform_100d_pquery.ops
python generate_dataset_operations.py ../test_data/datasets/randuniform_200d.dat pquery > ../test_data/operation_lists/randuniform_200d_pquery.ops