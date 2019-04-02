#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include <systemc.h>
#include "processing_engine.hpp"

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
    sc_fifo< sc_uint<16> > npu_length[CORE];
    sc_fifo<float> npu_weight[CORE];
    sc_fifo<float> npu_input[CORE];
    sc_fifo<float> npu_output[CORE];
    processing_engine_module npu_manager[CORE];

    // STATES
    unsigned int state_input_length, state_next_length;
    sc_uint<16> state_input_vector_size;
    float state_input_vector[INPUT_VECTOR];

    // EVENTS
    bool do_return;

    // PROCESS
    void process(void);
    void process_return(void);

    SC_CTOR(scheduler_module)
    {
        state_input_length = 0;
        state_next_length = 0;
        state_input_vector_size = 0;

        do_return = false;

        for (unsigned int i = 0; i < CORE; i++)
        {
            npu_manager[i].clk(clk);
            npu_manager[i].reset(reset);
            npu_manager[i].from_scheduler_length(npu_length[i]);
            npu_manager[i].from_scheduler_weight(npu_weight[i]);
            npu_manager[i].from_scheduler_input(npu_input[i]);
            npu_manager[i].to_scheduler(npu_output[i]);
        }

        SC_CTHREAD(process, clk.pos());
        SC_CTHREAD(process_return, clk.pos());
    }
};

#endif