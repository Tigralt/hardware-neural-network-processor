#ifndef WEIGHT_HPP
#define WEIGHT_HPP

#include <systemc.h>

SC_MODULE(weight_module)
{
    // PORTS
    sc_in<bool> clk;
    sc_fifo_in<float> from_dma;
    sc_fifo_in<unsigned int> from_config;
    sc_fifo_out<float> to_scheduler;

    // STATES
    unsigned int state_length;

    // PROCESS
    void process(void);

    SC_CTOR(weight_module)
    {
        state_length = 0;

        SC_CTHREAD(process, clk.pos());
    }
};

#endif