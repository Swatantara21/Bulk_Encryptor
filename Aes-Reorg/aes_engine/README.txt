The aes_engine contains the following:

Aa: It consists of the .aa source files

tb : consists of the AHIR testbench.c file 



Makefile : It consists of the commands to convert the .aa files in the Aa folder to vhdl , through intermediate steps, in the AHIR tool-chain.

testbench_hw and ahir_system_test_bench : These are the two executables to be run in simultaneously to verify the implemented hardware design. 

All other files are the intermediate files from .aa to .vhdl in the ahir flow.

 
