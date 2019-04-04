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

        from_config.read(state_next_length);

#ifndef __SYNTHESIS__
        cout << "[scheduler_module] @" << sc_time_stamp() << " next loaded (" << state_next_length << ")" << endl;
#endif

        // Load cores
        // If there is less nodes than cores, do not start unused cores
        for (unsigned int i = 0; i <= state_next_length && i < CORE; i++)
        {
            npu_length[i].write(state_input_vector_size);
        }

        // Schedule
        for (unsigned int core_done = 0; core_done < state_next_length; core_done += CORE)
        {
            for (unsigned int input_counter = 0; input_counter < state_input_vector_size; input_counter++)
            {
                for (unsigned int i = 0; core_done + i < state_next_length && i < CORE; i++)
                {
                    npu_weight[i % CORE].write(from_weight.read());
                    npu_input[i % CORE].write(state_input_vector[input_counter % state_input_vector_size]);
                }
            }
        }

        to_io_ready.write(true);

        // Return values
        float value;
        for (unsigned int i = 0; i < state_next_length; i++)
        {
            npu_output[i % CORE].read(value);
            to_io.write(value);
#ifndef __SYNTHESIS__
            cout << "[scheduler_module] @" << sc_time_stamp() << " returning result (" << value << ")" << endl;
#endif
        }
    }
}