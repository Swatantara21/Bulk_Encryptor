#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>

#include "pthreadUtils.h"

#include "timer.h"
#include "riffa.h"
//#ifndef SW
//#include "vhdlCStubs.h"
//#endif
#define NUM_TESTS 100
#define DATA_CH_SIZE 4



uint64_t status;
uint64_t result[DATA_CH_SIZE];
uint64_t sent_values[DATA_CH_SIZE];
uint64_t cmd_values[1];
uint64_t key_values[2];

uint64_t sent_values[1024];
uint64_t cmd_values[1024];

//uint32_t numWords;
fpga_t * fpga;
//uint32_t sent, recvd, chnl, id;


void Exit(int sig)
{
	fprintf(stderr, "## Break! ##\n");
	exit(0);
}
void RecvStatus(){
	uint64_t status;
	while (1){
	status = fpga_recv(fpga, 1, result, ch_size, 25000);
	fprintf(stdout,"\nstatus: %16lx\n", status);
	}
}

DEFINE_THREAD(RecvStatus)

int main(int argc, char** argv) {
	//fpga_t * fpga;
	fpga_info_list info;
	int option;
	int i,j;
	int id;
	int chnl;
	int ch_size;
	size_t numWords, numLoops, remWords;
	int sent;
	int recvd;
	int failure = 0;
	//unsigned int * sendBuffer;
	//unsigned int * recvBuffer;
	int err;
	int idx,jdx,k;
	GET_TIME_INIT(3);
	if (argc < 2) {
		printf("Usage: %s <option>\n", argv[0]);
		return -1;
	}

	option = atoi(argv[1]);

	if (option == 0) {
		// List FPGA info
		// Populate the fpga_info_list struct
		if (fpga_list(&info) != 0) {
			printf("Error populating fpga_info_list\n");
			return -1;
		}
		printf("fpga info:\nNumber of devices: %d\n", info.num_fpgas);
		for (i = 0; i < info.num_fpgas; i++) {
			printf("%d: id:%d\n", i, info.id[i]);
			printf("%d: num_chnls:%d\n", i, info.num_chnls[i]);
			printf("%d: name:%s\n", i, info.name[i]);
			printf("%d: vendor id:%04X\n", i, info.vendor_id[i]);
			printf("%d: device id:%04X\n", i, info.device_id[i]);
		}
		printf("options: \n1: fpga reset\n2: KAT \n3: MonteCarlo Test (for data ch size=4)\n");
	}
	else if (option == 1) { // Reset FPGA
		if (argc < 3) {
			printf("Usage: %s %d <fpga id>\n", argv[0], option);
			return -1;
		}

		id = atoi(argv[2]);

		// Get the device with id
		fpga = fpga_open(id);
		if (fpga == NULL) {
			printf("Could not get FPGA %d\n", id);
			return -1;
		}

		// Reset
		fpga_reset(fpga);

		// Done with device
		fpga_close(fpga);
	}
	else if (option == 2) { // Send data, receive data
		if (argc < 4) {
			printf("KAT(Known Answer Test)\nUsage: %s %d <fpga id> <data channel size (bytes) in channel tester> \n", argv[0], option);
			return -1;
		}

		//size_t maxWords, minWords;
		id = atoi(argv[2]);
		ch_size = atoi(argv[3]);
		numLoops = 1;
		// Get the device with id
		fpga = fpga_open(id);
		//uint64_t result[ch_size];
		if (fpga == NULL) {
			printf("Could not get FPGA %d\n", id);
			return -1;
		}


		cmd_values[0] = 0x0800000000000000;
		cmd_values[0] = cmd_values[0] + ch_size/4;
		key_values[0] = 0x0000000000000000;
		key_values[1] = 0x0000000000000000;

		
		k=0;
		sent_values[0] = 0x8000000000000000;
		sent_values[1] = 0x0000000000000000;
		for (idx =2; idx<256; idx=idx+2){
			sent_values[idx]=sent_values[idx-2];
			sent_values[idx+1]=sent_values[idx-1]; 
			if(sent_values[idx+k-2]==0xffffffffffffffff){
				k++;
				sent_values[idx+k]= 0x8000000000000000;
			}
			else
				sent_values[idx+k] = sent_values[idx+k]|(sent_values[idx+k]/2);
		}
		for (idx =0; idx<DATA_CH_SIZE; idx++)
			sent_values[idx+256]=sent_values[idx]; 
			
		//for (idx =0; idx<chsize/2; idx=idx+2)
		//	printf("\n%d: %16llx%16llx",idx,sent_values[idx],sent_values[idx+1]);
		
		
		GET_TIME_VAL(0);
		sent = fpga_send(fpga, 1, cmd_values, 2, 0, 1, 25000);
		printf("\ncmd sent status = %d",sent);
		recvd = fpga_recv(fpga, 1, &status, 2, 25000);
		fprintf(stdout,"\nrec_status: %d status: %16lx\n", recvd, status);

		sent = fpga_send(fpga, 1, key_values, 4, 0, 1, 25000);
		printf("\nkey sent status = %d",sent);
		
		recvd = fpga_recv(fpga, 1, &status, 2, 25000);
		fprintf(stdout,"\nrec_status: %d status: %16lx\n", recvd, status);
		//recvd = fpga_recv(fpga, 1, &status, 2, 25000);
		//fprintf(stdout,"\nrec_status: %d status: %16lx\n", recvd, status);
		
		sent = fpga_send(fpga, 0, sent_values, ch_size, 0, 1, 25000);
		printf("\ndata sent status = %d",sent);
		recvd = fpga_recv(fpga, 0, result, ch_size, 25000);
		printf("\ndata recv status = %d",sent);
		
		//for (k =0; k<ch_size/2; k=k+2)
		//	printf("\nres: %16llx%16llx",result[k],result[k+1]);
		
		cmd_values[0] = ch_size/4;
		sent = fpga_send(fpga, 1, cmd_values, 2, 0, 1, 25000);
		printf("\ncmd sent status = %d",sent);
		recvd = fpga_recv(fpga, 1, &status, 2, 25000);
		fprintf(stdout,"\nrec_status: %d status: %16lx\n", recvd, status);
		recvd = fpga_recv(fpga, 1, &status, 2, 25000);
		fprintf(stdout,"\nrec_status: %d status: %16lx\n", recvd, status);
		sent = fpga_send(fpga, 0, sent_values, ch_size, 0, 1, 25000);
		printf("\ndata sent status = %d",sent);
		
		recvd = fpga_recv(fpga, 0, result, ch_size, 25000);
		printf("\ndata recv status = %d",sent);
		GET_TIME_VAL(1);
		
		//for (k =0; k<ch_size/2; k=k+2)
		//	printf("\nres: %16llx%16llx",result[k],result[k+1]);
		
		cmd_values[0] = 0x0800000000000000 + ch_size/4;
		sent = fpga_send(fpga, 1, cmd_values, 2, 0, 1, 25000);
		printf("\ncmd sent status = %d",sent);
		recvd = fpga_recv(fpga, 1, &status, 2, 25000);
		fprintf(stdout,"\nrec_status: %d status: %16lx\n", recvd, status);

		sent = fpga_send(fpga, 1, key_values, 4, 0, 1, 25000);
		printf("\nkey sent status = %d",sent);
		
		recvd = fpga_recv(fpga, 1, &status, 2, 25000);
		fprintf(stdout,"\nrec_status: %d status: %16lx\n", recvd, status);
		//recvd = fpga_recv(fpga, 1, &status, 2, 25000);
		//fprintf(stdout,"\nrec_status: %d status: %16lx\n", recvd, status);
		
		sent = fpga_send(fpga, 0, sent_values, ch_size, 0, 1, 25000);
		printf("\ndata sent status = %d",sent);
		recvd = fpga_recv(fpga, 0, result, ch_size, 25000);
		printf("\ndata recv status = %d",sent);
		
		for (k =0; k<ch_size/2; k=k+2)
			printf("\n%16llx%16llx",result[k],result[k+1]);
	
				
		printf("\ntime taken (latency) : %f ms, N= %d \n",(TIME_VAL_TO_MS(1) - TIME_VAL_TO_MS(0)),ch_size/4);
		// Done with device
	        fpga_close(fpga);
	}
	else if (option == 3) { // Send data, receive data
		if (argc < 4) {
			printf("Monte Carlo Test for data channel size 4\nUsage: %s %d <fpga id> <data channel size (bytes) in channel tester> \n", argv[0], option);
			return -1;
		}
		
		//size_t maxWords, minWords;
		id = atoi(argv[2]);
		ch_size = atoi(argv[3]);
		numLoops = 1;
		// Get the device with id
		fpga = fpga_open(id);
		if (fpga == NULL) {
			printf("Could not get FPGA %d\n", id);
			return -1;
		}

		cmd_values[0] = 0x0800000000000000 + 0x03e8;
		key_values[0] = 0x8d2e60365f17c7df;
		key_values[1] = 0x1040d7501b4a7b5a;
		
		k=0;
		sent_values[0] = 0x59b5088e6dadc3ad;
		sent_values[1] = 0x5f27a460872d5929;
	
		GET_TIME_VAL(0);
		for (i=0; i<100; i++){
			fprintf(stderr,"cmd: %16llx key:%16llx %16llx\n",cmd_values[0],key_values[0],key_values[1]);
			fprintf(stdout,"\nPT: %d: %16llx %16llx",idx,sent_values[0],sent_values[1]);	
			//for (idx =0; idx<chsize/2; idx=idx+2)
			//	printf("\n%d: %16llx%16llx",idx,sent_values[idx],sent_values[idx+1]);	
			
			//send command
			sent = fpga_send(fpga, 1, cmd_values, 2, 0, 1, 25000);
			//printf("\ncmd sent status = %d",sent);
			recvd = fpga_recv(fpga, 1, &status, 2, 25000);
			//fprintf(stdout,"\nrec_status: %d status: %16llx\n", recvd, status);

			sent = fpga_send(fpga, 1, key_values, 4, 0, 1, 25000);
			//printf("\nkey sent status = %d",sent);
		
			recvd = fpga_recv(fpga, 1, &status, 2, 25000);
			//fprintf(stdout,"\nrec_status: %d status: %16llx\n", recvd, status);
			for (j=0; j<1000; j++){
				sent = fpga_send(fpga, 0, sent_values, 4, 0, 1, 25000);
				//printf("\ndata sent status = %d",sent);
				recvd = fpga_recv(fpga, 0, result, 4, 25000);
				//printf("\ndata recv status = %d",sent);
				sent_values[0] = result[0];
				sent_values[1] = result[1];
			}
			fprintf(stdout,"\nCT: %d: %16llx%16llx",idx, result[0],result[1]);
			key_values[0] = key_values[0] ^ result[0];
			key_values[1] = key_values[1] ^ result[1];
		}
		
		
		GET_TIME_VAL(1);
				
		for (k =0; k<ch_size/2; k=k+2)
			printf("\n%16llx%16llx",result[k],result[k+1]);
	
				
		printf("\ntime taken (latency) : %f ms, N= %d \n",(TIME_VAL_TO_MS(1) - TIME_VAL_TO_MS(0)),ch_size/4);
		// Done with device
	        fpga_close(fpga);
	}
	
	else if (option == 4) { // Send data, receive data
		if (argc < 6) {
			printf("Usage: %s %d <fpga id> <chnl> <out channel size in channel tester in bytes> <num inputs to transfer>\n", argv[0], option);
			return -1;
		}

		//size_t maxWords, minWords;
		id = atoi(argv[2]);
		chnl = atoi(argv[3]);
		/////numWords: no. words of size 32 bits.
		////numInputs: no. of inputs of length 128 bits. 
		int numInputs = atoi(argv[5]);
		numWords = numInputs*4;
		ch_size = atoi(argv[4]);
		numLoops = numWords/ch_size;
		//remWords = numWords%ch_size;
		// Get the device with id
		fpga = fpga_open(id);
		if (fpga == NULL) {
			printf("Could not get FPGA %d\n", id);
			return -1;
		}

		
		
		k=0;
		sent_values[0] = 0x8000000000000000;
		sent_values[1] = 0x0000000000000000;
		for (idx =2; idx<256; idx=idx+2){
			sent_values[idx]=sent_values[idx-2];
			sent_values[idx+1]=sent_values[idx-1]; 
			if(sent_values[idx+k-2]==0xffffffffffffffff){
				k++;
				sent_values[idx+k]= 0x8000000000000000;
			}
			else
				sent_values[idx+k] = sent_values[idx+k]|(sent_values[idx+k]/2);
		}
		for (idx =0; idx<DATA_CH_SIZE; idx++)
			sent_values[idx+256]=sent_values[idx]; 
		
		//for (idx =0; idx<128; idx=idx+2)
		//	printf("\n%d: %16llx%16llx",idx,sent_values[idx],sent_values[idx+1]);
		
		//printf("loops = %d",numLoops);
		
		GET_TIME_VAL(0);
		for(idx = 0; idx < numLoops; idx++)
		{	
			sent = fpga_send(fpga, chnl, sent_values, ch_size, 0, 1, 25000);
			recvd = fpga_recv(fpga, chnl, result, ch_size, 25000);
			//for (k =0; k<128; k=k+2)
			//	printf("\n%d: %16llx%16llx",k,result[k],result[k+1]);
			
		}
			
		GET_TIME_VAL(1);
		//for (idx =0; idx<256; idx=idx+2)
		//	printf("\n%16llx%16llx",result[idx],result[idx+1]);
		
		printf("\ntime taken : %f ms, N= %d \n",(TIME_VAL_TO_MS(1) - TIME_VAL_TO_MS(0)),numInputs);
		printf("avg time taken by 1 set of data : %f ms\n",((TIME_VAL_TO_MS(1) - TIME_VAL_TO_MS(0)))/numInputs);
		// Done with device
	        fpga_close(fpga);
	}
	return 0;
}
