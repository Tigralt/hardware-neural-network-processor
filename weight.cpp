#include "systemc.h"

SC_MODULE(weight_module)
{
    // Ports
    sc_fifo_in<float> dma;
    sc_fifo_in<int> weight_length;
    sc_fifo_out<float> weight;

    // States
    int _length, _counter;
    sc_event do_drain, do_load;

    // Processes
    void load_length(void);
    void drain_dma(void);

    SC_CTOR(weight_module) :
        dma("dma_0"),
        weight_length("weight_length_0"),
        weight("weight_0")
    {
        SC_METHOD(load_length);
        sensitive << weight_length.data_written();
        dont_initialize();

        SC_METHOD(drain_dma);
        sensitive << do_drain;
        dont_initialize();
    }
};

void weight_module::load_length(void)
{
    if (weight_length.num_available() == 0) {
        next_trigger(weight_length.data_written_event());
        return;
    }

    #if DEBUG == 1
        cout << "[weight_module] @" << sc_time_stamp() << " loading length " << endl;
    #endif

    weight_length.read(_length);
    _counter = 0;
    do_drain.notify();

    next_trigger(do_load);
}

void weight_module::drain_dma(void)
{
    float value;

    if (!dma.nb_read(value))
        return do_drain.notify(SC_ZERO_TIME);

    if (weight.nb_write(value))
        next_trigger(do_drain);
    else {
        next_trigger(weight.data_read_event());
        return;
    }

    _counter++;
    if (_counter < _length)
        do_drain.notify(SC_ZERO_TIME);
    else
        do_load.notify();

    #if DEBUG == 1
        cout << "[weight_module] @" << sc_time_stamp() << " writing value " << value << " to vector" << endl;
    #endif
}