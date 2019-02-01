#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
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

struct th_info_s {
	int block_size;
	int num_th;
	int tid;
}typedef th_info_s;

const double DATA_SIZE = 1000000000.0;

void *udp_server(void *a);

int main(int argc, char const *argv[]){

	
	char ch, opr[10], thread_size[10], blockvalue[10], opfname[50], myFunc[10];
	int NUM_THREADS, i, block_size,j;
	long long int k;
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

	block_size = atoi(blockvalue);

	pthread_t threads[NUM_THREADS];
	for (i = 0; i < NUM_THREADS; i++) {
		th_info_s * tinfo = (th_info_s *)malloc(sizeof(th_info_s));
		tinfo->block_size=block_size;
		tinfo->num_th=NUM_THREADS;
		tinfo->tid=i;
		pthread_create(&threads[i], NULL, udp_server, tinfo); //multiple tcp clients
	}
	
	for (i = 0; i < NUM_THREADS; i++) {
		pthread_join(threads[i], NULL);
	}

	return 0;
}

void *udp_server(void *a){

	int udpSocket,j=0;
	th_info_s * tinfo = (struct th_info_s *)a;
	struct sockaddr_in serverAddr;
	struct sockaddr_storage serverStorage;
	socklen_t addr_size;
	int i;
	long long int loopNum = 0;
	char *buff = (char *) malloc(sizeof(char) * tinfo->block_size);
	memset(buff, 'c', sizeof(char)*tinfo->block_size);

	udpSocket = socket(PF_INET, SOCK_DGRAM, 0);

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	memset(&serverAddr, 0, sizeof(serverAddr));

	if(tinfo->tid==0){
		serverAddr.sin_port = htons(PORT1);
	}else if(tinfo->tid==1){
		serverAddr.sin_port = htons(PORT2);
	}else if(tinfo->tid==2){
		serverAddr.sin_port = htons(PORT3);
	}else if(tinfo->tid==3){
		serverAddr.sin_port = htons(PORT4);
	}else if(tinfo->tid==4){
		serverAddr.sin_port = htons(PORT5);
	}else if(tinfo->tid==5){
		serverAddr.sin_port = htons(PORT6);
	}else if(tinfo->tid==6){
		serverAddr.sin_port = htons(PORT7);
	}else if(tinfo->tid==7){
		serverAddr.sin_port = htons(PORT8);
	}  

	bind(udpSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

	addr_size = sizeof serverStorage;
	
	if(tinfo->block_size ==1){
		for (loopNum = 0; loopNum < (1e8/tinfo->num_th); loopNum ++) {
			recvfrom(udpSocket,buff,sizeof(buff),0,(struct sockaddr *)&serverStorage, &addr_size);
			sendto(udpSocket,buff,sizeof(buff),0,(struct sockaddr *)&serverStorage,addr_size);
		}		

	}else{	
		for (j = 0; j < (100/tinfo->num_th); j++){
			for (loopNum = 0; loopNum < (DATA_SIZE / tinfo->block_size);loopNum ++) {
				recvfrom(udpSocket,buff,sizeof(buff),0,(struct sockaddr *)&serverStorage, &addr_size);
				sendto(udpSocket,buff,sizeof(buff),0,(struct sockaddr *)&serverStorage,addr_size);
			}
		}
	}
	free(buff);
	close(udpSocket);
}
