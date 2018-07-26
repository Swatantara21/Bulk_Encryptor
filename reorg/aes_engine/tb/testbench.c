#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include <pthreadUtils.h>
#include <Pipes.h>
#include <pipeHandler.h>
#ifndef SW
	#ifdef HW
		#include "vhdlCStubs.h"
	#else
		#ifdef AA2C
			#include "aes_aa_c_model.h"
		#endif
	#endif
#endif

#define ORDER 128

uint64_t sent_values[2];
uint64_t cmd_values[4];
void Exit(int sig)
{
	fprintf(stderr, "## Break! ##\n");
	exit(0);
}

void Sender()
{
	int idx,jdx,k;
	k=0;
	// sacrificial..
	cmd_values[0] = 0x0000000000000000;

	// ED=1 mode=0 got-new-key=1 count=255
	cmd_values[1] = 0x08000000000000ff;

	// key_A
	cmd_values[2] = 0x0000000000000000;
	// key_B
	cmd_values[3] = 0x0000000000000000;
	
	write_uint64_n("in_data",cmd_values,4);

	sent_values[0] = 0x8000000000000000;
	sent_values[1] = 0x0000000000000000;
	
	write_uint64_n("in_data",sent_values,2);
	fprintf(stderr,"\nPT: %016lx %016lx",sent_values[0],sent_values[1]);
	
	for(idx = 1; idx < ORDER-1; idx++)
	{	
		if(sent_values[k]==0xffffffffffffffff){
			k++;
			sent_values[k]= 0x8000000000000000;
		}
		else
			sent_values[k] = sent_values[k]|(sent_values[k]/2);
			
		write_uint64_n("in_data",sent_values,2);
		fprintf(stdout,"\nPT: %d: %016lx %016lx",idx,sent_values[0],sent_values[1]);
	}
	
	k=0;
	cmd_values[1] = 0x8800000000000008;
	cmd_values[0] = 0x0000000000000000;
	cmd_values[2] = 0x0000000000000000;
	cmd_values[3] = 0x0000000000000001;
	
	write_uint64_n("in_data",cmd_values,4);
	sent_values[0] = 0x8000000000000000;
	sent_values[1] = 0x0000000000000000;
	
	write_uint64_n("in_data",sent_values,2);
	fprintf(stderr,"\nPT: %16lx %16lx",sent_values[0],sent_values[1]);
	
	for(idx = 1; idx < ORDER-1; idx++)
	{	
		if(sent_values[k]==0xffffffffffffffff){
			k++;
			sent_values[k]= 0x8000000000000000;
		}
		else
			sent_values[k] = sent_values[k]|(sent_values[k]/2);
			
		write_uint64_n("in_data",sent_values,2);
		fprintf(stdout,"\nPT: %d: %16lx %16lx",idx,sent_values[0],sent_values[1]);
	}
	
}


DEFINE_THREAD(Sender)

int main(int argc, char* argv[])
{	
	uint64_t result[2];
	signal(SIGINT,  Exit);
  	signal(SIGTERM, Exit);
#ifdef AA2C
	init_pipe_handler();
#endif

	PTHREAD_DECL(Sender);
	PTHREAD_CREATE(Sender);

#ifdef AA2C
	aes_start_daemons(NULL,0);
#endif
	
	uint8_t idx, jdx;
	
	
	for(idx = 0; idx < ORDER; idx++)
	{
	
		read_uint64_n("out_data",result,2);
		fprintf(stdout,"\nCT: %d: %016lx%016lx",idx, result[0],result[1]);
	}
	for(idx = 0; idx < ORDER; idx++)
	{
	
		read_uint64_n("out_data",result,2);
		fprintf(stdout,"\nCT: %d: %016lx%016lx",idx, result[0],result[1]);
	}
	
	PTHREAD_CANCEL(Sender);
	return(0);
}
