#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <math.h>
#include <sys/time.h>
#include <unistd.h>

const double DATA_SIZE = 1000000000.0;

char *src_data; 
char *dest_data;

typedef struct readwriteinfo{
	double start_block; 
	double end_block; 
	int block_size_value; 
	char* op_type; 
}readwriteinfo;

void *RWR(readwriteinfo *a);
void *RWS(readwriteinfo *b);
void *RWRLat(void *c);
void *RWSLat(void *d);


unsigned long int num_blocks_per_th;
//FILL ME IN
int main(int argc, char *argv[]) {

	char ch,buff[255],opr[10],thread_size[10],blockvalue[10],opfname[50],myFunc[10];
	int NUM_THREADS,i,block_size;
	long long int loopNum=0;
	clock_t start,stop;
	FILE *fp1,*fp2,*fp3;
	if(argv[1]!=NULL){
		fp1 = fopen(argv[1], "r");
	}
	//printf("%f",DATA_SIZE);
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
	//printf("helloooo opr:%s",opr);
	//printf("helloooo blockvalue:%s",blockvalue);
	//printf("helloooo thread_size:%s",thread_size);

	//sscanf(blockvalue,"%d",&block_size);
	block_size = atoi(blockvalue);
	//printf("qqqqq%d",block_size);

	sscanf(thread_size,"%d",&NUM_THREADS);
	//printf("qqqq%d",NUM_THREADS);
	
	strcpy(opfname,argv[1]);
	//printf("%s",argv[1]);
	strcat(opfname,"_output.dat");
	if( access( "./output/RAMOUTPUT.txt",F_OK) != -1 ) {
		fp2 = fopen("./output/RAMOUTPUT.txt", "r+");
		fseek(fp2, 0, SEEK_END);
	}else{
		fp2 = fopen("./output/RAMOUTPUT.txt", "w");
		fprintf(fp2,"Workload \tConcurrency \tBlockSize \tMyRAMBenchValue \tTheoreticalValue \tMyRAMBenchEfficiency\n");		
	}
	
	if( access( "./output/RAMLATENCYOUTPUT.txt",F_OK) != -1 ) {
		fp3 = fopen("./output/RAMLATENCYOUTPUT.txt", "r+");
		fseek(fp3, 0, SEEK_END);
	}else{
		fp3 = fopen("./output/RAMLATENCYOUTPUT.txt", "w");
		fprintf(fp3,"Workload \tConcurrency \tBlockSize \tLatency\n");		
	}
	//printf("after unsigned long int num_blocks,num_blocks_per_th;");
	readwriteinfo rwi[NUM_THREADS];
	unsigned long int num_blocks;
	
	num_blocks = DATA_SIZE/block_size; 
	num_blocks_per_th = num_blocks/NUM_THREADS;
	//printf("num_blocks:%ld",num_blocks);	
	//printf("num_blocks_per_th:%ld",num_blocks_per_th);
	pthread_t threads[NUM_THREADS]; 
	int rc,tid;
	
	
	start=clock();
	for (i = 0; i < NUM_THREADS; i++) {
		
		rwi[i].start_block=i * num_blocks_per_th;
		rwi[i].end_block=((i+1) * num_blocks_per_th)-1;
		rwi[i].block_size_value=block_size;
		rwi[i].op_type=opr;
		
		if(strncmp(opr,"RWR",3)==0){
			
			if(block_size==1){
				//printf("\nin RW");
				rc = pthread_create(&threads[i], NULL,(void *) RWRLat, (void *) NUM_THREADS);
			}else{
				rc = pthread_create(&threads[i], NULL,(void *) RWR, (void *) &rwi[i]);
			}
		}else if(strncmp(opr,"RWS",3)==0){
			if(block_size==1){
				rc = pthread_create(&threads[i], NULL,(void *) RWSLat, (void *) NUM_THREADS);
			}else{
				rc = pthread_create(&threads[i], NULL,(void *) RWS, (void *) &rwi[i]);
			}
		}
		if (rc) {
			printf("Could not create thread %d\n", i);
		}
		
	}
		

	for (tid= 0; tid < NUM_THREADS; tid++) {
		rc = pthread_join(threads[tid], NULL);
		if (rc) {
		printf("Could not join thread %d\n", tid);
		}
	}

	stop=clock();
	printf("\nTotal time elapsed:%f seconds\n",(float)(stop-start)/(CLOCKS_PER_SEC));
	double ex_time=(float)(stop-start)/(CLOCKS_PER_SEC);
	if(block_size ==1){
		double latency = (ex_time/1e8)*1e6;
		fprintf(fp3,"%s\t\t%d\t\t%d\t\t%lf\n",opr,NUM_THREADS,block_size,latency);
	}else{
		double throughput = (DATA_SIZE*100 / ex_time)/1e9;
		double th_throughput=(2133.0*64.0)/1000.0;
		//fprintf(fp2,"Workload \tConcurrency \tBlockSize \tMyRAMBenchValue \tTheoreticalValue \tMyRAMBenchEfficiency\n");

		fprintf(fp2,"%s\t\t%d\t\t%d\t\t%lf\t\t%lf\t\t%lf\n",opr,NUM_THREADS,block_size,throughput,th_throughput,(throughput/th_throughput)*100);
	}
	fclose(fp1);
	fclose(fp2);
	fclose(fp3);
	free(src_data);
	free(dest_data);
	pthread_exit(NULL);
	return 0;

}

void *RWR(readwriteinfo *rwinfo){
	
	src_data = (char *)malloc(DATA_SIZE);
	dest_data = (char *)malloc(DATA_SIZE);

	unsigned long int i;
	unsigned long int ran_num,ran_block_num;
	int j=0;
	

	srand(time(NULL));
	
	for(j=0;j<100;j++){	
		for(i=rwinfo->start_block;i<rwinfo->end_block;i++){
			ran_num = rand();
			ran_block_num = ran_num%num_blocks_per_th + rwinfo->start_block; 
			memcpy(dest_data + (ran_block_num * rwinfo->block_size_value), src_data + (i * rwinfo->block_size_value), rwinfo->block_size_value);
		}
	}

}

void *RWS(readwriteinfo *rwinfo){
	
	src_data = (char *)malloc(DATA_SIZE);
	dest_data = (char *)malloc(DATA_SIZE);

	unsigned long int i;
	int j;
	for(j=0;j<100;j++){
		
		for(i=rwinfo->start_block;i<rwinfo->end_block;i++){
				 
			memcpy(dest_data + (i * rwinfo->block_size_value), src_data + (i * rwinfo->block_size_value), rwinfo->block_size_value);
		}
	}
}

void *RWRLat(void *cnt){
	printf("inside RWRLat");
	int num_th=(int) cnt;
	src_data = (char *)malloc(1000000);
	dest_data = (char *)malloc(1000000);
	unsigned long int i;
	unsigned long int ran_num,ran_block_num;
	srand(time(NULL));
	for(i=0;i<1e8/num_th;i++){	
		ran_num = rand()%1000000; 
		memcpy(dest_data + ran_num, src_data+ran_num,1);
	}

}

void *RWSLat(void *cnt){	
	int num_th=(int) cnt;
	src_data = (char *)malloc(1000000000);
	dest_data = (char *)malloc(1000000000);
	unsigned long int i;
	srand(time(NULL));
	for(i=0;i<1e8/num_th;i++){	
		memcpy(dest_data+i, src_data+i,1);
	}

}

