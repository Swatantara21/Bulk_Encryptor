#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <signal.h>
#include <assert.h>
#include <time.h>
#include <string.h>


#include "timer.h"
#include "riffa.h"
#include "be.h"


void printUsage(char* exec_name)
{
	fprintf(stderr,"Usage: %s [-m <mode>] [-t <test-type>] [-i <input-file>] [-o <output-file>]\n", exec_name);
	fprintf(stderr,"    mode      = encrypt/decrypt/test\n");
	fprintf(stderr,"    test-type = kat\n");
	fprintf(stderr,"    block-size is the number of 128-bit words to be encrypted/decrypted\n");
}


int analyze_mode(char* mode, int* encrypt_mode, int* decrypt_mode, int* test_mode)
{
	int ret_val = 0;
	*encrypt_mode = 0;
	*decrypt_mode = 0;
	*test_mode = 0;
	if(strcmp(mode,"encrypt") == 0)
		*encrypt_mode = 1;
	else if(strcmp(mode,"decrypt") == 0)
		*decrypt_mode = 1;
	else if(strcmp(mode,"test") == 0)
		*test_mode = 1;
	else
	{
		fprintf(stderr,"Error: unknown mode %s\n", mode);
		ret_val = 1;
	}

	return(ret_val);
}

	
int opt;

int main(int argc, char** argv) 
{

	char* mode;
	char* infile_name;
	char* outfile_name;

	int encrypt_mode, decrypt_mode, test_mode;

	if (argc < 2) {
		printUsage(argv[0]);
		return -1;
	}

	while ((opt = getopt(argc, argv, "m:i:o:")) != -1) 
	{
		switch (opt)
		{
			case 'm':
				mode = strdup(optarg);
				break;
			case 'i':
				infile_name = strdup(optarg);
				break;
			case 'o':
				outfile_name = strdup(optarg);
				break;
			default:
				break;
		}	
	}

	int status = analyze_mode(mode, &encrypt_mode, &decrypt_mode, &test_mode);
	if(status)
		return(-1);

	uint64_t key[2];
	fprintf(stdout,"Key=?\n");
	fscanf (stdin, "%016llx %016llx", &(key[0]), &(key[1]));

	fpga_t* fpga;
	fpga = probe_fpgas();
	if(fpga == NULL)
	{
		fprintf(stderr,"Error: could not find an FPGA\n");
		return(-1);
	}


	if(test_mode)
	{
		run_kat_test (fpga);
	}
	else if(encrypt_mode)
	{
		encrypt_file(key, infile_name, outfile_name, fpga);
	}
	else if(decrypt_mode)
	{
		decrypt_file(key, infile_name, outfile_name, fpga);
	}

	
	fprintf(stderr,"Info: time taken = %f ms\n",(get_millisecond_time(1) - get_millisecond_time(0)));
				
	        
	fpga_close(fpga);
	return 0;
}
