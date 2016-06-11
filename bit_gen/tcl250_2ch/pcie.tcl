#########################################
######### VIVADO TCL SCRIPT FOR #########
##### SYNTHESIZING HARDWARE OF PCIE #####
#### GEN1 COMMUNICATION USING RIFFA  ####
#### 2.2.0 WITH VIRTEX 7 VC709 BOARD ####
#########################################
######## Author : Piyush P. Soni ########
#########################################


set AHIR_RELEASE $::env(AHIR_RELEASE)

############ RIFFA 2.2.0 HDL ############
read_verilog [glob ../hdl_250_2ch/riffa_hdl/*.v] 
read_verilog [glob ../hdl_250_2ch/*.v]


########## RIFFA TO CPU BRIDGE ##########
read_verilog ../hdl_250_2ch/riffa_cpu_bridge.v
read_vhdl ../hdl_250_2ch/DualClockedQueue.vhdl


############# CONSTRAINT FILE ###########
read_xdc ../constraint/KC705_TOP.xdc


############# IP CORE FILES #############
set_property part xc7k325tffg900-2 [current_project]
set_property board_part xilinx.com:kc705:part0:1.1 [current_project]

read_ip ../ip/pcie_core/PCIeGen1x8If64.xci
synth_ip -force [get_files ../ip/pcie_core/PCIeGen1x8If64.xci]

read_ip ../ip/dual_clock_fifo/fifo_generator_0.xci
synth_ip -force [get_files ../ip/dual_clock_fifo/fifo_generator_0.xci]


############### USER DESIGN #############
read_vhdl -library aHiR_ieee_proposed $AHIR_RELEASE/vhdl/aHiR_ieee_proposed.vhdl 
read_vhdl -library ahir $AHIR_RELEASE/vhdl/ahir.vhdl 
read_vhdl ../ahir_files/aesv2_2/ahir_system_global_package.vhdl
read_vhdl ../ahir_files/aesv2_2/ahir_system.vhdl


############### SYNTHESIZE ##############
#set outputDir ./results
#file mkdir $outputDir
synth_design -top KC705_Gen1x8If64 -part xc7k325tffg900-2 -include_dirs ../hdl_250_2ch/riffa_hdl/
# write_vhdl -force synth.vhdl
opt_design
place_design
route_design
report_drc -file post_route_drc.txt
report_timing_summary -file post_route_timing_summary.txt
report_utilization -file posr_route_utilization.txt
write_bitstream -force riffa_test.bit
