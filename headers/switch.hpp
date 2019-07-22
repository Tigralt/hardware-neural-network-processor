#ifndef SWITCH_HPP
#define SWITCH_HPP

#include <systemc.h>

SC_MODULE(switch_module)
{
    // PORTS
    sc_in<bool> clk;
    sc_in<bool> reset;
    sc_fifo_in<float> in1;
    sc_fifo_in<float> in2;
    sc_fifo_out<float> out;
    
    void process(void);

    SC_CTOR(switch_module) :
        clk("clk"),
        in1("in1"),
        in2("in2"),
        out("out")
    {
        SC_CTHREAD(process, clk.pos());
        reset_signal_is(reset,true);
    }
};

#endif