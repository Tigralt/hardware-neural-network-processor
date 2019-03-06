#include "systemc.h"

SC_MODULE(config_module)
{
    // Ports
    sc_fifo_in<int> dma;
    sc_fifo_out<int> weight;
    sc_fifo_out<int> input;
    sc_fifo_out<int> next;

    // States
    int _layer = -1;

    // Processes
    void drain_dma(void);

    SC_CTOR(config_module) :
        dma("dma_0"),
        weight("weight_0"),
        input("input_0"),
        next("next_0")
    {
        SC_METHOD(drain_dma);
        sensitive << dma.data_written();
        dont_initialize();
    }
};

void config_module::drain_dma(void)
{
    #if DEBUG == 1
        cout << "[config_module] @" << sc_time_stamp() << " loading network configuration" << endl;
    #endif

    if (_layer < 0) { // Init
        dma.read(_layer);
        return next_trigger(SC_ZERO_TIME);
    }

    int current_layer;
    dma.read(current_layer);

    if (current_layer != 0) {
        input.write(_layer);
        weight.write(current_layer * _layer);
        next.write(current_layer);
        _layer = current_layer;
    } else { // END
        input.write(current_layer);
    }

    if (dma.num_available() > 0)
        next_trigger(SC_ZERO_TIME);
    else
        next_trigger(dma.data_written_event());
}