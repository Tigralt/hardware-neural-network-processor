#ifndef TOP_HPP
#define TOP_HPP

#include <systemc.h>
#include "config.hpp"
#include "weight.hpp"
#include "io.hpp"
#include "scheduler.hpp"
#include "processing_engine.hpp"

#define CORE_BIND(NAME, INDEX)                     \
    NAME.clk(clk);                                 \
    NAME.reset(reset);                             \
    NAME.from_scheduler_length(npu_length[INDEX]); \
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
    sc_fifo<unsigned int> fifo_weight_length;
    sc_fifo<unsigned int> fifo_input_length;
    sc_fifo<unsigned int> fifo_current_length;
    sc_fifo<unsigned int> fifo_next_length;
    sc_fifo<float> fifo_weight;
    sc_fifo<float> fifo_input;
    sc_fifo<float> fifo_output;

    sc_fifo<unsigned int> npu_length[CORE];
    sc_fifo<float> npu_weight[CORE];
    sc_fifo<float> npu_input[CORE];
    sc_fifo<float> npu_output[CORE];

    // Modules
    config_module mod_config;
    weight_module mod_weight;
    io_module mod_io;
    scheduler_module mod_scheduler;

// Cores
#if CORE == 4
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