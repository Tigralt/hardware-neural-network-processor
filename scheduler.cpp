#include "systemc.h"
#include "processing_engine.cpp"

SC_MODULE(scheduler_module)
{
    // Ports
    sc_fifo_in<float> weight;
    sc_fifo_in<float> input;
    sc_fifo_in<int> next;
    sc_fifo_out<float> output;
    sc_in<bool> io_valid;
    sc_out<bool> ready;

    // States
    sc_vector<sc_fifo<sc_uint<16>>> npu_length;
    sc_vector<sc_fifo<float>> npu_input;
    sc_vector<sc_fifo<float>> npu_weight;
    sc_vector<sc_fifo<float>> npu_output;
    sc_vector<processing_engine_module> npu_manager;

    float _vector[2048];
    unsigned int _input_size, _input_counter, _next_size, _next_counter;
    sc_event do_schedule, do_load, trigger_ready;

    // Processes
    void flip_ready(void);
    void load_input(void);
    void load_weight(void);
    void schedule(void);
    void ascend_output(void);

    SC_CTOR(scheduler_module) :
        weight("weight_0"),
        input("input_0"),
        next("next_0"),
        output("output_0"),
        io_valid("io_valid_0"),
        ready("ready_0"),
        npu_manager("npu_module", CORE),
        npu_input("npu_input", CORE),
        npu_weight("npu_weight", CORE),
        npu_length("npu_length", CORE),
        npu_output("npu_output", CORE)
    {
        for (int i=0; i<CORE; i++) {
            npu_manager[i].weight(npu_weight[i]);
            npu_manager[i].length(npu_length[i]);
            npu_manager[i].output(npu_output[i]);
            npu_manager[i].input(npu_input[i]);
        }

        SC_METHOD(flip_ready);
        sensitive << trigger_ready;

        SC_METHOD(load_input);
        sensitive << io_valid.pos();
        dont_initialize();

        SC_METHOD(load_weight);
        sensitive << do_load;
        dont_initialize();

        SC_METHOD(schedule);
        sensitive << do_schedule;
        dont_initialize();

        SC_METHOD(ascend_output);
        for (int i=0; i<CORE; i++)
            sensitive << npu_output[i].data_written_event();
        dont_initialize();
    }
};

void scheduler_module::flip_ready(void)
{
    #if DEBUG == 1
        if (!ready)
            cout << "[scheduler_module] @" << sc_time_stamp() << " waiting for inputs" << endl;
    #endif
    ready.write(!ready);
}

void scheduler_module::load_input(void)
{
    float i;

    trigger_ready.notify();
    _input_size = _input_counter = 0;
    while (input.nb_read(i)) {
        _vector[_input_size] = i;
        _input_size++;
    }

    #if DEBUG == 1
        cout << "[scheduler_module] @" << sc_time_stamp() << " inputs loaded (" << _input_size << ")" << endl;
    #endif

    for (int i=0; i<CORE; i++)
        npu_length[i].write(_input_size);

    do_load.notify();
}

void scheduler_module::load_weight(void)
{
    int i;
    if (next.nb_read(i)) {
        _next_counter = 0;
        _next_size = i;

        #if DEBUG == 1
            cout << "[scheduler_module] @" << sc_time_stamp() << " next loaded (" << _next_size << ")" << endl;
        #endif

        do_schedule.notify();
        next_trigger(do_load);
    } else
        next_trigger(next.data_written_event());
}

void scheduler_module::schedule(void)
{
    #if DEBUG == 1
        cout << "[scheduler_module] @" << sc_time_stamp() << " schedule (" << weight.num_available() << ")" << endl;
    #endif

    float w;
  
    if (weight.num_available() >= CORE || weight.num_available() >= _next_size)
        next_trigger(do_schedule);
    else {
        next_trigger(weight.data_written_event());
        return;
    }

    #if DEBUG == 1
        cout << "[scheduler_module] @" << sc_time_stamp() << " ready (" << _input_counter << "/" << _input_size << " - " << _next_counter << "/" << _next_size << ")" << endl; 
    #endif

    for (int i=0; i<CORE; i++) {
        if (_next_counter + i >= _next_size)
            break;

        weight.read(w);
        npu_weight[i].write(w);
        npu_input[i].write(_vector[_input_counter]);
    }
    _input_counter++;

    if (_input_counter < _input_size) {
        do_schedule.notify(SC_ZERO_TIME);
    } else {
        _next_counter += CORE;
        _input_counter = 0;
        if (_next_counter < _next_size)
            do_schedule.notify(SC_ZERO_TIME);
        else
            trigger_ready.notify();
    }
}

void scheduler_module::ascend_output(void)
{
    float value;
    for (int i=0; i<CORE; i++)
        if (npu_output[i].nb_read(value))
            output.write(value);

    #if DEBUG == 1
        cout << "[scheduler_module] @" << sc_time_stamp() << " returning results" << endl; 
    #endif
}