# ----------------------------------------------------------------------
# Copyright (c) 2016, The Regents of the University of California All
# rights reserved.
# 
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met:
# 
#     * Redistributions of source code must retain the above copyright
#       notice, this list of conditions and the following disclaimer.
# 
#     * Redistributions in binary form must reproduce the above
#       copyright notice, this list of conditions and the following
#       disclaimer in the documentation and/or other materials provided
#       with the distribution.
# 
#     * Neither the name of The Regents of the University of California
#       nor the names of its contributors may be used to endorse or
#       promote products derived from this software without specific
#       prior written permission.
# 
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL REGENTS OF THE
# UNIVERSITY OF CALIFORNIA BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
# BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
# OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
# ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
# TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
# USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
# DAMAGE.
# ----------------------------------------------------------------------
#----------------------------------------------------------------------------
# Filename:            KC705_Top.xdc
# Version:             1.00.a
# Verilog Standard:    Verilog-2001
# Description:         Xilinx Design Constraints for the KC705 (REVC) board.
# These constrain the PCIE_REFCLK, its DSBUF, LED Pins, and PCIE_RESET_N pin
#
# Author:              Dustin Richmond (@darichmond)
#-----------------------------------------------------------------------------#
#
###############################################################################
# User Time Names / User Time Groups / Time Specs
###############################################################################

###############################################################################
# User Physical Constraints
###############################################################################


###############################################################################
# Timing Constraints
###############################################################################


create_clock -period 10.000 -name pcie_refclk [get_pins refclk_ibuf/O]

###############################################################################
# Pinout and Related I/O Constraints
###############################################################################

#
# SYS reset (input) signal.  The sys_reset_n signal should be
# obtained from the PCI Express interface if possible.  For
# slot based form factors, a system reset signal is usually
# present on the connector.  For cable based form factors, a
# system reset signal may not be available.  In this case, the
# system reset signal must be generated locally by some form of
# supervisory circuit.  You may change the IOSTANDARD and LOC
# to suit your requirements and VCCO voltage banking rules.
# Some 7 series devices do not have 3.3 V I/Os available.
# Therefore the appropriate level shift is required to operate
# with these devices that contain only 1.8 V banks.
#

set_property IOSTANDARD LVCMOS25 [get_ports PCIE_RESET_N]
set_property PULLUP true [get_ports PCIE_RESET_N]
set_property LOC G25 [get_ports PCIE_RESET_N]

set_property IOSTANDARD LVCMOS15 [get_ports {LED[0]}]
set_property IOSTANDARD LVCMOS15 [get_ports {LED[1]}]
set_property IOSTANDARD LVCMOS15 [get_ports {LED[2]}]
set_property IOSTANDARD LVCMOS15 [get_ports {LED[3]}]

set_property LOC AB8 [get_ports {LED[0]}]
set_property LOC AA8 [get_ports {LED[1]}]
set_property LOC AC9 [get_ports {LED[2]}]
set_property LOC AB9 [get_ports {LED[3]}]

set_false_path -to [get_ports -filter {NAME=~LED*}]


###############################################################################
# Physical Constraints
###############################################################################
#
# SYS clock 100 MHz (input) signal. The sys_clk_p and sys_clk_n
# signals are the PCI Express reference clock. Virtex-7 GT
# Transceiver architecture requires the use of a dedicated clock
# resources (FPGA input pins) associated with each GT Transceiver.
# To use these pins an IBUFDS primitive (refclk_ibuf) is
# instantiated in user's design.
# Please refer to the Virtex-7 GT Transceiver User Guide
# (UG) for guidelines regarding clock resource selection.
#

set_property LOC IBUFDS_GTE2_X0Y1 [get_cells refclk_ibuf]

set_false_path -from [get_ports PCIE_RESET_N]


#################################################################
#Anshul:
#WARNING Rule violation (CFGBVS-1) Missing CFGBVS and CONFIG_VOLTAGE Design Properties
# - Neither the CFGBVS nor CONFIG_VOLTAGE voltage property is set in the current_design.
#  Configuration bank voltage select (CFGBVS) must be set to VCCO or GND, and CONFIG_VOLTAGE
# must be set to the correct configuration voltage, in order to determine the I/O voltage
# support for the pins in bank 0.  It is suggested to specify these either using the
# 'Edit Device Properties' function in the GUI or directly in the XDC file using the following syntax:
#
# set_property CFGBVS value1 [current_design]
 #where value1 is either VCCO or GND

#set_property CONFIG_VOLTAGE value2 [current_design]
 #where value2 is the voltage provided to configuration bank 0
#####
##  value 1 = VCCO value2 = 2.5 V
################
#### working correctly without removing above rule violation. 
 
######################################################################

###############################################################################
# End
###############################################################################
