#!/bin/bash
#SBATCH --nodes=1
#SBATCH --output=MyDiskBench_WS_100000_4.out
./MyDiskBench disk-WS-100000-4thread.dat

