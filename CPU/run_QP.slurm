#!/bin/sh
#SBATCH --nodes=1
#SBATCH --output=MyCPUBench_QP_4.out
./MyCPUBench cpu_QP_4thread.dat

