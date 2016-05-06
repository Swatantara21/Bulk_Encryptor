#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <Pipes.h>
#include <SocketLib.h>
uint8_t Inv_Sbox_1(uint8_t s_in);
uint8_t Inv_Sbox_2(uint8_t s_in);
uint8_t Inv_Sbox_3(uint8_t s_in);
uint8_t Inv_Sbox_4(uint8_t s_in);
uint8_t MUL2(uint8_t mul_in);
void c_block_daemon_in();
void c_block_daemon_out();
void d_block_daemon();
void global_storage_initializer_();
