#include "../headers/top.hpp"

top_module::top_module(sc_module_name name) : clk("clock"),
                                              mod_config("config"),
                                              mod_weight("weight"),
                                              mod_io("io"),
                                              mod_scheduler("scheduler")
{
    mod_config.clk(clk);
    mod_config.reset(reset);
    mod_config.from_dma(dma_config);
    mod_config.to_io(fifo_input_length);
    mod_config.to_weight(fifo_weight_length);
    mod_config.to_scheduler(fifo_next_length);

    mod_weight.clk(clk);
    mod_weight.reset(reset);
    mod_weight.from_dma(dma_weight);
    mod_weight.from_config(fifo_weight_length);
    mod_weight.to_scheduler(fifo_weight);

    mod_io.clk(clk);
    mod_io.reset(reset);
    mod_io.from_dma(dma_input);
    mod_io.to_dma(dma_output);
    mod_io.from_config(fifo_input_length);
    mod_io.to_scheduler(fifo_input);
    mod_io.from_scheduler(fifo_output);
    mod_io.to_scheduler_valid(valid);
    mod_io.from_scheduler_ready(ready);

    mod_scheduler.clk(clk);
    mod_scheduler.reset(reset);
    mod_scheduler.from_weight(fifo_weight);
    mod_scheduler.from_io(fifo_input);
    mod_scheduler.to_io(fifo_output);
    mod_scheduler.from_config(fifo_next_length);
    mod_scheduler.from_io_valid(valid);
    mod_scheduler.to_io_ready(ready);
}