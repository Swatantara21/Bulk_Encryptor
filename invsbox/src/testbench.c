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

#define ORDER 64

uint32_t sent_value[ORDER];
//uint32_t key_values[ORDER];

void Exit(int sig)
{
	fprintf(stderr, "## Break! ##\n");
	exit(0);
}

	
void Sender()
{
	int idx;
	
	sent_value[1] = 0x00010203 ;
	sent_value[0] = 0x04050607 ;
	sent_value[2] = 0x08090a0b ;
	sent_value[3] = 0x0c0d0e0f ;
	sent_value[4] = 0x10111213 ;
	sent_value[5] = 0x14151617 ;
	sent_value[6] = 0x18191a1b ;
	sent_value[7] = 0x1c1d1e1f ;
	sent_value[8] = 0x20212223 ;
	sent_value[9] = 0x24252627 ;
	sent_value[10] = 0x28292a2b ;
	sent_value[11] = 0x2c2d2e2f ;
	sent_value[12] = 0x30313233 ;
	sent_value[13] = 0x34353637 ;
	sent_value[14] = 0x38393a3b ;
	sent_value[15] = 0x3c3d3e3f ;
	sent_value[16] = 0x40414243 ;
	sent_value[17] = 0x44454647 ;
	sent_value[18] = 0x48494a4b ;
	sent_value[19] = 0x4c4d4e4f ;
	sent_value[20] = 0x50515253 ;
	sent_value[21] = 0x54555657 ;
	sent_value[22] = 0x58595a5b ;
	sent_value[23] = 0x5c5d5e5f ;
	sent_value[24] = 0x60616263 ;
	sent_value[25] = 0x64656667 ;
	sent_value[26] = 0x68696a6b ;
	sent_value[27] = 0x6c6d6e6f ;
	sent_value[28] = 0x70717273 ;
	sent_value[29] = 0x74757677 ;
	sent_value[30] = 0x78797a7b ;
	sent_value[31] = 0x7c7d7e7f ;
	sent_value[32] = 0x80818283 ;
	sent_value[33] = 0x84858687 ;
	sent_value[34] = 0x88898a8b ;
	sent_value[35] = 0x8c8d8e8f ;
	sent_value[36] = 0x90919293 ;
	sent_value[37] = 0x94959697 ;
	sent_value[38] = 0x98999a9b ;
	sent_value[39] = 0x9c9d9e9f ;
	sent_value[40] = 0xa0a1a2a3 ;
	sent_value[41] = 0xa4a5a6a7 ;
	sent_value[42] = 0xa8a9aaab ;
	sent_value[43] = 0xacadaeaf ;
	sent_value[44] = 0xb0b1b2b3 ;
	sent_value[45] = 0xb4b5b6b7 ;
	sent_value[46] = 0xb8b9babb ;
	sent_value[47] = 0xbcbdbebf ;
	sent_value[48] = 0xc0c1c2c3 ;
	sent_value[49] = 0xc4c5c6c7 ;
	sent_value[50] = 0xc8c9cacb ;
	sent_value[51] = 0xcccdcecf ;
	sent_value[52] = 0xd0d1d2d3 ;
	sent_value[53] = 0xd4d5d6d7 ;
	sent_value[54] = 0xd8d9dadb ;
	sent_value[55] = 0xdcdddedf ;
	sent_value[56] = 0xe0e1e2e3 ;
	sent_value[57] = 0xe4e5e6e7 ;
	sent_value[58] = 0xe8e9eaeb ;
	sent_value[59] = 0xecedeeef ;
	sent_value[60] = 0xf0f1f2f3 ;
	sent_value[61] = 0xf4f5f6f7 ;
	sent_value[62] = 0xf8f9fafb ;
	sent_value[63] = 0xfcfdfeff ;
	
	write_uint32_n("in_data",sent_value,ORDER);
	//write_uint32_n("key",key_values,ORDER);
}

DEFINE_THREAD(Sender)

int main(int argc, char* argv[])
{
	uint32_t result[ORDER];
	signal(SIGINT,  Exit);
  	signal(SIGTERM, Exit);

	PTHREAD_DECL(Sender);
	PTHREAD_CREATE(Sender);

	uint8_t idx;
	
	read_uint32_n("out_data",result,ORDER);

	for(idx = 0; idx < ORDER; idx++)
	{
		fprintf(stdout,"Result = %x, Sent = %x.\n", result[idx],sent_value[idx]);
	}
	PTHREAD_CANCEL(Sender);
	return(0);
}
