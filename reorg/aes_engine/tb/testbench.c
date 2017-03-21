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

#define ORDER 128

uint64_t sent_values[2];
uint64_t cmd_values[3];
void Exit(int sig)
{
	fprintf(stderr, "## Break! ##\n");
	exit(0);
}

	
void SenderCmd()
{
	int idx,jdx,k;
	cmd_values[0] = 0x880000000000000a;
	cmd_values[1] = 0x000000000000000;
	cmd_values[2] = 0x0000000000000000;
	
	write_uint64_n("cmd_in",cmd_values,3);
	fprintf(stderr,"\ncmd: %16lx key:%16lx %16lx",cmd_values[0],cmd_values[1],cmd_values[2]);
	
	cmd_values[0] = 0x880000000000001a;
	cmd_values[1] = 0x0000000000000000;
	cmd_values[2] = 0x0000000000000000;
	write_uint64_n("cmd_in",cmd_values,3);
	fprintf(stderr,"\ncmd: %16lx key:%16lx %16lx",cmd_values[0],cmd_values[1],cmd_values[2]);
	
}

void Sender()
{
	int idx,jdx,k;
	k=0;
	cmd_values[1] = 0x8800000000000004;
	cmd_values[0] = 0x0000000000000000;
	cmd_values[2] = 0x0000000000000000;
	cmd_values[3] = 0x0000000000000000;
	
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

void RecvStatus(){
	uint64_t status;
	while (1){
		status = read_uint64("status_out");
		fprintf(stdout,"\nstatus: %16lx\n", status);
	}
}

DEFINE_THREAD(Sender)
DEFINE_THREAD(SenderCmd)
DEFINE_THREAD(RecvStatus)

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
		fprintf(stdout,"\nCT: %d: %16lx%16lx",idx, result[0],result[1]);
	}
	for(idx = 0; idx < ORDER; idx++)
	{
	
		read_uint64_n("out_data",result,2);
		fprintf(stdout,"\nCT: %d: %16lx%16lx",idx, result[0],result[1]);
	}
	
	PTHREAD_CANCEL(Sender);
	return(0);
}
