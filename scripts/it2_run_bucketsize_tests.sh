#!/bin/sh -r2

inputDirectory="/usr/not-backed-up/DonaldFYP/test_data"
outputDirectory="/usr/not-backed-up/DonaldFYP/results/end_of_iteration2/bucket_size"

mkdir "$outputDirectory" -p

# ======== DELETE ========

# RANDOMLY UNIFORM DATA WITH VARYING DIMENSIONS -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -skdtree -t $inputDirectory/operation_lists/randuniform_1d_delete.ops -p$inputDirectory/datasets/randuniform_1d.dat > $outputDirectory/times_randuniform_1d_delete.txt -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/randuniform_2d_delete.ops -p$inputDirectory/datasets/randuniform_2d.dat > $outputDirectory/times_randuniform_2d_delete.txt -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/randuniform_3d_delete.ops -p$inputDirectory/datasets/randuniform_3d.dat > $outputDirectory/times_randuniform_3d_delete.txt -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/randuniform_5d_delete.ops -p$inputDirectory/datasets/randuniform_5d.dat > $outputDirectory/times_randuniform_5d_delete.txt -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/randuniform_8d_delete.ops -p$inputDirectory/datasets/randuniform_8d.dat > $outputDirectory/times_randuniform_8d_delete.txt -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/randuniform_10d_delete.ops -p$inputDirectory/datasets/randuniform_10d.dat > $outputDirectory/times_randuniform_10d_delete.txt -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/randuniform_30d_delete.ops -p$inputDirectory/datasets/randuniform_30d.dat > $outputDirectory/times_randuniform_50d_delete.txt -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/randuniform_50d_delete.ops -p$inputDirectory/datasets/randuniform_50d.dat > $outputDirectory/times_randuniform_100d_delete.txt -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/randuniform_100d_delete.ops -p$inputDirectory/datasets/randuniform_100d.dat > $outputDirectory/times_randuniform_200d_delete.txt -r2

# CLUSTERED UNIFORM DATA WITH VARYING DIMENSIONS -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/skewed_1d_delete.ops -p$inputDirectory/datasets/skewed_1d.dat > $outputDirectory/times_skewed_1d_delete.txt -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/skewed_2d_delete.ops -p$inputDirectory/datasets/skewed_2d.dat > $outputDirectory/times_skewed_2d_delete.txt -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/skewed_3d_delete.ops -p$inputDirectory/datasets/skewed_3d.dat > $outputDirectory/times_skewed_3d_delete.txt -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/skewed_5d_delete.ops -p$inputDirectory/datasets/skewed_5d.dat > $outputDirectory/times_skewed_5d_delete.txt -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/skewed_8d_delete.ops -p$inputDirectory/datasets/skewed_8d.dat > $outputDirectory/times_skewed_8d_delete.txt -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/skewed_10d_delete.ops -p$inputDirectory/datasets/skewed_10d.dat > $outputDirectory/times_skewed_10d_delete.txt -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/skewed_30d_delete.ops -p$inputDirectory/datasets/skewed_30d.dat > $outputDirectory/times_skewed_50d_delete.txt -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/skewed_50d_delete.ops -p$inputDirectory/datasets/skewed_50d.dat > $outputDirectory/times_skewed_100d_delete.txt -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/skewed_100d_delete.ops -p$inputDirectory/datasets/skewed_100d.dat > $outputDirectory/times_skewed_200d_delete.txt -r2

# SKEWED UNIFORM DATA WITH VARYING DIMENSIONS -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/clustered_1d_delete.ops -p$inputDirectory/datasets/clustered_1d.dat > $outputDirectory/times_clustered_1d_delete.txt -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/clustered_2d_delete.ops -p$inputDirectory/datasets/clustered_2d.dat > $outputDirectory/times_clustered_2d_delete.txt -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/clustered_3d_delete.ops -p$inputDirectory/datasets/clustered_3d.dat > $outputDirectory/times_clustered_3d_delete.txt -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/clustered_5d_delete.ops -p$inputDirectory/datasets/clustered_5d.dat > $outputDirectory/times_clustered_5d_delete.txt -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/clustered_8d_delete.ops -p$inputDirectory/datasets/clustered_8d.dat > $outputDirectory/times_clustered_8d_delete.txt -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/clustered_10d_delete.ops -p$inputDirectory/datasets/clustered_10d.dat > $outputDirectory/times_clustered_10d_delete.txt -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/clustered_30d_delete.ops -p$inputDirectory/datasets/clustered_30d.dat > $outputDirectory/times_clustered_50d_delete.txt -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/clustered_50d_delete.ops -p$inputDirectory/datasets/clustered_50d.dat > $outputDirectory/times_clustered_100d_delete.txt -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/clustered_100d_delete.ops -p$inputDirectory/datasets/clustered_100d.dat > $outputDirectory/times_clustered_200d_delete.txt -r2

# RANDOMLY UNIFORM DATA WITH VARYING SIZE -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/sizevary_n10_delete.ops -p$inputDirectory/datasets/sizevary_n10.dat > $outputDirectory/times_sizevary_n10_delete.txt -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/sizevary_n100_delete.ops -p$inputDirectory/datasets/sizevary_n100.dat > $outputDirectory/times_sizevary_n100_delete.txt -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/sizevary_n1000_delete.ops -p$inputDirectory/datasets/sizevary_n1000.dat > $outputDirectory/times_sizevary_n1000_delete.txt -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/sizevary_n5000_delete.ops -p$inputDirectory/datasets/sizevary_n5000.dat > $outputDirectory/times_sizevary_n5000_delete.txt -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/sizevary_n10000_delete.ops -p$inputDirectory/datasets/sizevary_n10000.dat > $outputDirectory/times_sizevary_n10000_delete.txt -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/sizevary_n50000_delete.ops -p$inputDirectory/datasets/sizevary_n50000.dat > $outputDirectory/times_sizevary_n50000_delete.txt -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/sizevary_n100000_delete.ops -p$inputDirectory/datasets/sizevary_n100000.dat > $outputDirectory/times_sizevary_n100000_delete.txt -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/sizevary_n500000_delete.ops -p$inputDirectory/datasets/sizevary_n500000.dat > $outputDirectory/times_sizevary_n500000_delete.txt -r2

# ASTROPHYSICS DATA WITH VARYING SIZE -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/astrophysics_10000_delete.ops -p$inputDirectory/datasets/astrophysics_10000.dat > $outputDirectory/times_astrophysics_10000_delete.txt -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/astrophysics_100000_delete.ops -p$inputDirectory/datasets/astrophysics_100000.dat > $outputDirectory/times_astrophysics_100000_delete.txt -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/astrophysics_500000_delete.ops -p$inputDirectory/datasets/astrophysics_500000.dat > $outputDirectory/times_astrophysics_500000_delete.txt -r2

# ======== POINT QUERIES ========

# RANDOMLY UNIFORM DATA WITH VARYING DIMENSIONS -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -skdtree -t $inputDirectory/operation_lists/randuniform_1d_pquery.ops -p$inputDirectory/datasets/randuniform_1d.dat > $outputDirectory/times_randuniform_1d_pquery.txt -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/randuniform_2d_pquery.ops -p$inputDirectory/datasets/randuniform_2d.dat > $outputDirectory/times_randuniform_2d_pquery.txt -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/randuniform_3d_pquery.ops -p$inputDirectory/datasets/randuniform_3d.dat > $outputDirectory/times_randuniform_3d_pquery.txt -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/randuniform_5d_pquery.ops -p$inputDirectory/datasets/randuniform_5d.dat > $outputDirectory/times_randuniform_5d_pquery.txt -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/randuniform_8d_pquery.ops -p$inputDirectory/datasets/randuniform_8d.dat > $outputDirectory/times_randuniform_8d_pquery.txt -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/randuniform_10d_pquery.ops -p$inputDirectory/datasets/randuniform_10d.dat > $outputDirectory/times_randuniform_10d_pquery.txt -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/randuniform_30d_pquery.ops -p$inputDirectory/datasets/randuniform_30d.dat > $outputDirectory/times_randuniform_50d_pquery.txt -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/randuniform_50d_pquery.ops -p$inputDirectory/datasets/randuniform_50d.dat > $outputDirectory/times_randuniform_100d_pquery.txt -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/randuniform_100d_pquery.ops -p$inputDirectory/datasets/randuniform_100d.dat > $outputDirectory/times_randuniform_200d_pquery.txt -r2

# CLUSTERED UNIFORM DATA WITH VARYING DIMENSIONS -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/skewed_1d_pquery.ops -p$inputDirectory/datasets/skewed_1d.dat > $outputDirectory/times_skewed_1d_pquery.txt -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/skewed_2d_pquery.ops -p$inputDirectory/datasets/skewed_2d.dat > $outputDirectory/times_skewed_2d_pquery.txt -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/skewed_3d_pquery.ops -p$inputDirectory/datasets/skewed_3d.dat > $outputDirectory/times_skewed_3d_pquery.txt -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/skewed_5d_pquery.ops -p$inputDirectory/datasets/skewed_5d.dat > $outputDirectory/times_skewed_5d_pquery.txt -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/skewed_8d_pquery.ops -p$inputDirectory/datasets/skewed_8d.dat > $outputDirectory/times_skewed_8d_pquery.txt -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/skewed_10d_pquery.ops -p$inputDirectory/datasets/skewed_10d.dat > $outputDirectory/times_skewed_10d_pquery.txt -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/skewed_30d_pquery.ops -p$inputDirectory/datasets/skewed_30d.dat > $outputDirectory/times_skewed_50d_pquery.txt -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/skewed_50d_pquery.ops -p$inputDirectory/datasets/skewed_50d.dat > $outputDirectory/times_skewed_100d_pquery.txt -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/skewed_100d_pquery.ops -p$inputDirectory/datasets/skewed_100d.dat > $outputDirectory/times_skewed_200d_pquery.txt -r2

# SKEWED UNIFORM DATA WITH VARYING DIMENSIONS -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/clustered_1d_pquery.ops -p$inputDirectory/datasets/clustered_1d.dat > $outputDirectory/times_clustered_1d_pquery.txt -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/clustered_2d_pquery.ops -p$inputDirectory/datasets/clustered_2d.dat > $outputDirectory/times_clustered_2d_pquery.txt -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/clustered_3d_pquery.ops -p$inputDirectory/datasets/clustered_3d.dat > $outputDirectory/times_clustered_3d_pquery.txt -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/clustered_5d_pquery.ops -p$inputDirectory/datasets/clustered_5d.dat > $outputDirectory/times_clustered_5d_pquery.txt -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/clustered_8d_pquery.ops -p$inputDirectory/datasets/clustered_8d.dat > $outputDirectory/times_clustered_8d_pquery.txt -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/clustered_10d_pquery.ops -p$inputDirectory/datasets/clustered_10d.dat > $outputDirectory/times_clustered_10d_pquery.txt -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/clustered_30d_pquery.ops -p$inputDirectory/datasets/clustered_30d.dat > $outputDirectory/times_clustered_50d_pquery.txt -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/clustered_50d_pquery.ops -p$inputDirectory/datasets/clustered_50d.dat > $outputDirectory/times_clustered_100d_pquery.txt -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/clustered_100d_pquery.ops -p$inputDirectory/datasets/clustered_100d.dat > $outputDirectory/times_clustered_200d_pquery.txt -r2

# RANDOMLY UNIFORM DATA WITH VARYING SIZE -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/sizevary_n10_pquery.ops -p$inputDirectory/datasets/sizevary_n10.dat > $outputDirectory/times_sizevary_n10_pquery.txt -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/sizevary_n100_pquery.ops -p$inputDirectory/datasets/sizevary_n100.dat > $outputDirectory/times_sizevary_n100_pquery.txt -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/sizevary_n1000_pquery.ops -p$inputDirectory/datasets/sizevary_n1000.dat > $outputDirectory/times_sizevary_n1000_pquery.txt -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/sizevary_n5000_pquery.ops -p$inputDirectory/datasets/sizevary_n5000.dat > $outputDirectory/times_sizevary_n5000_pquery.txt -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/sizevary_n10000_pquery.ops -p$inputDirectory/datasets/sizevary_n10000.dat > $outputDirectory/times_sizevary_n10000_pquery.txt -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/sizevary_n50000_pquery.ops -p$inputDirectory/datasets/sizevary_n50000.dat > $outputDirectory/times_sizevary_n50000_pquery.txt -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/sizevary_n100000_pquery.ops -p$inputDirectory/datasets/sizevary_n100000.dat > $outputDirectory/times_sizevary_n100000_pquery.txt -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/sizevary_n500000_pquery.ops -p$inputDirectory/datasets/sizevary_n500000.dat > $outputDirectory/times_sizevary_n500000_pquery.txt -r2

# ASTROPHYSICS DATA WITH VARYING SIZE -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/astrophysics_10000_pquery.ops -p$inputDirectory/datasets/astrophysics_10000.dat > $outputDirectory/times_astrophysics_10000_pquery.txt -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/astrophysics_100000_pquery.ops -p$inputDirectory/datasets/astrophysics_100000.dat > $outputDirectory/times_astrophysics_100000_pquery.txt -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/astrophysics_500000_pquery.ops -p$inputDirectory/datasets/astrophysics_500000.dat > $outputDirectory/times_astrophysics_500000_pquery.txt -r2

# ======== INSERT ========

# RANDOMLY UNIFORM DATA WITH VARYING DIMENSIONS -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -skdtree -t $inputDirectory/operation_lists/randuniform_1d_insert.ops > $outputDirectory/times_randuniform_1d_insert.txt -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/randuniform_2d_insert.ops > $outputDirectory/times_randuniform_2d_insert.txt -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/randuniform_3d_insert.ops > $outputDirectory/times_randuniform_3d_insert.txt -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/randuniform_5d_insert.ops > $outputDirectory/times_randuniform_5d_insert.txt -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/randuniform_8d_insert.ops > $outputDirectory/times_randuniform_8d_insert.txt -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/randuniform_10d_insert.ops > $outputDirectory/times_randuniform_10d_insert.txt -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/randuniform_30d_insert.ops > $outputDirectory/times_randuniform_50d_insert.txt -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/randuniform_50d_insert.ops > $outputDirectory/times_randuniform_100d_insert.txt -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/randuniform_100d_insert.ops > $outputDirectory/times_randuniform_200d_insert.txt -r2

# CLUSTERED UNIFORM DATA WITH VARYING DIMENSIONS -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/skewed_1d_insert.ops > $outputDirectory/times_skewed_1d_insert.txt -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/skewed_2d_insert.ops > $outputDirectory/times_skewed_2d_insert.txt -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/skewed_3d_insert.ops > $outputDirectory/times_skewed_3d_insert.txt -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/skewed_5d_insert.ops > $outputDirectory/times_skewed_5d_insert.txt -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/skewed_8d_insert.ops > $outputDirectory/times_skewed_8d_insert.txt -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/skewed_10d_insert.ops > $outputDirectory/times_skewed_10d_insert.txt -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/skewed_30d_insert.ops > $outputDirectory/times_skewed_50d_insert.txt -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/skewed_50d_insert.ops > $outputDirectory/times_skewed_100d_insert.txt -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/skewed_100d_insert.ops > $outputDirectory/times_skewed_200d_insert.txt -r2

# SKEWED UNIFORM DATA WITH VARYING DIMENSIONS -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/clustered_1d_insert.ops > $outputDirectory/times_clustered_1d_insert.txt -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/clustered_2d_insert.ops > $outputDirectory/times_clustered_2d_insert.txt -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/clustered_3d_insert.ops > $outputDirectory/times_clustered_3d_insert.txt -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/clustered_5d_insert.ops > $outputDirectory/times_clustered_5d_insert.txt -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/clustered_8d_insert.ops > $outputDirectory/times_clustered_8d_insert.txt -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/clustered_10d_insert.ops > $outputDirectory/times_clustered_10d_insert.txt -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/clustered_30d_insert.ops > $outputDirectory/times_clustered_50d_insert.txt -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/clustered_50d_insert.ops > $outputDirectory/times_clustered_100d_insert.txt -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/clustered_100d_insert.ops > $outputDirectory/times_clustered_200d_insert.txt -r2

# RANDOMLY UNIFORM DATA WITH VARYING SIZE -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/sizevary_n10_insert.ops > $outputDirectory/times_sizevary_n10_insert.txt -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/sizevary_n100_insert.ops > $outputDirectory/times_sizevary_n100_insert.txt -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/sizevary_n1000_insert.ops > $outputDirectory/times_sizevary_n1000_insert.txt -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/sizevary_n5000_insert.ops > $outputDirectory/times_sizevary_n5000_insert.txt -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/sizevary_n10000_insert.ops > $outputDirectory/times_sizevary_n10000_insert.txt -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/sizevary_n50000_insert.ops > $outputDirectory/times_sizevary_n50000_insert.txt -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/sizevary_n100000_insert.ops > $outputDirectory/times_sizevary_n100000_insert.txt -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/sizevary_n500000_insert.ops > $outputDirectory/times_sizevary_n500000_insert.txt -r2

# ASTROPHYSICS DATA WITH VARYING SIZE -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/astrophysics_10000_insert.ops > $outputDirectory/times_astrophysics_10000_insert.txt -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/astrophysics_100000_insert.ops > $outputDirectory/times_astrophysics_100000_insert.txt -r2
../build/evaluator -spseudo_pyramid_tree -spyramid_tree -sbdh -sdht -skdtree -t $inputDirectory/operation_lists/astrophysics_500000_insert.ops > $outputDirectory/times_astrophysics_500000_insert.txt