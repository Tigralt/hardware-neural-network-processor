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

    top_module(sc_module_name name) :
        clk("clock"),
        mod_config("config"),
        mod_weight("weight"),
        mod_io("io"),
        mod_scheduler("scheduler")
    {
        mod_config.clk(clk);
        mod_config.from_dma(dma_config);
        mod_config.to_io(fifo_input_length);
        mod_config.to_weight(fifo_weight_length);
        mod_config.to_scheduler(fifo_next_length);

        mod_weight.clk(clk);
        mod_weight.from_dma(dma_weight);
        mod_weight.from_config(fifo_weight_length);
        mod_weight.to_scheduler(fifo_weight);

        mod_io.clk(clk);
        mod_io.from_dma(dma_input);
        mod_io.to_dma(dma_output);
        mod_io.from_config(fifo_input_length);
        mod_io.to_scheduler(fifo_input);
        mod_io.from_scheduler(fifo_output);
        mod_io.to_scheduler_valid(valid);
        mod_io.from_scheduler_ready(ready);

        mod_scheduler.clk(clk);
        mod_scheduler.from_weight(fifo_weight);
        mod_scheduler.from_io(fifo_input);
        mod_scheduler.to_io(fifo_output);
        mod_scheduler.from_config(fifo_next_length);
        mod_scheduler.from_io_valid(valid);
        mod_scheduler.to_io_ready(ready);
    }
    SC_HAS_PROCESS(top_module);
};

#endif