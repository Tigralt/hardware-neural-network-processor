#include "systemc.h"
#define CORE 4

#include "config.cpp"
#include "weight.cpp"
#include "io.cpp"
#include "scheduler.cpp"

SC_MODULE(top_module)
{
    // IO
    sc_fifo_in<int> dma_config;
    sc_fifo_in<float> dma_weight;
    sc_fifo_in<float> dma_input;
    sc_fifo_out<float> dma_output;

    // Internal
    sc_fifo<int> fifo_weight_length;
    sc_fifo<int> fifo_input_length;
    sc_fifo<int> fifo_next_length;
    sc_fifo<float> fifo_weight;
    sc_fifo<float> fifo_input;
    sc_fifo<float> fifo_output;
    sc_signal<bool> fifo_input_valid;
    sc_signal<bool> scheduler_ready;

    // Modules
    config_module mod_config;
    weight_module mod_weight;
    io_module mod_io;
    scheduler_module mod_scheduler;

    // Initialization
    void init(void);

    SC_CTOR(top_module) :
        mod_config("config_0"),
        mod_weight("weight_0"),
        mod_io("io_0"),
        mod_scheduler("scheduler_0")
    {
        mod_config.dma(dma_config);
        mod_config.input(fifo_input_length);
        mod_config.weight(fifo_weight_length);
        mod_config.next(fifo_next_length);

        mod_weight.dma(dma_weight);
        mod_weight.weight_length(fifo_weight_length);
        mod_weight.weight(fifo_weight);

        mod_io.dma(dma_input);
        mod_io.dma_output(dma_output);
        mod_io.input_length(fifo_input_length);
        mod_io.input(fifo_input);
        mod_io.output(fifo_output);
        mod_io.valid(fifo_input_valid);
        mod_io.scheduler_ready(scheduler_ready);

        mod_scheduler.weight(fifo_weight);
        mod_scheduler.input(fifo_input);
        mod_scheduler.output(fifo_output);
        mod_scheduler.next(fifo_next_length);
        mod_scheduler.io_valid(fifo_input_valid);
        mod_scheduler.ready(scheduler_ready);

        SC_METHOD(init);
    }
};

void top_module::init(void) {
    
}
