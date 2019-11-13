#ifndef TOP_HPP
#define TOP_HPP

#include <systemc.h>
#include "config.hpp"
#include "scheduler.hpp"
#include "processing_engine.hpp"



#define CORE_BIND(NAME, INDEX)                     \
    NAME.clk(clk);                                 \
    NAME.reset(reset);                             \
    NAME.from_scheduler_instructions(npu_instructions[INDEX]); \
    NAME.from_scheduler_weight(npu_weight[INDEX]); \
    NAME.from_scheduler_input(npu_input[INDEX]);   \
    NAME.to_scheduler(npu_output[INDEX]);

class top_module : public sc_core::sc_module
{
public:
    sc_in<bool> clk;
    sc_in<bool> reset;

    // IO
    sc_fifo_in<unsigned int> dma_config;
    sc_fifo_in<float> dma_weight;
    sc_fifo_in<float> dma_input;
    sc_fifo_out<float> dma_output;

    // Internal
    sc_fifo<unsigned int> fifo_instructions;

    sc_fifo<sc_uint<17>> npu_instructions[CORE];
    sc_fifo<float> npu_weight[CORE];
    sc_fifo<float> npu_input[CORE];
    sc_fifo<float> npu_output[CORE];

    // Modules
    config_module mod_config;
    scheduler_module mod_scheduler;

// Cores
#if CORE == 8
    processing_engine_module mod_core_1;
    processing_engine_module mod_core_2;
    processing_engine_module mod_core_3;
    processing_engine_module mod_core_4;
    processing_engine_module mod_core_5;
    processing_engine_module mod_core_6;
    processing_engine_module mod_core_7;
    processing_engine_module mod_core_8;
#elif CORE == 4
    processing_engine_module mod_core_1;
    processing_engine_module mod_core_2;
    processing_engine_module mod_core_3;
    processing_engine_module mod_core_4;
#elif CORE == 2
    processing_engine_module mod_core_1;
    processing_engine_module mod_core_2;
#elif CORE == 1
    processing_engine_module mod_core_1;
#endif

    top_module(sc_module_name name);
    SC_HAS_PROCESS(top_module);
};

#endif