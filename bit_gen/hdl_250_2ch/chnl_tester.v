`timescale 1ns/1ns
module chnl_tester #(
	parameter C_PCI_DATA_WIDTH = 9'd32,
// local or lower level module parameters //
// declare this parameter for multiple values of CHNL_INDEX //
	parameter CHNL0_OUT_DATA_LEN = 32'd4,
	parameter CHNL1_OUT_DATA_LEN = 32'd2

)
(
	input CLK,
	input sys_clk,
	input pcie_reset_n,
	input RST,

	output CHNL_RX0_CLK, 
	input CHNL_RX0, 
	output CHNL_RX0_ACK, 
	input CHNL_RX0_LAST, 
	input [31:0] CHNL_RX0_LEN, 
	input [30:0] CHNL_RX0_OFF, 
	input [C_PCI_DATA_WIDTH-1:0] CHNL_RX0_DATA, 
	input CHNL_RX0_DATA_VALID, 
	output CHNL_RX0_DATA_REN,
	
	output CHNL_TX0_CLK, 
	output CHNL_TX0, 
	input CHNL_TX0_ACK, 
	output CHNL_TX0_LAST, 
	output [31:0] CHNL_TX0_LEN, 
	output [30:0] CHNL_TX0_OFF, 
	output [C_PCI_DATA_WIDTH-1:0] CHNL_TX0_DATA, 
	output CHNL_TX0_DATA_VALID, 
	input CHNL_TX0_DATA_REN,
	
	output CHNL_RX1_CLK, 
	input CHNL_RX1, 
	output CHNL_RX1_ACK, 
	input CHNL_RX1_LAST, 
	input [31:0] CHNL_RX1_LEN, 
	input [30:0] CHNL_RX1_OFF, 
	input [C_PCI_DATA_WIDTH-1:0] CHNL_RX1_DATA, 
	input CHNL_RX1_DATA_VALID, 
	output CHNL_RX1_DATA_REN,
	
	output CHNL_TX1_CLK, 
	output CHNL_TX1, 
	input CHNL_TX1_ACK, 
	output CHNL_TX1_LAST, 
	output [31:0] CHNL_TX1_LEN, 
	output [30:0] CHNL_TX1_OFF, 
	output [C_PCI_DATA_WIDTH-1:0] CHNL_TX1_DATA, 
	output CHNL_TX1_DATA_VALID, 
	input CHNL_TX1_DATA_REN
);

wire [(C_PCI_DATA_WIDTH)-1:0] r0Data;
wire [(C_PCI_DATA_WIDTH)-1:0] t0Data;

wire [(C_PCI_DATA_WIDTH)-1:0] r1Data;
wire [(C_PCI_DATA_WIDTH)-1:0] t1Data;


////// AHIR SYSTEM SIGNALS //////

wire r0Data_write_ack; 
wire r0Data_write_req; 
wire t0Data_read_req; 
wire t0Data_read_ack; 

wire r1Data_write_ack; 
wire r1Data_write_req; 
wire t1Data_read_req; 
wire t1Data_read_ack; 

assign CHNL_RX1_CLK = CLK;
assign CHNL_TX1_CLK = CLK;
assign CHNL_TX1_LAST = 1;
assign CHNL_TX1_OFF[31*0 +:31] = 0;

assign CHNL_RX0_CLK = CLK;
assign CHNL_TX0_CLK = CLK;
assign CHNL_TX0_LAST = 1;
assign CHNL_TX0_OFF[31*0 +:31] = 0;


// create multiple instantiation in a fashion similar as below //
// in each instance, replace CHNL_INDEX with the chnl no.//
// CHNL_INDEX is zero indexed //
// also add the interfaceing signals of the channel in the input/output ports of module//
// remember to do the corresponding changes in the top module file where the chnl_tester is called//

riffa_cpu_bridge #(
.C_PCI_DATA_WIDTH(C_PCI_DATA_WIDTH),
.TX_DATA_LEN(CHNL0_OUT_DATA_LEN)
) interface0 (
	.CLK(CLK),
	.DOWNCLK(CLK),
	.RST(RST),
	.CHNL_RX(CHNL_RX0),
	.CHNL_RX_ACK(CHNL_RX0_ACK),			
	.CHNL_RX_LEN(CHNL_RX0_LEN[32*0 +:32]),
	.CHNL_RX_DATA(CHNL_RX0_DATA[C_PCI_DATA_WIDTH*0 +:C_PCI_DATA_WIDTH]),
	.CHNL_RX_DATA_VALID(CHNL_RX0_DATA_VALID),
	.CHNL_RX_DATA_REN(CHNL_RX0_DATA_REN),
	.CHNL_TX(CHNL_TX0),
	.CHNL_TX_ACK(CHNL_TX0_ACK),
	.CHNL_TX_LEN(CHNL_TX0_LEN[32*0 +:32]),
	.CHNL_TX_DATA(CHNL_TX0_DATA[C_PCI_DATA_WIDTH*0 +:C_PCI_DATA_WIDTH]),
	.CHNL_TX_DATA_VALID(CHNL_TX0_DATA_VALID),
	.CHNL_TX_DATA_REN(CHNL_TX0_DATA_REN),
	.in_data_pipe_write_data(r0Data[C_PCI_DATA_WIDTH*0 +:C_PCI_DATA_WIDTH]),
	.in_data_pipe_write_req(r0Data_write_req),
	.in_data_pipe_write_ack(r0Data_write_ack),
	.out_data_pipe_read_data(t0Data[C_PCI_DATA_WIDTH*0 +:C_PCI_DATA_WIDTH]),
	.out_data_pipe_read_req(t0Data_read_req),
	.out_data_pipe_read_ack(t0Data_read_ack)
);


riffa_cpu_bridge #(
.C_PCI_DATA_WIDTH(C_PCI_DATA_WIDTH),
.TX_DATA_LEN(CHNL1_OUT_DATA_LEN)
) interface1 (
	.CLK(CLK),
	.DOWNCLK(CLK),
	.RST(RST),
	.CHNL_RX(CHNL_RX1),
	.CHNL_RX_ACK(CHNL_RX1_ACK),			
	.CHNL_RX_LEN(CHNL_RX1_LEN[32*0 +:32]),
	.CHNL_RX_DATA(CHNL_RX1_DATA[C_PCI_DATA_WIDTH*0 +:C_PCI_DATA_WIDTH]),
	.CHNL_RX_DATA_VALID(CHNL_RX1_DATA_VALID),
	.CHNL_RX_DATA_REN(CHNL_RX1_DATA_REN),
	.CHNL_TX(CHNL_TX1),
	.CHNL_TX_ACK(CHNL_TX1_ACK),
	.CHNL_TX_LEN(CHNL_TX1_LEN[32*0 +:32]),
	.CHNL_TX_DATA(CHNL_TX1_DATA[C_PCI_DATA_WIDTH*0 +:C_PCI_DATA_WIDTH]),
	.CHNL_TX_DATA_VALID(CHNL_TX1_DATA_VALID),
	.CHNL_TX_DATA_REN(CHNL_TX1_DATA_REN),
	.in_data_pipe_write_data(r1Data[C_PCI_DATA_WIDTH*0 +:C_PCI_DATA_WIDTH]),
	.in_data_pipe_write_req(r1Data_write_req),
	.in_data_pipe_write_ack(r1Data_write_ack),
	.out_data_pipe_read_data(t1Data[C_PCI_DATA_WIDTH*0 +:C_PCI_DATA_WIDTH]),
	.out_data_pipe_read_req(t1Data_read_req),
	.out_data_pipe_read_ack(t1Data_read_ack)
);


//replace in_pipe_name with different input_pipe_names
//replace out_pipe_name with different output_pipe_names
//repeat instantiation of single data/req/ack pair for both rx and tx with different chnl_index numbers
ahir_system logic_block(
	.clk(CLK),
	.reset(RST),
	// pipe_signals_start
	.in_data_pipe_write_data(r0Data[C_PCI_DATA_WIDTH*0 +:C_PCI_DATA_WIDTH]),
	.in_data_pipe_write_req(r0Data_write_req),
	.in_data_pipe_write_ack(r0Data_write_ack),
	.out_data_pipe_read_data(t0Data[C_PCI_DATA_WIDTH*0 +:C_PCI_DATA_WIDTH]),
	.out_data_pipe_read_req(t0Data_read_req),
	.out_data_pipe_read_ack(t0Data_read_ack),
	.cmd_in_pipe_write_data(r1Data[C_PCI_DATA_WIDTH*0 +:C_PCI_DATA_WIDTH]),
	.cmd_in_pipe_write_req(r1Data_write_req),
	.cmd_in_pipe_write_ack(r1Data_write_ack) ,
	.status_out_pipe_read_data(t1Data[C_PCI_DATA_WIDTH*0 +:C_PCI_DATA_WIDTH]),
	.status_out_pipe_read_req(t1Data_read_req),
	.status_out_pipe_read_ack(t1Data_read_ack)
	// pipe_signals_end
);	
//assign tData[C_PCI_DATA_WIDTH*0 +:C_PCI_DATA_WIDTH] = rData[C_PCI_DATA_WIDTH*0 +:C_PCI_DATA_WIDTH];
//assign out_data_pipe_read_ack = in_data_pipe_write_req;
//assign in_data_pipe_write_ack = out_data_pipe_read_req;

endmodule
