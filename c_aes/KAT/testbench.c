#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define ORDER 128

uint32_t sent_values[4];
uint32_t key_values[5];
uint32_t key_values2[5];


	
int main()
{
	int idx,jdx,k;
	k=0;
	sent_values[0] = 0x80000000;
	sent_values[1] = 0;
	sent_values[2] = 0;
	sent_values[3] = 0;
	printf("\n PT: %8x %8x %8x %8x",sent_values[0],sent_values[1],sent_values[2],sent_values[3]);
	//write_uint32_n("in_data",sent_values,4);
	for(idx = 1; idx < ORDER; idx++)
	{	
		if(sent_values[k]==0xffffffff){
			k++;
			sent_values[k]= 0x80000000;
		}
		else
			sent_values[k] = sent_values[k]|(sent_values[k]/2);
		printf("\n PT: %8x %8x %8x %8x",sent_values[0],sent_values[1],sent_values[2],sent_values[3]);	
		/*sent_values[idx*4] = sent_values[idx*4 -4]|(sent_values[idx*4 -4]/2);
		write_uint32("in_data",sent_values[idx*4]);
		fprintf(stdout,"sent %d = %x\n",idx, sent_values[idx*4]);
		
		for (jdx = 1; jdx<4 ;jdx++){
			sent_values[idx*4+jdx] = 0;
			write_uint32("in_data",sent_values[idx*4 + jdx]);
			fprintf(stdout,"sent %d = %x\n",idx, sent_values[idx*4 + jdx]);
		}*/
	}
	/*for(idx = ORDER/8; idx < ORDER/4; idx++)
	{
		sent_values[idx*4] = sent_values[idx*4 -4]|(sent_values[idx*4 -4]/2);
		write_uint32("in_data",sent_values[idx*4]);
		fprintf(stdout,"sent %d = %x\n",idx, sent_values[idx*4]);
		
		for (jdx = 1; jdx<4 ;jdx++){
			sent_values[idx*4+jdx] = 0;
			write_uint32("in_data",sent_values[idx*4 + jdx]);
			fprintf(stdout,"sent %d = %x\n",idx, sent_values[idx*4 + jdx]);
		}
	}
	
	//write_uint32_n("in_data",sent_values,ORDER);*/
	return 0;
}



