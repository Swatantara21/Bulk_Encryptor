#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <Pipes.h>
#include <SocketLib.h>
void In_wrap_daemon();
uint8_t Inv_Sbox_1(uint8_t s_in);
uint8_t Inv_Sbox_2(uint8_t s_in);
uint8_t Inv_Sbox_3(uint8_t s_in);
uint8_t Inv_Sbox_4(uint8_t s_in);
uint8_t MUL2(uint8_t mul_in);
void Out_wrap_daemon();
void c_block_daemon();
void d_block_daemon();
void e_block_daemon();
void global_storage_initializer_();
uint8_t sbox_mux_impl(uint8_t data_in);
void w_in_buff_daemon();
void w_out_buff_daemon();
