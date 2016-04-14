#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include "pthreadUtils.h"
//#include <Pipes.h>
//#include <pipeHandler.h>

#include "timer.h"
#include "riffa.h"
//#ifndef SW
//#include "vhdlCStubs.h"
//#endif

uint32_t numWords;
uint32_t result[4];
uint32_t sent_values[4];
fpga_t * fpga;
uint32_t sent, recvd, chnl, id;
#define NUM_TESTS 100
void Exit(int sig)
{
	fprintf(stderr, "## Break! ##\n");
	exit(0);
}

	
void Sender()
{
	int idx,jdx,k;
	k=2;
	sent_values[2] = 0x80000000;
	sent_values[0] = 0x00ffffff;
	sent_values[1] = 0x00ffffff;
	sent_values[3] = 0x00fffff0;
	//printf("\nPT: %8x %8x %8x %8x",sent_values[0],sent_values[1],sent_values[2],sent_values[3]);
	//write_uint32_n("in_data",sent_values,4);
	//sent = fpga_send(fpga, chnl, sent_values, 4, 0, 1, 25000);	
	for(idx = 0; idx < numWords; idx++)
	{	
		sent = fpga_send(fpga, chnl, sent_values, 4, 0, 1, 25000);
		//write_uint32_n("in_data",sent_values,4);
		printf("\nsend_status = %d -- %d ,    input = %8x %8x %8x %8x",sent, idx,sent_values[0],sent_values[1],sent_values[2],sent_values[3]);
		if(sent_values[k]==0xffffffff){
			k++;
			sent_values[k]= 0x80000000;
		}
		else
			sent_values[k] = sent_values[k]|(sent_values[k]/2);
			
			

		///////////////////////
		recvd = fpga_recv(fpga, chnl, result, 4, 25000);			
			fprintf(stdout,"\nrecv_status = %d -- %d ,    output = %8x %8x %8x %8x",recvd,idx, result[0],result[1],result[2],result[3]);
	///////////////////////////////////////////////////
	}
}

DEFINE_THREAD(Sender)


int main(int argc, char** argv) {
	//fpga_t * fpga;
	fpga_info_list info;
	int option;
	int i;
	//int id;
	//int chnl;
	//size_t numWords;
	//int sent;
	//int recvd;
	int failure = 0;
	unsigned int * sendBuffer;
	unsigned int * recvBuffer;
	int err;
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
			printf("Usage: %s %d <fpga id> <chnl> <num words to transfer>\n", argv[0], option);
			return -1;
		}

		//size_t maxWords, minWords;
		id = atoi(argv[2]);
		chnl = atoi(argv[3]);
		numWords = atoi(argv[4]);
		
		// Get the device with id
		fpga = fpga_open(id);
		if (fpga == NULL) {
			printf("Could not get FPGA %d\n", id);
			return -1;
		}

		
		
		signal(SIGINT,  Exit);
	  	signal(SIGTERM, Exit);

		PTHREAD_DECL(Sender);
		PTHREAD_CREATE(Sender);
	

		uint8_t idx, jdx;
	
		/*for(idx = 0; idx < numWords; idx++)
		{
			
			//read_uint32_n("out_data",result,4);
			recvd = fpga_recv(fpga, chnl, result, 4, 25000);			
			fprintf(stdout,"\nrecv_status = %d -- %d ,    output = %8x %8x %8x %8x",recvd,idx, result[0],result[1],result[2],result[3]);
			if (recvd <= 0) idx--;
		}
	*/
	
		PTHREAD_CANCEL(Sender);
				
		// Done with device
	        fpga_close(fpga);
	}
	return 0;
}
