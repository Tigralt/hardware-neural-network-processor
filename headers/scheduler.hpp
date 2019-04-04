#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include <systemc.h>

#define CORE 4
#define INPUT_VECTOR 2048

SC_MODULE(scheduler_module)
{
    // PORTS
    sc_in<bool> clk;
    sc_in<bool> reset;
    sc_fifo_in<float> from_weight;
    sc_fifo_in<float> from_io;
    sc_fifo_in<unsigned int> from_config;
    sc_fifo_out<float> to_io;
    sc_in<bool> from_io_valid;
    sc_out<bool> to_io_ready;

    // PROCESSING ENGINES
    sc_fifo_out<unsigned int> npu_length[CORE];
    sc_fifo_out<float> npu_weight[CORE];
    sc_fifo_out<float> npu_input[CORE];
    sc_fifo_in<float> npu_output[CORE];

    // STATES
    unsigned int state_input_length, state_next_length, state_input_vector_size;
    float state_input_vector[INPUT_VECTOR];

    // PROCESS
    void process(void);

    SC_CTOR(scheduler_module)
    {
        state_input_length = 0;
        state_next_length = 0;
        state_input_vector_size = 0;

        SC_CTHREAD(process, clk.pos());
        reset_signal_is(reset, true);
    }
};

#endif