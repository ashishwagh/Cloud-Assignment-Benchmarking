#!/bin/bash
#SBATCH --nodes=1
#SBATCH --output=MyDiskBench_WR_1_128.out
./MyDiskBench disk-WR-1-128thread.dat
