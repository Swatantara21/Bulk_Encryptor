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

#define ORDER 20

uint32_t sent_values[4];
uint32_t key_values[5];

void Exit(int sig)
{
	fprintf(stderr, "## Break! ##\n");
	exit(0);
}

	
void Sender()
{
	int idx,jdx,k;
	k=0;
	sent_values[0] = 0x80000000;
	sent_values[1] = 0;
	sent_values[2] = 0;
	sent_values[3] = 0;
	//printf("\nPT: %8x %8x %8x %8x",sent_values[0],sent_values[1],sent_values[2],sent_values[3]);
	write_uint32_n("in_data",sent_values,4);
	for(idx = 1; idx < ORDER; idx++)
	{	
		if(sent_values[k]==0xffffffff){
			k++;
			sent_values[k]= 0x80000000;
		}
		else
			sent_values[k] = sent_values[k]|(sent_values[k]/2);
			
		write_uint32_n("in_data",sent_values,4);
		//printf("\nPT: %8x %8x %8x %8x",sent_values[0],sent_values[1],sent_values[2],sent_values[3]);	
	}
}

void Sender_key()
{
	int idx;
	key_values[0] = 1;
	write_uint32("key_in",key_values[0]);
	//fprintf(stdout,"key %d = %x\n",idx, key_values[0]);
	for(idx =1 ; idx < 5; idx++)
	{
		uint32_t k = 0;
		key_values[idx] = k;
		write_uint32("key_in",key_values[idx]);
		//fprintf(stdout,"key %d = %x\n",idx, key_values[idx]);
	}
	write_uint32_n("key_in",key_values,5);
	//fprintf(stdout,"\nkey %d = %8x %8x %8x %8x %8x\n",idx, key_values[0],key_values[1],key_values[2],key_values[3],key_values[4]);
}

DEFINE_THREAD(Sender)

DEFINE_THREAD(Sender_key)

int main(int argc, char* argv[])
{	
	uint32_t result[4];
	signal(SIGINT,  Exit);
  	signal(SIGTERM, Exit);

	PTHREAD_DECL(Sender);
	PTHREAD_CREATE(Sender);
	
	//PTHREAD_DECL(Sender_key);
	//PTHREAD_CREATE(Sender_key);

	uint8_t idx, jdx;
	
	/*read_uint32_n("out_data",result,4);
	for(idx = 0; idx < 4; idx++)
	{
		fprintf(stdout,"Result = %x, Sent = %x.\n", result[idx],sent_values[idx]);
		//fprintf(stdout,"Result = %x\n", result[idx]);
	}*/
	for(idx = 0; idx < ORDER; idx++)
	{
	
		read_uint32_n("out_data",result,4);
		//result[idx] = read_uint32("out_data");
		//fprintf(stdout,"Result = %x, Sent = %x, Key = %x.\n", result[idx],sent_values[idx],key_values[idx]);
		fprintf(stdout,"\n%8x %8x %8x %8x", result[0],result[1],result[2],result[3]);
	}
	
	
	PTHREAD_CANCEL(Sender);
	//PTHREAD_CANCEL(Sender_key);
	return(0);
}
