#ifndef IO_HPP
#define IO_HPP

#include <systemc.h>

SC_MODULE(io_module)
{
    // PORTS
    sc_in<bool> clk;
    sc_in<bool> reset;
    sc_fifo_in<float> from_dma;
    sc_fifo_out<float> to_dma;
    sc_fifo_in<unsigned int> from_config;
    sc_fifo_in<float> from_scheduler;
    sc_fifo_out<float> to_scheduler;

    // STATES
    unsigned int state_length, state_to_return;

    // EVENTS
    bool do_load_from_dma, do_return;

    // PROCESS
    void process(void);

    SC_CTOR(io_module) :
        clk("clock"),
        from_dma("from_dma"),
        to_dma("to_dma"),
        from_config("from_config"),
        from_scheduler("from_scheduler"),
        to_scheduler("to_scheduler")
    {
        // Init STATES
        state_length = 0;
        state_to_return = 0;

        // Init EVENTS
        do_load_from_dma = true;
        do_return = false;

        SC_CTHREAD(process, clk.pos());
        reset_signal_is(reset,true);
    }
};

#endif