`timescale 1ns/1ns
module chnl_tester #(
	parameter C_PCI_DATA_WIDTH = 9'd32,
// local or lower level module parameters //
// declare this parameter for multiple values of CHNL_INDEX //
	parameter CHNL0_OUT_DATA_LEN = 32'd4
)
(
	input CLK,
	input sys_clk,
	input pcie_reset_n,
	input RST,

	output CHNL_RX_CLK, 
	input CHNL_RX, 
	output CHNL_RX_ACK, 
	input CHNL_RX_LAST, 
	input [31:0] CHNL_RX_LEN, 
	input [30:0] CHNL_RX_OFF, 
	input [C_PCI_DATA_WIDTH-1:0] CHNL_RX_DATA, 
	input CHNL_RX_DATA_VALID, 
	output CHNL_RX_DATA_REN,
	
	output CHNL_TX_CLK, 
	output CHNL_TX, 
	input CHNL_TX_ACK, 
	output CHNL_TX_LAST, 
	output [31:0] CHNL_TX_LEN, 
	output [30:0] CHNL_TX_OFF, 
	output [C_PCI_DATA_WIDTH-1:0] CHNL_TX_DATA, 
	output CHNL_TX_DATA_VALID, 
	input CHNL_TX_DATA_REN
);

wire [(C_PCI_DATA_WIDTH)-1:0] rData;
wire [(C_PCI_DATA_WIDTH)-1:0] tData;


////// AHIR SYSTEM SIGNALS //////

wire in_data_pipe_write_ack; 
wire in_data_pipe_write_req; 
wire out_data_pipe_read_req; 
wire out_data_pipe_read_ack; 


assign CHNL_RX_CLK = CLK;
assign CHNL_TX_CLK = CLK;
assign CHNL_TX_LAST = 1;
assign CHNL_TX_OFF[31*0 +:31] = 0;


// create multiple instantiation in a fashion similar as below //
// in each instance, replace CHNL_INDEX with the chnl no.//
// CHNL_INDEX is zero indexed //
riffa_cpu_bridge #(
.C_PCI_DATA_WIDTH(C_PCI_DATA_WIDTH),
.TX_DATA_LEN(CHNL0_OUT_DATA_LEN)
) interface0 (
	.CLK(CLK),
	.DOWNCLK(sys_clk),
	.RST(RST),
	.CHNL_RX(CHNL_RX),
	.CHNL_RX_ACK(CHNL_RX_ACK),			
	.CHNL_RX_LEN(CHNL_RX_LEN[32*0 +:32]),
	.CHNL_RX_DATA(CHNL_RX_DATA[C_PCI_DATA_WIDTH*0 +:C_PCI_DATA_WIDTH]),
	.CHNL_RX_DATA_VALID(CHNL_RX_DATA_VALID),
	.CHNL_RX_DATA_REN(CHNL_RX_DATA_REN),
	.CHNL_TX(CHNL_TX),
	.CHNL_TX_ACK(CHNL_TX_ACK),
	.CHNL_TX_LEN(CHNL_TX_LEN[32*0 +:32]),
	.CHNL_TX_DATA(CHNL_TX_DATA[C_PCI_DATA_WIDTH*0 +:C_PCI_DATA_WIDTH]),
	.CHNL_TX_DATA_VALID(CHNL_TX_DATA_VALID),
	.CHNL_TX_DATA_REN(CHNL_TX_DATA_REN),
	.in_data_pipe_write_data(rData[C_PCI_DATA_WIDTH*0 +:C_PCI_DATA_WIDTH]),
	.in_data_pipe_write_req(in_data_pipe_write_req),
	.in_data_pipe_write_ack(in_data_pipe_write_ack),
	.out_data_pipe_read_data(tData[C_PCI_DATA_WIDTH*0 +:C_PCI_DATA_WIDTH]),
	.out_data_pipe_read_req(out_data_pipe_read_req),
	.out_data_pipe_read_ack(out_data_pipe_read_ack)
);


//replace in_pipe_name with different input_pipe_names
//replace out_pipe_name with different output_pipe_names
//repeat instantiation of single data/req/ack pair for both rx and tx with different chnl_index numbers
ahir_system logic_block(
	.clk(sys_clk),
	.reset(RST),
	// pipe_signals_start
	.in_data_pipe_write_data(rData[C_PCI_DATA_WIDTH*0 +:C_PCI_DATA_WIDTH]),
	.in_data_pipe_write_req(in_data_pipe_write_req),
	.in_data_pipe_write_ack(in_data_pipe_write_ack) ,
	.out_data_pipe_read_data(tData[C_PCI_DATA_WIDTH*0 +:C_PCI_DATA_WIDTH]),
	.out_data_pipe_read_req(out_data_pipe_read_req),
	.out_data_pipe_read_ack(out_data_pipe_read_ack)
	// pipe_signals_end
);	

//assign tData[C_PCI_DATA_WIDTH*0 +:C_PCI_DATA_WIDTH] = rData[C_PCI_DATA_WIDTH*0 +:C_PCI_DATA_WIDTH];
//assign out_data_pipe_read_ack = in_data_pipe_write_req;
//assign in_data_pipe_write_ack = out_data_pipe_read_req;

endmodule
