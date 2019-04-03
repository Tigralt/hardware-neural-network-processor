#include "../headers/scheduler.hpp"

void scheduler_module::process(void)
{
    to_io_ready.write(true);

    while (true)
    {
        // Wait for events
        while (!(from_io_valid.read()))
            wait();
        to_io_ready.write(false);

        // Init
        float input, weight;
        state_input_vector_size = 0;

        // Process
        while (from_io.nb_read(input))
        {
            state_input_vector[state_input_vector_size++] = input;
        }

#ifndef __SYNTHESIS__
        cout << "[scheduler_module] @" << sc_time_stamp() << " inputs loaded (" << state_input_vector_size << ")" << endl;
#endif

        for (unsigned int i = 0; i < CORE; i++)
            npu_length[i].write(state_input_vector_size);

        from_config.read(state_next_length);

#ifndef __SYNTHESIS__
        cout << "[scheduler_module] @" << sc_time_stamp() << " next loaded (" << state_next_length << ")" << endl;
#endif

        // Init
        unsigned int input_counter = -1;
        unsigned int loop_max = state_next_length * state_input_vector_size;

        // Process
        for (unsigned int i = 0, index; i < loop_max; i++)
        {
            index = (i % state_next_length) % CORE;
            if (index == 0)
                input_counter++;

            npu_weight[index].write(from_weight.read());
            npu_input[index].write(state_input_vector[input_counter]);
        }

        to_io_ready.write(true);

        // Return values
        float value;
        for (unsigned int i = 0; i < state_next_length; i++)
        {
            npu_output[i].read(value);
            to_io.write(value);
#ifndef __SYNTHESIS__
            cout << "[scheduler_module] @" << sc_time_stamp() << " returning result (" << value << ")" << endl;
#endif
        }
    }
}