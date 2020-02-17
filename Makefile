CC=g++
CFLAGS=-I. -I$(SYSTEMC_DEBUG)/include -L. -L$(SYSTEMC_DEBUG)/lib-linux64 -Wl,-rpath=$(SYSTEMC_DEBUG)/lib-linux64

all:
	$(CC) $(CFLAGS) testbench/main.cpp sources/*.cpp -o build/npu -lsystemc -lm -lcnpy -lz