#ifndef _be_h____
#define _be_h____
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <signal.h>
#include <assert.h>
#include <time.h>



//
// size of  encrypt/decrypt block.  This is chosen
// to be 1024x64 which corresponds to 512x128 bit
// blocks.
//
#define ED_BLOCK_SIZE_IN_DWORDS             1024
#define ED_BLOCK_SIZE_IN_BYTES              (1024*8)

//
// we use channel 0
//
#define FPGA_CHANNEL_ID				0

//
//  probe-fpga and return index of first FPGA that works.
//  
fpga_t* probe_fpgas ();

//
// reset fpga with specified id.
//
int reset_fpga (fpga_t* f);

//
// read 1024 dwords from file.
//
int read_block_from_file (FILE* fp, uint64_t* read_buffer);

//
// dump 1024 dwords to file.
//
int write_block_to_file (FILE* fp, uint64_t* read_buffer);

//
// encrypts block in ptext_block with key, places result in cipher_block.
//    (note ptext_block and cipher_block should point to arrays with enough space)
//
// returns 0 on success.
//
int encrypt_block(uint64_t* key, uint64_t* ptext_block, uint64_t* cipher_block, fpga_t* fpga);

//
// decrypts block in cipher_block with key, places result in ptext_block.
//    (note ptext_block and cipher_block should point to arrays with enough space)
//
// returns 0 on success.
//
int decrypt_block(uint64_t* key, uint64_t* cipher_block, uint64_t* ptext_block, fpga_t* fpga);



//
// file related stuff.
//
int encrypt_file(uint64_t* key, char* in_file, char* out_file, fpga_t* fpga);
int decrypt_file(uint64_t* key, char* in_file, char* out_file, fpga_t* fpga);

//
// run kat test as per AES standard.
//
int run_kat_test (fpga_t* fpga);

#endif
