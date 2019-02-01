#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
#include <fcntl.h>


typedef struct readwriteinfo{
	double start_block; 
	double end_block; 
	long long int block_size_value; 
	char* op_type; 
}readwriteinfo;

void *myRRFunc(readwriteinfo *a);
void *myRSFunc(readwriteinfo *b);
void *myWRFunc(readwriteinfo *c);
void *myWSFunc(readwriteinfo *d);
void *myRRFuncLat(readwriteinfo *e);
void *myWRFuncLat(readwriteinfo *d);

char filename[]="/tmp/dump";
char filename1[]="/tmp/dump1";
unsigned long int num_blocks_per_th;
const double FILE_SIZE = 10000000000.0;

//FILL ME IN
int main(int argc, char *argv[]) {

	char ch,opr[10],thread_size[10],blockvalue[10],opfname[50],myFunc[10];
	int NUM_THREADS,i,block_size;
	long long int loopNum=0;
	long long int k;
	clock_t start,stop;
	FILE *fp1,*fp2,*fp3;
	if(argv[1]!=NULL){
		fp1 = fopen(argv[1], "r");
	}
	
	while(fgets(opr, sizeof opr, fp1) != NULL){
		//fgets(opr,sizeof opr,(FILE*)fp1);
		fgets(blockvalue,sizeof blockvalue,(FILE*)fp1);
		fgets(thread_size,sizeof thread_size,(FILE*)fp1);
	}	

	for (i = 0; i < strlen(opr); i++)
        {
            if ( opr[i] == '\n')
                opr[i] = '\0';
        }
	//printf("hello%s",thread_size);
	sscanf(thread_size, "%d", &NUM_THREADS);
	
	if( access( "./output/DISKOUTPUT.txt",F_OK) != -1 ) {
		fp2 = fopen("./output/DISKOUTPUT.txt", "r+");
		fseek(fp2, 0, SEEK_END);
	}else{
		fp2 = fopen("./output/DISKOUTPUT.txt", "w");
		fprintf(fp2,"Workload \tConcurrency \tBlockSize \tMyDiskBenchThroughput \tTheoreticalThroughput \tEfficiency\n");		
	}
	
	if( access( "./output/DISKLATENCYOUTPUT.txt",F_OK) != -1 ) {
		fp3 = fopen("./output/DISKLATENCYOUTPUT.txt", "r+");
		fseek(fp3, 0, SEEK_END);
	}else{
		fp3 = fopen("./output/DISKLATENCYOUTPUT.txt", "w");
		fprintf(fp3,"Workload \tConcurrency \tBlockSize \tLatency \tIOPS\n");		
	}
	
	block_size=atoi(blockvalue);
	//printf("Block size%d",block_size);
	FILE *fp;
	
	strcpy(opfname,argv[1]);
	strcat(opfname,"_output.dat");

	char buff[1]={'c'};
	//printf("\nafter seek");		
	fp=fopen(filename,"w");

	fseek(fp,FILE_SIZE-1,SEEK_SET);

	for(k=0;k<FILE_SIZE/1000000;k++){
		fwrite(buff,sizeof(buff), 1000000,fp);
		fflush(fp);
	}
	
	fclose(fp);
	//printf("\nafter file create");
	
	readwriteinfo rwi[NUM_THREADS];
	unsigned long int num_blocks;
	
	if(block_size ==1){
		num_blocks = (FILE_SIZE/(10*block_size*1000));
	}else{
		num_blocks = FILE_SIZE/block_size;
	} 
	num_blocks_per_th = num_blocks/NUM_THREADS;
	
	pthread_t threads[NUM_THREADS]; 
	long tid; 
	int rc;
	
	//printf("%d",strncmp(prec,"QP",2));	
	
	start=clock();
	for (tid = 0; tid < NUM_THREADS; tid++) {
		
		rwi[tid].start_block=tid * num_blocks_per_th;
		rwi[tid].end_block=((tid+1) * num_blocks_per_th)-1;
		rwi[tid].block_size_value=block_size;
		rwi[tid].op_type=opr;
		
		if(strncmp(opr,"RR",2)==0){
			if(block_size == 1){
				rc = pthread_create(&threads[tid], NULL, myRRFuncLat, (void *) &rwi);
			}else{
				rc = pthread_create(&threads[tid], NULL, myRRFunc, (void *) &rwi);
			}
	
		}else if(strncmp(opr,"RS",2)==0){
			rc = pthread_create(&threads[tid], NULL, myRSFunc, (void *) &rwi);
		}else if(strncmp(opr,"WR",2)==0){
			if(block_size == 1){
				rc = pthread_create(&threads[tid], NULL, myWRFuncLat, (void *) &rwi);
			}else{
				rc = pthread_create(&threads[tid], NULL, myWRFunc, (void *) &rwi);
			}
		}else if(strncmp(opr,"WS",2)==0){
			rc = pthread_create(&threads[tid], NULL, myWSFunc, (void *) &rwi);
		}

		if (rc) {
			printf("Could not create thread %ld\n", tid);
		}
	}

	for (tid = 0; tid < NUM_THREADS; tid++) {
		rc = pthread_join(threads[tid], NULL);
		if (rc) {
		printf("Could not join thread %ld\n", tid);
		}
	}
	
	stop=clock();
	printf("\nTotal time elapsed:%f seconds\n",(float)(stop-start)/(CLOCKS_PER_SEC));
	double ex_time=(float)(stop-start)/(CLOCKS_PER_SEC);
	if(block_size ==1){
		double latency = (ex_time/1e6)*1e3;
		double iops = (1e9/ex_time)*1e3;
		fprintf(fp3,"%s\t\t%d\t\t%d\t\t%lf\t%lf\n",opr,NUM_THREADS,block_size,latency,iops);
	}else{
		double throughput = (FILE_SIZE*100 / ex_time)/1e9;
		double th_throughput=74000*block_size;
		//fprintf(fp2,"Workload \tConcurrency \tBlockSize \tMyRAMBenchValue \tTheoreticalValue \tMyRAMBenchEfficiency\n");

		fprintf(fp2,"%s\t\t%d\t\t%d\t\t%lf\t\t%llf\t\t%llf\n",opr,NUM_THREADS,block_size,throughput,th_throughput,(throughput/th_throughput)*100);
	}	
	fclose(fp1);
	fclose(fp2);
	fclose(fp3);
	return 0;
}

void *myRRFunc(readwriteinfo *rwi){

	unsigned long int i,ran_num,ran_block_num;
	
	int op_file;
	op_file = open(filename, O_RDWR);
	char* buff = malloc(rwi->block_size_value *  sizeof(char));
	for(i=rwi->start_block;i<=rwi->end_block;i++){
			ran_num = rand();
			ran_block_num = ((ran_num%num_blocks_per_th) + rwi->start_block);
			lseek(op_file,ran_block_num*rwi->block_size_value,SEEK_SET);
			read(op_file, buff, rwi->block_size_value);
	}
	free(buff);
    	close(op_file);
    	pthread_exit(NULL);

}

void *myRSFunc(readwriteinfo *rwi){

	unsigned long int i;
	int op_file;
	op_file = open(filename, O_RDWR);
	char* buff = malloc(rwi->block_size_value *  sizeof(char));
	for(i=rwi->start_block;i<=rwi->end_block;i++){
		lseek(op_file,i*rwi->block_size_value,SEEK_SET);
		read(op_file, buff, rwi->block_size_value);
	}
	free(buff);
    	close(op_file);
    	pthread_exit(NULL);

}

void *myWRFunc(readwriteinfo *rwi){

	unsigned long int i,ran_num,ran_block_num;
	
	int op_file;
	op_file = open(filename1, O_RDWR);
	char* buff = malloc(rwi->block_size_value *  sizeof(char));
	for(i=rwi->start_block;i<=rwi->end_block;i++){
		ran_num = rand();
		ran_block_num = ((ran_num%num_blocks_per_th) + rwi->start_block);
		lseek(op_file,ran_block_num*rwi->block_size_value,SEEK_SET);
		write(op_file, buff, rwi->block_size_value);
	}
	free(buff);
    	close(op_file);
    	pthread_exit(NULL);

}

void *myWSFunc(readwriteinfo *rwi){

	unsigned long int i;
	
	int op_file;
	op_file = open(filename, O_RDWR);
	char* buff = malloc(rwi->block_size_value *  sizeof(char));
	for(i=rwi->start_block;i<=rwi->end_block;i++){
		lseek(op_file,i*rwi->block_size_value,SEEK_SET);
		write(op_file, buff, rwi->block_size_value);
	}
	free(buff);
    	close(op_file);
    	pthread_exit(NULL);

}
void *myRRFuncLat(readwriteinfo *rwi){

	unsigned long int i,ran_num,ran_block_num;
	int op_file;
	op_file = open(filename, O_RDWR);
	char* buff = malloc(1000 *  sizeof(char));
	for(i=rwi->start_block;i<=rwi->end_block;i++){
		ran_num = rand();
		ran_block_num = ((ran_num%num_blocks_per_th) + rwi->start_block);
		lseek(op_file,ran_block_num*rwi->block_size_value*1000,SEEK_SET);
		read(op_file, buff, rwi->block_size_value);
	}
	free(buff);
    	close(op_file);
    	pthread_exit(NULL);

}

void *myWRFuncLat(readwriteinfo *rwi){

	unsigned long int i,ran_num,ran_block_num;
	
	int op_file;
	op_file = open(filename, O_RDWR);
	char* buff = malloc(rwi->block_size_value *  sizeof(char));
	for(i=rwi->start_block;i<=rwi->end_block;i++){
		ran_num = rand();
		ran_block_num = ((ran_num%num_blocks_per_th) + rwi->start_block);
		lseek(op_file,ran_block_num*rwi->block_size_value,SEEK_SET);
		write(op_file, buff, rwi->block_size_value);
	}
	free(buff);
    	close(op_file);
    	pthread_exit(NULL);

}
