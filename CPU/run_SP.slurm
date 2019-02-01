#!/bin/sh
#SBATCH --nodes=1
#SBATCH --output=MyCPUBench_SP_4.out
./MyCPUBench cpu_SP_4thread.dat
