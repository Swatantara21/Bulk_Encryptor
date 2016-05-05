#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "timer.h"
#include "riffa.h"
//#ifndef SW
//#include "vhdlCStubs.h"
//#endif

uint64_t result[256];
uint64_t sent_values[256];
//uint32_t numWords;
//fpga_t * fpga;
//uint32_t sent, recvd, chnl, id;
#define NUM_TESTS 100


int main(int argc, char** argv) {
	fpga_t * fpga;
	fpga_info_list info;
	int option;
	int i;
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
		printf("Number of devices: %d\n", info.num_fpgas);
		for (i = 0; i < info.num_fpgas; i++) {
			printf("%d: id:%d\n", i, info.id[i]);
			printf("%d: num_chnls:%d\n", i, info.num_chnls[i]);
			printf("%d: name:%s\n", i, info.name[i]);
			printf("%d: vendor id:%04X\n", i, info.vendor_id[i]);
			printf("%d: device id:%04X\n", i, info.device_id[i]);
		}
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
		if (argc < 5) {
			printf("Usage: %s %d <fpga id> <chnl> <channel size in channel tester in bytes> <num words to transfer>\n", argv[0], option);
			return -1;
		}

		//size_t maxWords, minWords;
		id = atoi(argv[2]);
		chnl = atoi(argv[3]);
		//numWords = atoi(argv[5]);
		ch_size = atoi(argv[4]);
		numLoops = 1;
		//remWords = numWords%ch_size;
		// Get the device with id
		fpga = fpga_open(id);
		//uint64_t result[ch_size];
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
		
		//for (idx =0; idx<chsize/2; idx=idx+2)
		//	printf("\n%d: %16llx%16llx",idx,sent_values[idx],sent_values[idx+1]);
		
		//printf("loops = %d",numLoops);
		
			
			sent = fpga_send(fpga, chnl, sent_values, ch_size, 0, 1, 25000);
			recvd = fpga_recv(fpga, chnl, result, ch_size, 25000);
			for (k =0; k<ch_size/2; k=k+2)
				printf("\n%16llx%16llx",result[k],result[k+1]);
			
		
			
		GET_TIME_VAL(1);
		//for (idx =0; idx<256; idx=idx+2)
		//	printf("\n%16llx%16llx",result[idx],result[idx+1]);
		
		//printf("\ntime taken : %f ms, N= %d \n",(TIME_VAL_TO_MS(1) - TIME_VAL_TO_MS(0)),numWords);
		//printf("avg time taken by 1 set of data : %f ms\n",((TIME_VAL_TO_MS(1) - TIME_VAL_TO_MS(0)))/numWords);
		// Done with device
	        fpga_close(fpga);
	}
	else if (option == 3) { // Send data, receive data
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
		//uint64_t result[ch_size];
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
		
		printf("\ntime taken : %f ms, N= %d \n",(TIME_VAL_TO_MS(1) - TIME_VAL_TO_MS(0)),numWords);
		printf("avg time taken by 1 set of data : %f ms\n",((TIME_VAL_TO_MS(1) - TIME_VAL_TO_MS(0)))/numWords);
		// Done with device
	        fpga_close(fpga);
	}
	return 0;
}
