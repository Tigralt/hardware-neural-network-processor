#ifndef PROCESSING_ENGINE_HPP
#define PROCESSING_ENGINE_HPP

#include <systemc.h>

SC_MODULE(processing_engine_module)
{
    // PORTS
    sc_in<bool> clk;
    sc_in<bool> reset;
    sc_fifo_in<float> from_scheduler_weight;
    sc_fifo_in<float> from_scheduler_input;
    sc_fifo_in< sc_uint<16> > from_scheduler_length;
    sc_fifo_out<float> to_scheduler;

    // STATES
    sc_uint<16> state_length;

    // PROCESS
    void process(void);

    // UTIL
    float activation_function(float input);

    SC_CTOR(processing_engine_module)
    {
        state_length = 0;

        SC_CTHREAD(process, clk.pos());
        reset_signal_is(reset,true);
    }
};

#endif