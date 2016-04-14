#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include <pthreadUtils.h>
#include <Pipes.h>
#include <pipeHandler.h>
#ifndef SW
#include "vhdlCStubs.h"
#endif

#define ORDER 15

uint64_t sent_values[2];

void Exit(int sig)
{
	fprintf(stderr, "## Break! ##\n");
	exit(0);
}

	
void Sender()
{
	int idx,jdx,k;
	k=0;
	sent_values[0] = 0x8000000000000000;
	sent_values[1] = 0x0000000000000000;
	
	write_uint64_n("in_data",sent_values,2);
	//fprintf(stderr,"\nPT: %16llx %16llx",sent_values[0],sent_values[1]);
	
	for(idx = 1; idx < ORDER; idx++)
	{	
		if(sent_values[k]==0xffffffffffffffff){
			k++;
			sent_values[k]= 0x8000000000000000;
		}
		else
			sent_values[k] = sent_values[k]|(sent_values[k]/2);
			
		write_uint64_n("in_data",sent_values,2);
		//fprintf(stdout,"\nPT: %16llx %16llx",sent_values[0],sent_values[1]);
	}
}


DEFINE_THREAD(Sender)


int main(int argc, char* argv[])
{	
	uint64_t result[2];
	signal(SIGINT,  Exit);
  	signal(SIGTERM, Exit);

	PTHREAD_DECL(Sender);
	PTHREAD_CREATE(Sender);
	
	uint8_t idx, jdx;
	
	
	for(idx = 0; idx < ORDER; idx++)
	{
	
		read_uint64_n("out_data",result,2);
		fprintf(stdout,"\n%16llx %16llx", result[0],result[1]);
	}
	
	
	PTHREAD_CANCEL(Sender);
	return(0);
}
