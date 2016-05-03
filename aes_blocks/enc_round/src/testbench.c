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

#define ORDER 4

uint32_t sent_values[ORDER];
uint32_t key_values[4];

void Exit(int sig)
{
	fprintf(stderr, "## Break! ##\n");
	exit(0);
}

	
void Sender()
{
	int idx,jdx;
	sent_values[0] = 0x00000080;
	sent_values[1] = 0;
	sent_values[2] = 0;
	sent_values[3] = 0;
	for(idx = 1; idx < ORDER/4; idx++)
	{
		sent_values[idx*4] = sent_values[idx*4 -4]+1;
		for (jdx = 0; jdx<4 ;jdx++)
		sent_values[idx*4+jdx] = 0;
		
		fprintf(stdout,"sent %d = %x\n",idx, sent_values[idx]);
	}
	
	write_uint32_n("in_data",sent_values,ORDER);
}

void Sender_key()
{
	int idx;
	
	for(idx =0 ; idx < 4; idx++)
	{
		uint32_t k = 0;
		key_values[idx] = k;
		fprintf(stdout,"key %d = %x\n",idx, key_values[idx]);
	}
	write_uint32_n("key_in",key_values,ORDER);
}

DEFINE_THREAD(Sender)

DEFINE_THREAD(Sender_key)

int main(int argc, char* argv[])
{	
	uint32_t result[ORDER];
	signal(SIGINT,  Exit);
  	signal(SIGTERM, Exit);

	PTHREAD_DECL(Sender);
	PTHREAD_CREATE(Sender);
	
	PTHREAD_DECL(Sender_key);
	PTHREAD_CREATE(Sender_key);

	uint8_t idx, jdx;
	
	/*read_uint32_n("out_data",result,4);
	for(idx = 0; idx < 4; idx++)
	{
		fprintf(stdout,"Result = %x, Sent = %x.\n", result[idx],sent_values[idx]);
		//fprintf(stdout,"Result = %x\n", result[idx]);
	}*/
	for(idx = 0; idx < ORDER; idx++)
	{
	
		result[idx] = read_uint32("out_data");
		//fprintf(stdout,"Result = %x, Sent = %x, Key = %x.\n", result[idx],sent_values[idx],key_values[idx]);
		fprintf(stdout,"Result = %x\n", result[idx]);
	}
	PTHREAD_CANCEL(Sender);
	PTHREAD_CANCEL(Sender_key);
	return(0);
}
