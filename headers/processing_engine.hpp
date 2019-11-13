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
    sc_fifo_in<unsigned int> from_scheduler_instructions;
    sc_fifo_out<float> to_scheduler;

    // STATES
    unsigned int state_length, state_activation_function;

    // PROCESS
    void process(void);

    // UTIL
    float sigmoid(float input);
    float relu(float input);
    float softmax(float input);

    SC_CTOR(processing_engine_module)
    {
        state_length = 0;
        state_activation_function = 0;

        SC_CTHREAD(process, clk.pos());
        reset_signal_is(reset,true);
    }
};

#endif