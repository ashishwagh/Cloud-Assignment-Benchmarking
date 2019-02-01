#!/bin/bash
#SBATCH --nodes=1
#SBATCH --output=MyDiskBench_RS_100000_4.out
./MyDiskBench disk-RS-100000-4thread.dat
