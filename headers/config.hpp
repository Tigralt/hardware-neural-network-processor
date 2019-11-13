#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <systemc.h>

SC_MODULE(config_module)
{
    // PORTS
    sc_in<bool> clk;
    sc_in<bool> reset;
    sc_fifo_in<unsigned int> from_dma;
    sc_fifo_out<unsigned int> to_scheduler_instructions;

    // PROCESS
    void process_drain_dma(void);

    SC_CTOR(config_module) :
        clk("clk"),
        from_dma("from_dma"),
        to_scheduler_instructions("to_scheduler_instructions")
    {
        SC_CTHREAD(process_drain_dma, clk.pos());
        reset_signal_is(reset,true);
    }
};

#endif