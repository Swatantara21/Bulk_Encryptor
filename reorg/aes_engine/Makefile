# build software version of testbench (to check the "desired behaviour")
AHIR_INCLUDE=$(AHIR_RELEASE)/include
AHIR_LIB=$(AHIR_RELEASE)/lib
VHDL_LIB=$(AHIR_RELEASE)/vhdl
VHDL_VHPI_LIB=$(AHIR_RELEASE)/CtestBench/vhdl
FUNCTIONLIB=$(AHIR_RELEASE)/functionLibrary/
AA=./Aa
TB=./tb
all: HW 
TOVC:aalink aa2vc 
VC2VHDL: vc2vhdl  vhdlsim
AA2VHDLSIM: aa2vc  vc2vhdl  vhdlsim
TOVHDL:TOVC vc2vhdl


TOPMODULES=-T e_block_daemon -T c_block_daemon -T w_in_buff_daemon -T w_out_buff_daemon -T d_block_daemon -T In_wrap_daemon -T Out_wrap_daemon


# five steps from C to vhdl simulator.
HW: aalink aa2vc  vc2vhdl  vhdlsim

AA2VHDL: aa2vc vc2vhdl vhdlsim

# Aa to vC
aalink: $(AA)/e_block.aa $(AA)/sbox.aa $(AA)/enc_round.aa $(AA)/c_block.aa $(AA)/key_expnd.aa $(AA)/d_block.aa $(AA)/invsbox.aa $(AA)/dec_round.aa $(AA)/wrapper.aa
	AaLinkExtMem $(AA)/e_block.aa $(AA)/sbox.aa $(AA)/enc_round.aa $(AA)/c_block.aa $(AA)/key_expnd.aa $(AA)/d_block.aa $(AA)/invsbox.aa $(AA)/dec_round.aa $(AA)/wrapper.aa | vcFormat > prog.linked.aa
	AaOpt -B prog.linked.aa | vcFormat > prog.linked.opt.aa

aa2vc: prog.linked.opt.aa
	Aa2VC -O -C prog.linked.opt.aa | vcFormat > prog.vc

# vC to VHDL
vc2vhdl: prog.vc
	vc2vhdl -D -O -S 4 -I 2 -v -a -C -e ahir_system -w -s ghdl $(TOPMODULES) -f prog.vc -L $(FUNCTIONLIB)/fpu.list
	vhdlFormat < ahir_system_global_package.unformatted_vhdl > vhdl/ahir_system_global_package.vhdl
	vhdlFormat < ahir_system.unformatted_vhdl > vhdl/ahir_system.vhdl
	vhdlFormat < ahir_system_test_bench.unformatted_vhdl > vhdl/ahir_system_test_bench.vhdl
	rm -f *.unformatted_vhdl

# build testbench and ghdl executable
# note the use of libVhpi in building the testbench.
vhdlsim: vhdl/ahir_system.vhdl vhdl/ahir_system_test_bench.vhdl $(TB)/testbench.c vhdlCStubs.h vhdlCStubs.c
	gcc -c vhdlCStubs.c  -I$(TB) -I./ -I$(AHIR_INCLUDE)
	gcc -c $(TB)/testbench.c -I$(AHIR_INCLUDE) -I$(TB) -I./
	gcc -o testbench_hw testbench.o vhdlCStubs.o  -L$(AHIR_LIB) -lSocketLibPipeHandler -lpthread
	ghdl --clean
	ghdl --remove
	ghdl -i --work=GhdlLink  $(VHDL_LIB)/GhdlLink.vhdl
	ghdl -i --work=aHiR_ieee_proposed  $(VHDL_LIB)/aHiR_ieee_proposed.vhdl
	ghdl -i --work=ahir  $(VHDL_LIB)/ahir.vhdl
	ghdl -i --work=work vhdl/ahir_system_global_package.vhdl
	ghdl -i --work=work vhdl/ahir_system.vhdl
	ghdl -i --work=work vhdl/ahir_system_test_bench.vhdl
	ghdl -m --work=work -Wl,-L$(AHIR_LIB) -Wl,-lVhpi ahir_system_test_bench 

clean:
	rm -rf *.o* *.cf vhdl/*.*vhdl vhdlCStubs.* *.vcd in_data* out_data* testbench_sw testbench_hw ahir_system_test_bench vhpi.log *.aa *.vc *.lso xst *.ngc *_xmsgs *.xrpt pipeHandler.log *.srp *.ghw *.dot *.linked.aa *.vc *.linked.opt.aa .gsi.aa

PHONY: all clean	
