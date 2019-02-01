#!/bin/sh
#SBATCH --nodes=1
#SBATCH --output=MyDiskBench_RR_1_128.out
./MyDiskBench disk-RR-1-128thread.dat

