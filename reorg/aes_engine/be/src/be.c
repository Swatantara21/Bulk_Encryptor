#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <signal.h>
#include <assert.h>
#include <time.h>


#include "timer.h"
#include "riffa.h"
#include "be.h"


fpga_t* probe_fpgas ()
{
	fpga_info_list info;
	fpga_t* fpga = NULL;

	// List FPGA info
	// Populate the fpga_info_list struct
	if (fpga_list(&info) != 0) {
		printf("Error populating fpga_info_list\n");
		return NULL;
	}
	printf("Number of devices: %d\n", info.num_fpgas);
	int i;
	for (i = 0; i < info.num_fpgas; i++) {
		printf("%d: id:%d\n", i, info.id[i]);
		printf("%d: num_chnls:%d\n", i, info.num_chnls[i]);
		printf("%d: name:%s\n", i, info.name[i]);
		printf("%d: vendor id:%04X\n", i, info.vendor_id[i]);
		printf("%d: device id:%04X\n", i, info.device_id[i]);
		
		fpga = fpga_open(info.id[0]);
		if(fpga != NULL)
		{
			fprintf(stderr, "%d: opened device id:%04X\n", i, info.device_id[i]);
			break;
		}
		else
		{
			fprintf(stderr, "%d: could not open device id:%04X\n", i, info.device_id[i]);
		}
	}
	return(fpga);
}

int reset_fpga (fpga_t* fpga)
{
	if(fpga != NULL)
		fpga_reset(fpga);
	return(0);
}

int read_block_from_file (FILE* fp, uint64_t* read_buffer)
{
	int read_count = fread((void*) read_buffer, ED_BLOCK_SIZE_IN_BYTES, 1, fp);
	int ret_val = ((read_count == EOF) || (read_count < ED_BLOCK_SIZE_IN_BYTES));
	return(ret_val);
}

int write_block_to_file (FILE* fp, uint64_t* read_buffer)
{
	int write_count = fwrite((void*) read_buffer, ED_BLOCK_SIZE_IN_BYTES, 1, fp);
	int ret_val = (write_count < ED_BLOCK_SIZE_IN_BYTES);
	return(ret_val);
}

int encrypt_block(uint64_t* key, uint64_t* ptext_block, uint64_t* cipher_block, fpga_t* fpga_id)
{
	uint64_t command_buffer[4];
	int err = 0;
	
	command_buffer[0] = 0x0;
	if(key != NULL)
	{
		command_buffer[1] = 0x8800000000000000 + 511;
		command_buffer[2] = key[0];
		command_buffer[3] = key[1];
		int sent_cmd = fpga_send (fpga_id, FPGA_CHANNEL_ID, command_buffer,8,0,0,25000);
		if(sent_cmd < 8)
		{
			fprintf(stderr,"Error:encrypt_block: short send of command (%d instead of 8)\n", sent_cmd);
			err = 1;
		}
	}
	else
	{
		command_buffer[1] = 0x8000000000000000 + 511;
		int sent_cmd = fpga_send (fpga_id, FPGA_CHANNEL_ID, command_buffer,4,0,0,25000);
		if(sent_cmd < 4)
		{
			fprintf(stderr,"Error:encrypt_block: short send of command (%d instead of 4)\n", sent_cmd);
			err = 1;
		}
	}

	if (!err)
	{
		int sent_count = fpga_send (fpga_id, FPGA_CHANNEL_ID, ptext_block, (2*ED_BLOCK_SIZE_IN_DWORDS),0,1,25000);
		if(sent_count < (2*ED_BLOCK_SIZE_IN_DWORDS))
		{
			fprintf(stderr,"Error:encrypt_block: short send of plain-text (%d instead of %d)\n", sent_count, (2*ED_BLOCK_SIZE_IN_DWORDS));
			err = 1;
		}
		if(!err)
		{
			int recv_count = 	fpga_recv(fpga_id, FPGA_CHANNEL_ID, cipher_block, (2*ED_BLOCK_SIZE_IN_DWORDS),25000);
			if(recv_count < (2*ED_BLOCK_SIZE_IN_DWORDS))
			{
				fprintf(stderr,"Error:encrypt_block: short recv of cipher-text (%d instead of %d)\n", recv_count, (2*ED_BLOCK_SIZE_IN_DWORDS));
				err = 1;
			}
		}
	}

	return(err);
}

int decrypt_block(uint64_t* key, uint64_t* cipher_block, uint64_t* ptext_block, fpga_t* fpga)
{
	uint64_t command_buffer[4];
	int err = 0;
	
	command_buffer[0] = 0x0;
	if(key != NULL)
	{
		command_buffer[1] = 0x0800000000000000 + 511;
		command_buffer[2] = key[0];
		command_buffer[3] = key[1];
		int sent_cmd = fpga_send (fpga, FPGA_CHANNEL_ID, command_buffer,8,0,0,25000);
		if(sent_cmd < 8)
		{
			fprintf(stderr,"Error:decrypt_block: short send of command (%d instead of 8)\n", sent_cmd);
			err = 1;
		}
	}
	else
	{
		command_buffer[1] = 0x8000000000000000 + 511;
		int sent_cmd = fpga_send (fpga, FPGA_CHANNEL_ID, command_buffer,4,0,0,25000);
		if(sent_cmd < 4)
		{
			fprintf(stderr,"Error:decrypt_block: short send of command (%d instead of 4)\n", sent_cmd);
			err = 1;
		}
	}

	if (!err)
	{
		int sent_count = fpga_send (fpga, FPGA_CHANNEL_ID, cipher_block, (2*ED_BLOCK_SIZE_IN_DWORDS),0,1,25000);
		if(sent_count < (2*ED_BLOCK_SIZE_IN_DWORDS))
		{
			fprintf(stderr,"Error:decrypt_block: short send of cipher-text (%d instead of %d)\n", sent_count, (2*ED_BLOCK_SIZE_IN_DWORDS));
			err = 1;
		}
		if(!err)
		{
			int recv_count = 	fpga_recv(fpga, FPGA_CHANNEL_ID, ptext_block, (2*ED_BLOCK_SIZE_IN_DWORDS),25000);
			if(recv_count < (2*ED_BLOCK_SIZE_IN_DWORDS))
			{
				fprintf(stderr,"Error:decrypt_block: short recv of plain-text (%d instead of %d)\n", recv_count, (2*ED_BLOCK_SIZE_IN_DWORDS));
				err = 1;
			}
		}
	}

	return(err);
}

int encrypt_file(uint64_t* key, char* in_file, char* out_file, fpga_t* fpga)
{
	sample_timer(0);
	FILE* ifp = fopen(in_file,"rb");
	if(ifp == NULL)
	{
		fprintf(stderr,"Error:encrypt_file: could not open input file %s\n", in_file);
		return(-1);
	}
	FILE* ofp = fopen(out_file,"wb");
	if(ofp == NULL)
	{
		fprintf(stderr,"Error:encrypt_file: could not open output file %s\n", out_file);
		return(-1);
	}

	uint64_t ptext_block[ED_BLOCK_SIZE_IN_DWORDS];	
	uint64_t cipher_block[ED_BLOCK_SIZE_IN_DWORDS];	
	int block_count = 0;
	while(1)
	{
		int rstatus = read_block_from_file(ifp, ptext_block);
		encrypt_block(((block_count == 0) ? key : NULL), ptext_block, cipher_block, fpga);
		int wstatus = write_block_to_file(ofp, cipher_block);

		block_count++;

		if(rstatus || wstatus)
		{
			fprintf(stderr,"Info:encrypt_file: finished encrypting %d blocks\n",  block_count);
			break;
		}
		
	}
	sample_timer(1);
}

int decrypt_file(uint64_t* key, char* in_file, char* out_file, fpga_t* fpga)
{
	sample_timer(0);
	FILE* ifp = fopen(in_file,"rb");
	if(ifp == NULL)
	{
		fprintf(stderr,"Error:decrypt_file: could not open input file %s\n", in_file);
		return(-1);
	}
	FILE* ofp = fopen(out_file,"wb");
	if(ofp == NULL)
	{
		fprintf(stderr,"Error:decrypt_file: could not open output file %s\n", out_file);
		return(-1);
	}

	uint64_t ptext_block[ED_BLOCK_SIZE_IN_DWORDS];	
	uint64_t cipher_block[ED_BLOCK_SIZE_IN_DWORDS];	
	int block_count = 0;
	while(1)
	{
		int rstatus = read_block_from_file(ifp, cipher_block);
		decrypt_block(((block_count == 0) ? key : NULL), cipher_block, ptext_block, fpga);
		int wstatus = write_block_to_file(ofp, ptext_block);

		block_count++;

		if(rstatus || wstatus)
		{
			fprintf(stderr,"Info:decrypt_file: finished decrypting %d blocks\n",  block_count);
			break;
		}
		
	}
	sample_timer(1);
}

//
// run KAT test.
int run_kat_test (fpga_t* fpga)
{

	// Generate KAT in buffer.
	uint64_t ptext_buffer[ED_BLOCK_SIZE_IN_DWORDS];
	uint64_t ctext_buffer[ED_BLOCK_SIZE_IN_DWORDS];
	uint64_t ptext_buffer_2[ED_BLOCK_SIZE_IN_DWORDS];

	int k=0;
	int idx;

	ptext_buffer[0] = 0x8000000000000000;
	ptext_buffer[1] = 0x0000000000000000;

	for (idx =2; idx < ED_BLOCK_SIZE_IN_DWORDS/2; idx=idx+2){
		ptext_buffer[idx]=ptext_buffer[idx-2];
		ptext_buffer[idx+1]=ptext_buffer[idx-1];
		if(ptext_buffer[idx+k-2]==0xffffffffffffffff){
			k++;
			ptext_buffer[idx+k]= 0x8000000000000000;
		}
		else
			ptext_buffer[idx+k] = ptext_buffer[idx+k]|(ptext_buffer[idx+k]/2);
	}
	for (idx =0; idx< ED_BLOCK_SIZE_IN_DWORDS/2; idx++)
		ptext_buffer[idx+(ED_BLOCK_SIZE_IN_DWORDS/2)]=ptext_buffer[idx];

	uint64_t key[2];
	key[0] = 0;
	key[1] = 1;

	
	sample_timer(0);
	encrypt_block(key,  ptext_buffer, ctext_buffer,   fpga);
	decrypt_block(NULL, ctext_buffer, ptext_buffer_2,  fpga);
	sample_timer(1);

	fprintf(stdout,"KAT test ptext ctext ptext2\n");
	for (idx =0; idx< ED_BLOCK_SIZE_IN_DWORDS; idx += 2)
	{
		fprintf(stdout,"%016llx%016llx %016llx%016llx %016llx016llx\n",
			ptext_buffer[idx], ptext_buffer[idx+1],
			ctext_buffer[idx], ctext_buffer[idx+1],
			ptext_buffer_2[idx], ptext_buffer_2[idx+1]);
	}

	return(0);
}

