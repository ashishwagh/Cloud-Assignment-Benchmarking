#!/bin/sh
#SBATCH --nodes=1
#SBATCH --output=MyCPUBench_DP_2.out
./MyCPUBench cpu_DP_2thread.dat

