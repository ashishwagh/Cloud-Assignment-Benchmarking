#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>
#include <immintrin.h>
#include <unistd.h>

#define itr500b 500000000000
#define itr250b 250000000000
#define itr125b 125000000000

__m256i myQPFunc(void *a);
__m256i myHPFunc(void *b);
__m256 	mySPFunc(void *c);
__m256d myDPFunc(void *d);

//FILL ME IN
int main(int argc, char *argv[]) {

	char ch,buff[255],prec[10],thread_size[10],opfname[50],myFunc[10];
	int NUM_THREADS,i;
	long long int loopNum=0,n_iteration;
	clock_t start,stop;
	FILE *fp1,*fp2;
	if(argv[1]!=NULL){
		fp1 = fopen(argv[1], "r");
	}
	
	fgets(prec,sizeof prec,(FILE*)fp1);
	for (i = 0; i < strlen(prec); i++)
        {
            if ( prec[i] == '\n')
                prec[i] = '\0';
        }
	fgets(thread_size,sizeof thread_size,(FILE*)fp1);
	
	//printf("hello%s",thread_size);
	sscanf(thread_size, "%d", &NUM_THREADS);
	//NUM_THREADS=atoi(thread_size);

	//printf("Hi\n%s%d",prec,NUM_THREADS);
	
	strcpy(opfname,argv[1]);
	strcat(opfname,"_output.dat");
	if( access( "./output/CPUOUTPUT.txt",F_OK) != -1 ) {
		fp2 = fopen("./output/CPUOUTPUT.txt", "r+");
		fseek(fp2, 0, SEEK_END);
	}else{
		fp2 = fopen("./output/CPUOUTPUT.txt", "w");
		fprintf(fp2,"Workload \tConcurrency \tMyCPUBenchValue \tTheoreticalValue \tEfficiency\n");		
	}
	
	
	pthread_t threads[NUM_THREADS]; 
	long tid; 
	int rc;
	
	//printf("%d",strncmp(prec,"QP",2));	
	
	start=clock();
	for (tid = 0; tid < NUM_THREADS; tid++) {
		if(strncmp(prec,"QP",2)==0){
			pthread_create(&threads[tid], NULL, myQPFunc, (void *) NUM_THREADS);
		}else if(strncmp(prec,"HP",2)==0){
			pthread_create(&threads[tid], NULL, myHPFunc, (void *) NUM_THREADS);
		}else if(strncmp(prec,"SP",2)==0){
			pthread_create(&threads[tid], NULL, mySPFunc, (void *) NUM_THREADS);
		}else if(strncmp(prec,"DP",2)==0){
			pthread_create(&threads[tid], NULL, myDPFunc, (void *) NUM_THREADS);
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
	double ex_time=(float)(stop-start)/(CLOCKS_PER_SEC);
	printf("\nTotal time elapsed: %f seconds\n",(float)(stop-start)/(CLOCKS_PER_SEC));
	if(NUM_THREADS==1){
		n_iteration=10000000000000;
	}else if(NUM_THREADS==2){
		n_iteration=500000000000;
	}else if(NUM_THREADS==4){
		n_iteration=250000000000;	
	}
	
	double gflops = ((double)n_iteration / (ex_time)) / 1e9;
	double th_gflops=0.0;
	//fprintf(fp2,"Workload \tConcurrency \tMyCPUBenchValue \tTheoreticalValue \tEfficiency\n");
	if(strncmp(prec,"QP",2)==0){
		th_gflops=8*2.60*32;
	}else if(strncmp(prec,"HP",2)==0){
		th_gflops=8*2.60*16;
	}else if(strncmp(prec,"SP",2)==0){
		th_gflops=8*2.60*8;
	}else if(strncmp(prec,"DP",2)==0){
		th_gflops=8*2.60*4;
	}

	fprintf(fp2,"%s\t\t%d\t\t%lf\t\t%lf\t\t%lf\n",prec,NUM_THREADS,gflops,th_gflops,(gflops/th_gflops)*100);
	fclose(fp1);
	fclose(fp2);
	//pthread_exit(NULL);
	return 0;

}

__m256i myQPFunc(void *cnt){
	long long int i,n_iteration;
	int thread_cnt = (int) cnt;
	
	if(thread_cnt==1){
		//printf("Thread count=%d",thread_cnt);
		n_iteration=10000000000000;
	}else if(thread_cnt==2){
		//printf("Thread count=%d",thread_cnt);
		n_iteration=500000000000;
			
	}else if(thread_cnt==4){
		//printf("Thread count=%d",thread_cnt);
		n_iteration=250000000000;	
	}

	__m256i a = _mm256_set_epi8('a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a');
	__m256i b = _mm256_set_epi8('b','b','b','b','b','b','b','b','b','b','b','b','b','b','b','b','b','b','b','b','b','b','b','b','b','b','b','b','b','b','b','b');

	__m256i result;
	for(i=0;i<n_iteration/32;i++){
		result = _mm256_add_epi8(a,b);
	}
	return result;
	
}

__m256i myHPFunc(void *cnt){
	long long int i,n_iteration;
	int thread_cnt = (int) cnt;
	
	if(thread_cnt==1){
		//printf("Thread count=%d",thread_cnt);
		n_iteration=10000000000000;
	}else if(thread_cnt==2){
		//printf("Thread count=%d",thread_cnt);
		n_iteration=500000000000;
			
	}else if(thread_cnt==4){
		//printf("Thread count=%d",thread_cnt);
		n_iteration=250000000000;	
	}
	
	__m256i a = _mm256_set_epi16(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16);
	__m256i b = _mm256_set_epi16(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16);
	
	__m256i result;
	for(i=0;i<n_iteration/16;i++){
		result = _mm256_add_epi16(a,b);
	}
	return result;
}


__m256 mySPFunc(void *cnt){
	
	long long int i,n_iteration;
	int thread_cnt = (int) cnt;
	
	if(thread_cnt==1){
		//printf("Thread count=%d",thread_cnt);
		n_iteration=10000000000000;
	}else if(thread_cnt==2){
		//printf("Thread count=%d",thread_cnt);
		n_iteration=500000000000;
			
	}else if(thread_cnt==4){
		//printf("Thread count=%d",thread_cnt);
		n_iteration=250000000000;	
	}

	int a1[8]={1,1,1,1,1,1,1,1};
	int a2[8]={2,2,2,2,2,2,2,2};

	__m256 i1 = _mm256_load_ps(a1);
	__m256 i2 = _mm256_load_ps(a2);

	__m256 result;
	for(i=0;i< n_iteration/16 ;i++){
		result = _mm256_add_ps(i1, i2);
		i1 = _mm256_sub_ps(result, i2);
	}
	return result;
}

__m256d myDPFunc(void *cnt){
	long long int i,n_iteration,check;
	int thread_cnt = (int) cnt;
	
	if(thread_cnt==1){
		//printf("Thread count=%d",thread_cnt);
		n_iteration=10000000000000;
	}else if(thread_cnt==2){
		//printf("Thread count=%d",thread_cnt);
		n_iteration=500000000000;
			
	}else if(thread_cnt==4){
		//printf("Thread count=%d",thread_cnt);
		n_iteration=250000000000;	
	}

	float a1[4]={1.0,1.0,1.0,1.0};
	float a2[4]={2.0,2.0,2.0,2.0};
	__m256d i1 = _mm256_load_pd(a1);
	__m256d i2 = _mm256_load_pd(a2);

	//check = n_iteration/4;
	//printf("%lld",check);
	__m256d result;
	for(i=0;i< n_iteration/8 ;i++){
		result = _mm256_add_pd(i1, i2);
		i1 = _mm256_sub_pd(i2, result);
	}
	return result;

}
