#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <arpa/inet.h>

#define PORT1 3001
#define PORT2 3010
#define PORT3 3020
#define PORT4 3030
#define PORT5 3040
#define PORT6 3050
#define PORT7 3060
#define PORT8 3070

const double DATA_SIZE = 1000000000.0;

struct th_info_c {
	int block_size;
	int num_th;
	int tid;
}typedef th_info_c;

void *client_tcp(void *a);

int main(int argc, char const *argv[]) {
	char ch, opr[10], thread_size[10], blockvalue[10], opfname[50], myFunc[10];
	int NUM_THREADS, i, block_size;
	clock_t start, stop;
	FILE *fp1, *fp2, *fp3;

	if (argv[1] != NULL) {
		fp1 = fopen(argv[1], "r");
	}

	while (fgets(opr, sizeof opr, fp1) != NULL) {
		fgets(blockvalue, sizeof blockvalue, (FILE*) fp1);
		fgets(thread_size, sizeof thread_size, (FILE*) fp1);
	}

	for (i = 0; i < strlen(opr); i++) {
		if (opr[i] == '\n')
			opr[i] = '\0';
	}

	sscanf(thread_size, "%d", &NUM_THREADS);

	if (access("NETWORKOUTPUT.txt", F_OK) != -1) {
		fp2 = fopen("NETWORKOUTPUT.txt", "r+");
		fseek(fp2, 0, SEEK_END);
	} else {
		fp2 = fopen("NETWORKOUTPUT.txt", "w");
		fprintf(fp2,"Protocol \tConcurrency \tBlockSize \tMyNETBenchValue \tTheoreticalValue \tMyNETBenchEfficiency\n");
	}

	if (access("NETWORKLATENCYOUTPUT.txt", F_OK) != -1) {
		fp3 = fopen("NETWORKLATENCYOUTPUT.txt", "r+");
		fseek(fp3, 0, SEEK_END);
	} else {
		fp3 = fopen("NETWORKLATENCYOUTPUT.txt", "w");
		fprintf(fp3, "Protocol \tConcurrency \tBlockSize \tLatency\n");
	}

	block_size = atoi(blockvalue);
	//block_size = block_size * 1000;

	pthread_t threads[NUM_THREADS];
	start = clock();

	for (i = 0; i < NUM_THREADS; i++) {
		th_info_c * tinfo = (th_info_c *)malloc(sizeof(th_info_c));
		tinfo->block_size=block_size;
		tinfo->num_th=NUM_THREADS;
		tinfo->tid=i;
		pthread_create(&threads[i], NULL, client_tcp, tinfo); //multiple tcp clients
	}
	for (i = 0; i < NUM_THREADS; i++) {
		pthread_join(threads[i], NULL);
	}

	stop = clock();

	printf("\nTotal time elapsed:%f seconds\n",(float) (stop - start) / (CLOCKS_PER_SEC));
	double ex_time=(float)(stop-start)/(CLOCKS_PER_SEC);
	if(block_size ==1){
		double latency = ((ex_time*1000)/1e8);
		fprintf(fp3,"%s\t\t%d\t\t%d\t\t%lf\n",opr,NUM_THREADS,block_size,latency);
	}else{
		double throughput = ((DATA_SIZE*100/ex_time)/1e6);
		double th_throughput=56000;
		//fprintf(fp2,"Protocol \tConcurrency \tBlockSize \tMyNETBenchValue \tTheoreticalValue \tMyNETBenchEfficiency\n");

		fprintf(fp2,"%s\t\t%d\t\t%d\t\t%lf\t\t%lf\t\t%lf\n",opr,NUM_THREADS,block_size,throughput,th_throughput,(throughput/th_throughput)*100);
	}	

	fclose(fp1);
	fclose(fp2);
	fclose(fp3);
	return 0;

}
void *client_tcp(void *a){

	th_info_c * tinfo = (struct th_info_c *)a;
	int sock = 0,j=0;
	long long int loopNum = 0;
	char *buff = (char *) malloc(sizeof(char) * tinfo->block_size);
	memset(buff, 'c', sizeof(char)*tinfo->block_size);

	struct sockaddr_in serv_addr;
	memset(&serv_addr, '0', sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;

	if(tinfo->tid==0){
		serv_addr.sin_port = htons(PORT1);
	}else if(tinfo->tid==1){
		serv_addr.sin_port = htons(PORT2);
	}else if(tinfo->tid==2){
		serv_addr.sin_port = htons(PORT3);
	}else if(tinfo->tid==3){
		serv_addr.sin_port = htons(PORT4);
	}else if(tinfo->tid==4){
		serv_addr.sin_port = htons(PORT5);
	}else if(tinfo->tid==5){
		serv_addr.sin_port = htons(PORT6);
	}else if(tinfo->tid==6){
		serv_addr.sin_port = htons(PORT7);
	}else if(tinfo->tid==7){
		serv_addr.sin_port = htons(PORT8);
	}

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("\n Socket creation error \n");
		exit(1);
	}

	// Convert IPv4 and IPv6 addresses from text to binary form
	 if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
	 {
	 printf("\nInvalid address/ Address not supported \n");
	 exit(1);
	 }
	
	//printf("\nbefore connect");
	if (connect(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		printf("\nConnection Failed \n");
		exit(1);;
	}

	if(tinfo->block_size ==1){
		for (loopNum = 0; loopNum < (1e8/tinfo->num_th); loopNum ++) {
			send(sock, buff, tinfo->block_size, 0);
			recv(sock, buff, tinfo->block_size, 0);
		}		

	}else{
		for (j = 0; j < (100/tinfo->num_th); j++){	
			for (loopNum = 0; loopNum < (DATA_SIZE / tinfo->block_size); loopNum ++) {
				send(sock, buff, tinfo->block_size, 0);
				recv(sock, buff, tinfo->block_size, 0);
			}
		}
	}
	free(buff);
	close(sock);	
}


