# build software version of testbench (to check the "desired behaviour")
AHIR_INCLUDE=$(AHIR_RELEASE)/include
AHIR_LIB=$(AHIR_RELEASE)/lib
VHDL_LIB=$(AHIR_RELEASE)/vhdl
VHDL_VHPI_LIB=$(AHIR_RELEASE)/CtestBench/vhdl
FUNCTIONLIB=$(AHIR_RELEASE)/functionLibrary/
SRC=./src
AA2VHDL: aalink aa2vc vc2vhdl vhdlsim
TOPMODULES=-T loopback_daemon 


ASYS = ahir_system
# Aa to vC
aalink: loopback.Aa 
	AaLinkExtMem loopback.Aa | vcFormat > loopback.linked.aa
	AaOpt -B loopback.linked.aa | vcFormat > loopback.linked.opt.aa

aa2vc: loopback.linked.opt.aa
	Aa2VC -O -C loopback.linked.opt.aa | vcFormat > loopback.vc

# vC to VHDL
vc2vhdl: loopback.vc
	vc2vhdl  -O -S 4 -I 2 -v -a -C -e $(ASYS) -w -s ghdl $(TOPMODULES) -f loopback.vc 
	vhdlFormat < $(ASYS)_global_package.unformatted_vhdl > $(ASYS)_global_package.vhdl
	vhdlFormat < $(ASYS).unformatted_vhdl > $(ASYS).vhdl
	vhdlFormat < $(ASYS)_test_bench.unformatted_vhdl > $(ASYS)_test_bench.vhdl
	rm *.unformatted_vhdl

# build testbench and ghdl executable
# note the use of libVhpi in building the testbench.
vhdlsim: ahir_system.vhdl ahir_system_test_bench.vhdl $(SRC)/testbench.c vhdlCStubs.h vhdlCStubs.c
	gcc -c vhdlCStubs.c -I$(SRC) -I./ -I$(AHIR_INCLUDE)
	gcc -c $(SRC)/testbench.c -I$(AHIR_INCLUDE) -I$(SRC) -I./
	gcc -o testbench_hw testbench.o vhdlCStubs.o  -L$(AHIR_LIB) -lSocketLib -lpthread
	ghdl --clean
	ghdl --remove
	ghdl -i --work=GhdlLink  $(VHDL_LIB)/GhdlLink.vhdl
	ghdl -i --work=aHiR_ieee_proposed  $(VHDL_LIB)/aHiR_ieee_proposed.vhdl
	ghdl -i --work=ahir  $(VHDL_LIB)/ahir.vhdl
	ghdl -i --work=work ahir_system_global_package.vhdl
	ghdl -i --work=work ahir_system.vhdl
	ghdl -i --work=work ahir_system_test_bench.vhdl
	ghdl -m --work=work -Wl,-L$(AHIR_LIB) -Wl,-lVhpi ahir_system_test_bench 

clean:
	rm -rf *.o* *.cf *.*vhdl vhdlCStubs.* *.vcd in_data* out_data* testbench_sw testbench_hw ahir_system_test_bench vhpi.log *.aa *.vc *.lso xst *.ngc *_xmsgs *.xrpt pipeHandler.log *.srp *.ghw *.dot


PHONY: all  clean
