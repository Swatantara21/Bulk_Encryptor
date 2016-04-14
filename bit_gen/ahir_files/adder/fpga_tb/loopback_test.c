#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <riffa.h>


int main(int argc, char* argv[])
{
	fpga_t* fpga = fpga_open(0);
	if(fpga == NULL)
	{
		fprintf(stderr,"Error: could not open FPGA.\n");
		return(1);
	}
	fprintf(stderr,"Info: opened FPGA.\n");
	fpga_reset(fpga);
	fprintf(stderr,"Info: reset the FPGA.\n");

	uint64_t A, B;
	int ret_stat, I;
	for(I = 0; I  < 16; I++)
	{
		A = I;
		ret_stat = fpga_send(fpga, 0, &A, 2,0,1, 0);	
		ret_stat = fpga_recv(fpga, 0, &B, 2, 5000);
		fprintf(stderr,"A = %llx, B = %llx\n", A, B);
	
		A = (~A);
		ret_stat = fpga_send(fpga, 0, &A, 2,0,1, 0);	
		ret_stat = fpga_recv(fpga, 0, &B, 2, 5000);
		fprintf(stderr,"A = %llx, B = %llx\n", A, B);
	}

	fpga_close(fpga);
	fprintf(stderr,"Info: closed the FPGA.\n");

	return(0);
}
