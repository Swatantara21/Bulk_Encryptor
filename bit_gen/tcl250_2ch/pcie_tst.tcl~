set AHIR_RELEASE $::env(AHIR_RELEASE)

############ RIFFA 2.2.0 HDL ############
read_verilog [glob ../hdl/riffa_hdl/*.v] 
read_verilog [glob ../hdl/*.v]


########## RIFFA TO CPU BRIDGE ##########
read_verilog ../hdl/riffa_cpu_bridge.v
read_vhdl ../hdl/DualClockedQueue.vhdl


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
