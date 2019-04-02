#ifndef TOP_HPP
#define TOP_HPP

#include <systemc.h>
#include "config.hpp"
#include "weight.hpp"
#include "io.hpp"
#include "scheduler.hpp"

class top_module : public sc_core::sc_module
{
public:
    sc_in<bool> clk;

    // IO
    sc_fifo_in<unsigned int> dma_config;
    sc_fifo_in<float> dma_weight;
    sc_fifo_in<float> dma_input;
    sc_fifo_out<float> dma_output;

    // Internal
    sc_fifo<unsigned int> fifo_weight_length;
    sc_fifo<unsigned int> fifo_input_length;
    sc_fifo<unsigned int> fifo_next_length;
    sc_fifo<float> fifo_weight;
    sc_fifo<float> fifo_input;
    sc_fifo<float> fifo_output;
    sc_signal<bool> valid;
    sc_signal<bool> ready;

    // Modules
    config_module mod_config;
    weight_module mod_weight;
    io_module mod_io;
    scheduler_module mod_scheduler;

    top_module(sc_module_name name);
    SC_HAS_PROCESS(top_module);
};

#endif