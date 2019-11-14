#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include <systemc.h>

#define CORE 4
#define INPUT_VECTOR 32768

SC_MODULE(scheduler_module)
{
    // PORTS
    sc_in<bool> clk;
    sc_in<bool> reset;
    sc_fifo_in<float> from_dma_weight;
    sc_fifo_in<float> from_dma_input;
    sc_fifo_in<unsigned int> from_dma_instructions;
    sc_fifo_out<float> to_dma;

    // PROCESSING ENGINES
    sc_fifo_out<sc_uint<17>> npu_instructions[CORE];
    sc_fifo_out<float> npu_weight[CORE];
    sc_fifo_out<float> npu_input[CORE];
    sc_fifo_in<float> npu_output[CORE];

    // STATES
    sc_uint<15> state_current_length, state_next_length;
    sc_uint<2> state_activation_function;
    float state_current_vector[INPUT_VECTOR];

    // PROCESS
    void process(void);

    SC_CTOR(scheduler_module)
    {
        // Init STATES
        state_current_length = 0;
        state_next_length = 0;
        state_activation_function = 0;

        SC_CTHREAD(process, clk.pos());
        reset_signal_is(reset, true);
    }
};

#endif