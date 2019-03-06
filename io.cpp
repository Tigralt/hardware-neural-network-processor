#include "systemc.h"

SC_MODULE(io_module)
{
    // Ports
    sc_fifo_in<float> dma;
    sc_fifo_in<int> input_length;
    sc_fifo_in<float> output;
    sc_fifo_out<float> dma_output;
    sc_fifo_out<float> input;
    sc_in<bool> scheduler_ready;
    sc_out<bool> valid;

    // States
    int _length, _counter;
    bool do_return = false;
    sc_event do_drain, do_output;

    // Processes
    void load_length(void);
    void drain(void);
    void to_dma(void);

    SC_CTOR(io_module) :
        dma("dma_0"),
        input_length("input_length_0"),
        output("output_0"),
        input("input_0"),
        valid("valid_0")
    {
        SC_METHOD(load_length);
        sensitive << scheduler_ready.pos();
        dont_initialize();

        SC_METHOD(drain);
        sensitive << do_drain;
        dont_initialize();

        SC_METHOD(to_dma);
        sensitive << do_output;
        dont_initialize();
    }
};

void io_module::load_length(void)
{
    if (input_length.num_available() == 0)
        return next_trigger(input_length.data_written_event());

    input_length.read(_length);
    do_return = _length == 0;

    if (do_return) {
        do_output.notify();
    } else {
        _counter = 0;
        do_drain.notify();

        #if DEBUG == 1
            cout << "[io_module] @" << sc_time_stamp() << " loading length (" << _length << ")" << endl;
        #endif
    }

    next_trigger(scheduler_ready.posedge_event());
}

void io_module::drain(void)
{
    float value;

    if (!dma.nb_read(value))
        if (!output.nb_read(value)) 
            return do_drain.notify(SC_ZERO_TIME);
    input.write(value);

    _counter++;
    if (_counter < _length) {
        do_drain.notify(SC_ZERO_TIME);
        valid.write(false);
    }
    else {
        valid.write(true);
    }

    #if DEBUG == 1
        cout << "[io_module] @" << sc_time_stamp() << " writing value " << value << " to vector (" << _counter << "/" << _length << ")" << endl;
    #endif
}

void io_module::to_dma(void)
{
    float value;

    if (do_return)
        next_trigger(do_output | output.data_written_event());
    else
        return next_trigger(do_output);

    if (output.nb_read(value)) {
        dma_output.write(value);
        do_output.notify(SC_ZERO_TIME);
    }

    #if DEBUG == 1
        cout << "[io_module] @" << sc_time_stamp() << " return [" << value << "] to DMA" << endl;
    #endif
}