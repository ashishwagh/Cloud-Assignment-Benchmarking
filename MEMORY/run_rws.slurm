#!/bin/bash
#SBATCH --nodes=1
#SBATCH --output=MyRAMBench_rws_1_4.out
./MyRAMBench memory-RWS-1-4thread.dat
