#ifndef IO_HPP
#define IO_HPP

#include <systemc.h>

#define LOOP_VECTOR 2048

SC_MODULE(io_module)
{
    // PORTS
    sc_in<bool> clk;
    sc_in<bool> reset;
    sc_fifo_in<float> inputs;
    sc_fifo_in<unsigned int> from_config;
    sc_fifo_out<float> to_scheduler;

    // STATES
    unsigned int state_length;
    float state_loop_vector[LOOP_VECTOR];

    // PROCESS
    void process(void);

    SC_CTOR(io_module) :
        clk("clock"),
        inputs("inputs"),
        from_config("from_config"),
        to_scheduler("to_scheduler")
    {
        // Init STATES
        state_length = 0;

        SC_CTHREAD(process, clk.pos());
        reset_signal_is(reset,true);
    }
};

#endif