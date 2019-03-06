#include "systemc.h"

SC_MODULE(processing_engine_module)
{
    // Ports
    sc_fifo_in<float> weight;
    sc_fifo_in<float> input;
    sc_fifo_in<sc_uint<16>> length;
    sc_fifo_out<float> output;

    // States
    float _output, _counter;
    sc_uint<16> _length;

    // Processes
    void store_length(void);
    void calculation(void);

    SC_CTOR(processing_engine_module) :
        weight("weight_0"),
        input("input_0"),
        length("length_0"),
        output("output_0")
    {
        SC_METHOD(store_length);
        sensitive << length.data_written();
        dont_initialize();

        SC_METHOD(calculation);
    }
};

void processing_engine_module::store_length(void)
{
    _counter = 0;
    _output = 0;
    length.read(_length);
}

void processing_engine_module::calculation(void)
{
    next_trigger(weight.data_written_event() & input.data_written_event());

    if (weight.num_available() == 0 || input.num_available() == 0) {
        return;
    }

    float w, i;

    weight.read(w);
    input.read(i);
    _output += w * i;
    _counter++;
    
    #if DEBUG == 1
        cout << "[processing_engine_module] @" << sc_time_stamp() << " [" << w << ";" << i << "] (" << _counter << "/" << _length << ")" << endl;
    #endif

    if (_counter == _length) {
        _counter = 0;
        _output = _output / (1 + abs(_output));
        output.write(_output);
        _output = 0;

        #if DEBUG == 1
            cout << "[processing_engine_module] @" << sc_time_stamp() << " end of calculation [" << _output << "]" << endl;
        #endif
    }
}
