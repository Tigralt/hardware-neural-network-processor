#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include <systemc.h>
#include "verbose.hpp"

#define CORE 4
#define INSTRUCTION_BUFFER 512
#define INPUT_BUFFER 32768

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
    sc_fifo_out< sc_uint<17> > npu_instructions[CORE];
    sc_fifo_out<float> npu_weight[CORE];
    sc_fifo_out<float> npu_input[CORE];
    sc_fifo_in<float> npu_output[CORE];

    // STATES
    unsigned int state_instruction_counter;
    sc_uint<32> state_instruction_buffer[INSTRUCTION_BUFFER];
    float state_input_buffer[INPUT_BUFFER];
    float state_output_buffer[INPUT_BUFFER];

    // PROCESS
    void process(void);

    SC_CTOR(scheduler_module)
    {
        // Init STATES
        state_instruction_counter = 0;

        SC_CTHREAD(process, clk.pos());
        reset_signal_is(reset, true);
    }
};

#endif