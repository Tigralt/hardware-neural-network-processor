#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <systemc.h>

SC_MODULE(config_module)
{
    // PORTS
    sc_in<bool> clk;
    sc_fifo_in<unsigned int> from_dma;
    sc_fifo_out<unsigned int> to_weight;
    sc_fifo_out<unsigned int> to_io;
    sc_fifo_out<unsigned int> to_scheduler;

    // PROCESS
    void process_drain_dma(void);

    SC_CTOR(config_module) :
        clk("clk"),
        from_dma("from_dma"),
        to_weight("to_weight"),
        to_io("to_io"),
        to_scheduler("to_scheduler")
    {
        SC_CTHREAD(process_drain_dma, clk.pos());
    }
};

#endif