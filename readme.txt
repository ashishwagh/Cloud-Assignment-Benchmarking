*********************************************************************************

Readme file _PA1
*********************************************************************************

Login to the hyperion account using $ ssh .All code will be there in -pa1 folder.

*********************************************************************************
CPU Benchmarking
*********************************************************************************
Step 1:Go to cpu folder using $cd /-pa1 /cpu.
Step 2:Use $ vim MyCPUBench.c to view the source code.
Step 3:for comiling the code use $make all command.
Step 4:There will be files named run_DP.slurm,run_SP.slurm,run_QP.slurm,run_HP.slurm.Below are the sample contains of run_QP_slurm file
	#!/bin/sh
	#SBATCH --nodes=1
	#SBATCH --output=MyCPUBench_QP_4.out  -->output file name
	./MyCPUBench cpu_QP_4thread.dat  --> executable file name with input file name

Step 5:for generating the output file we need to change input file and output file name.
Step 6:use $sbatch run_QP.slurm command for scheduling the job.
Step 7:Go to output folder using $ cd output command.
Step 8:Output file will be generated by the name CPUOUTPUT.txt

*********************************************************************************
Memory Benchmarking
*********************************************************************************
Step 1:Go to cpu folder using $cd /-pa1 /memory.
Step 2:Use $ vim MyRAMBench.c to view the source code.
Step 3:for comiling the code use $make all command.
Step 4:There will be files named run_rwr.slurm,run_rws.slurm.Below are the sample contains of run_rwr.slurm file
	#!/bin/bash
	#SBATCH --nodes=1
	#SBATCH --output=MyRAMBench_rwr_1_4.out -->output file name
	./MyRAMBench memory-RWR-1-4thread.dat  --> executable file name with input file name

Step 5:for generating the output file we need to change input file and output file name.
Step 6:use $sbatch run_WR.slurm command for scheduling the job.
Step 7:Go to output folder using $ cd output command.
Step 8:Output file will be generated by the name RAMOUTPUT.txt for throughput and RAMLATENCYOUTPUT.txt for latency.

*********************************************************************************
Disk benchmarking
*********************************************************************************
Step 1:Go to cpu folder using $cd /-pa1 /disk.
Step 2:Use $ vim MyDiskBench.c to view the source code.
Step 3:for comiling the code use $make all command.
Step 4:There will be files named run_RR.slurm,run_RS.slurm,run_WR.slurm,run_WS.slurm.Below are the sample contains of run_RR.slurm file
	#!/bin/sh
	#SBATCH --nodes=1
	#SBATCH --output=MyDiskBench_RR_1_128.out  -->output file name
	./MyDiskBench disk-RR-1-128thread.dat   --> executable file name with input file name


Step 5:for generating the output file we need to change input file and output file name.
Step 6:use $sbatch run_RR.slurm command for scheduling the job.
Step 7:Go to output folder using $ cd output command.
Step 8:Output file will be generated by the name DISKOUTPUT.txt for throughput and DISKLATENCYOUTPUT.txt for latency.

*********************************************************************************
Network benchmarking
*********************************************************************************
Step 1:Go to cpu folder using $cd /-pa1/network.There will be separate files for client and server for TCP and UDP named MyNETBench-TCP-Client.c,MyNETBench-TCP-Server.c,MyNETBench-UDP-Client.c 
	and MyNETBench-UDP-Server.c.
Step 2:Use $ vim MyNETBench-TCP_server.c to view the source code.
Step 3:for comiling the code use $make all command.
Step 4:There will be files named run_TCP.slurm,run_UDP.slurm.Below are the sample contains of run_TCP.slurm file
	#!/bin/bash
	#SBATCH --nodes=2
	#SBATCH --ntasks=2
	#SBATCH --output=job.out
	#SBATCH --wait-all-nodes=1

	name=$(echo $SLURM_JOB_NODELIST | cut -d '-' -f1 -)
	node1=$(echo $SLURM_JOB_NODELIST | cut -d '-' -f2 - | tr -d '[')
	node2=$(echo $SLURM_JOB_NODELIST | cut -d '-' -f3 - | tr -d ']')
	if [ "$(hostname)" == "$name-$node1" ]
	then
	    ./MyNETBench-TCP_server ./network-TCP-32000-1thread.dat
	else
	    ./MyNETBench-TCP_client ./network-TCP-32000-1thread.dat $name-node1

	fi

Step 5:for generating the output file we need to change input file and output file name.
Step 6:use $sbatch run_TCP.slurm command for scheduling the job.
Step 7:Go to output folder using $ cd output command.
Step 8:Output file will be generated by the name RAMOUTPUT.txt for throughput and RAMLATENCYOUTPUT.txt for latency.

