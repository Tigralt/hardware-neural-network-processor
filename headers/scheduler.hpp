#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include <systemc.h>
#include "processing_engine.hpp"

#define CORE 4
#define INPUT_VECTOR 2048

#define CORE_BIND(NAME, INDEX)  NAME.clk(clk); \
                                NAME.reset(reset); \
                                NAME.from_scheduler_length(npu_length[INDEX]); \
                                NAME.from_scheduler_weight(npu_weight[INDEX]); \
                                NAME.from_scheduler_input(npu_input[INDEX]); \
                                NAME.to_scheduler(npu_output[INDEX]);

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
    processing_engine_module N1, N2, N3, N4;

    // STATES
    unsigned int state_input_length, state_next_length;
    sc_uint<16> state_input_vector_size;
    float state_input_vector[INPUT_VECTOR];

    // PROCESS
    void process(void);

    SC_CTOR(scheduler_module) :
        N1("processing_engine_module_0"),
        N2("processing_engine_module_1"),
        N3("processing_engine_module_2"),
        N4("processing_engine_module_3")
    {
        state_input_length = 0;
        state_next_length = 0;
        state_input_vector_size = 0;

        // for (unsigned int i = 0; i < CORE; i++)
        // {
        //     npu_manager[i].clk(clk);
        //     npu_manager[i].reset(reset);
        //     npu_manager[i].from_scheduler_length(npu_length[i]);
        //     npu_manager[i].from_scheduler_weight(npu_weight[i]);
        //     npu_manager[i].from_scheduler_input(npu_input[i]);
        //     npu_manager[i].to_scheduler(npu_output[i]);
        // }

        CORE_BIND(N1, 0);
        CORE_BIND(N2, 1);
        CORE_BIND(N3, 2);
        CORE_BIND(N4, 3);

        SC_CTHREAD(process, clk.pos());
        reset_signal_is(reset,true);
    }
};

#endif