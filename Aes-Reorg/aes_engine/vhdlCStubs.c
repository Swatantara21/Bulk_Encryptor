#include <vhdlCStubs.h>
void In_wrap_daemon()
{
char buffer[1024];  char* ss;  sprintf(buffer, "call In_wrap_daemon ");
append_int(buffer,0); ADD_SPACE__(buffer);
append_int(buffer,0); ADD_SPACE__(buffer);
send_packet_and_wait_for_response(buffer,strlen(buffer)+1,"localhost",9999);
return;
}
uint8_t Inv_Sbox_1(uint8_t s_in)
{
char buffer[1024];  char* ss;  sprintf(buffer, "call Inv_Sbox_1 ");
append_int(buffer,1); ADD_SPACE__(buffer);
append_uint8_t(buffer,s_in); ADD_SPACE__(buffer);
append_int(buffer,1); ADD_SPACE__(buffer);
append_int(buffer,8); ADD_SPACE__(buffer);
send_packet_and_wait_for_response(buffer,strlen(buffer)+1,"localhost",9999);
uint8_t s_out = get_uint8_t(buffer,&ss);
return(s_out);
}
uint8_t Inv_Sbox_2(uint8_t s_in)
{
char buffer[1024];  char* ss;  sprintf(buffer, "call Inv_Sbox_2 ");
append_int(buffer,1); ADD_SPACE__(buffer);
append_uint8_t(buffer,s_in); ADD_SPACE__(buffer);
append_int(buffer,1); ADD_SPACE__(buffer);
append_int(buffer,8); ADD_SPACE__(buffer);
send_packet_and_wait_for_response(buffer,strlen(buffer)+1,"localhost",9999);
uint8_t s_out = get_uint8_t(buffer,&ss);
return(s_out);
}
uint8_t Inv_Sbox_3(uint8_t s_in)
{
char buffer[1024];  char* ss;  sprintf(buffer, "call Inv_Sbox_3 ");
append_int(buffer,1); ADD_SPACE__(buffer);
append_uint8_t(buffer,s_in); ADD_SPACE__(buffer);
append_int(buffer,1); ADD_SPACE__(buffer);
append_int(buffer,8); ADD_SPACE__(buffer);
send_packet_and_wait_for_response(buffer,strlen(buffer)+1,"localhost",9999);
uint8_t s_out = get_uint8_t(buffer,&ss);
return(s_out);
}
uint8_t Inv_Sbox_4(uint8_t s_in)
{
char buffer[1024];  char* ss;  sprintf(buffer, "call Inv_Sbox_4 ");
append_int(buffer,1); ADD_SPACE__(buffer);
append_uint8_t(buffer,s_in); ADD_SPACE__(buffer);
append_int(buffer,1); ADD_SPACE__(buffer);
append_int(buffer,8); ADD_SPACE__(buffer);
send_packet_and_wait_for_response(buffer,strlen(buffer)+1,"localhost",9999);
uint8_t s_out = get_uint8_t(buffer,&ss);
return(s_out);
}
uint8_t MUL2(uint8_t mul_in)
{
char buffer[1024];  char* ss;  sprintf(buffer, "call MUL2 ");
append_int(buffer,1); ADD_SPACE__(buffer);
append_uint8_t(buffer,mul_in); ADD_SPACE__(buffer);
append_int(buffer,1); ADD_SPACE__(buffer);
append_int(buffer,8); ADD_SPACE__(buffer);
send_packet_and_wait_for_response(buffer,strlen(buffer)+1,"localhost",9999);
uint8_t mul_out = get_uint8_t(buffer,&ss);
return(mul_out);
}
void Out_wrap_daemon()
{
char buffer[1024];  char* ss;  sprintf(buffer, "call Out_wrap_daemon ");
append_int(buffer,0); ADD_SPACE__(buffer);
append_int(buffer,0); ADD_SPACE__(buffer);
send_packet_and_wait_for_response(buffer,strlen(buffer)+1,"localhost",9999);
return;
}
void c_block_daemon()
{
char buffer[1024];  char* ss;  sprintf(buffer, "call c_block_daemon ");
append_int(buffer,0); ADD_SPACE__(buffer);
append_int(buffer,0); ADD_SPACE__(buffer);
send_packet_and_wait_for_response(buffer,strlen(buffer)+1,"localhost",9999);
return;
}
void d_block_daemon()
{
char buffer[1024];  char* ss;  sprintf(buffer, "call d_block_daemon ");
append_int(buffer,0); ADD_SPACE__(buffer);
append_int(buffer,0); ADD_SPACE__(buffer);
send_packet_and_wait_for_response(buffer,strlen(buffer)+1,"localhost",9999);
return;
}
void e_block_daemon()
{
char buffer[1024];  char* ss;  sprintf(buffer, "call e_block_daemon ");
append_int(buffer,0); ADD_SPACE__(buffer);
append_int(buffer,0); ADD_SPACE__(buffer);
send_packet_and_wait_for_response(buffer,strlen(buffer)+1,"localhost",9999);
return;
}
void global_storage_initializer_()
{
char buffer[1024];  char* ss;  sprintf(buffer, "call global_storage_initializer_ ");
append_int(buffer,0); ADD_SPACE__(buffer);
append_int(buffer,0); ADD_SPACE__(buffer);
send_packet_and_wait_for_response(buffer,strlen(buffer)+1,"localhost",9999);
return;
}
uint8_t sbox_mux_impl(uint8_t data_in)
{
char buffer[1024];  char* ss;  sprintf(buffer, "call sbox_mux_impl ");
append_int(buffer,1); ADD_SPACE__(buffer);
append_uint8_t(buffer,data_in); ADD_SPACE__(buffer);
append_int(buffer,1); ADD_SPACE__(buffer);
append_int(buffer,8); ADD_SPACE__(buffer);
send_packet_and_wait_for_response(buffer,strlen(buffer)+1,"localhost",9999);
uint8_t s_out = get_uint8_t(buffer,&ss);
return(s_out);
}
void w_in_buff_daemon()
{
char buffer[1024];  char* ss;  sprintf(buffer, "call w_in_buff_daemon ");
append_int(buffer,0); ADD_SPACE__(buffer);
append_int(buffer,0); ADD_SPACE__(buffer);
send_packet_and_wait_for_response(buffer,strlen(buffer)+1,"localhost",9999);
return;
}
void w_out_buff_daemon()
{
char buffer[1024];  char* ss;  sprintf(buffer, "call w_out_buff_daemon ");
append_int(buffer,0); ADD_SPACE__(buffer);
append_int(buffer,0); ADD_SPACE__(buffer);
send_packet_and_wait_for_response(buffer,strlen(buffer)+1,"localhost",9999);
return;
}
