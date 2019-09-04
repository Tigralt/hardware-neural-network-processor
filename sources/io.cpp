#include "../headers/io.hpp"

void io_module::process(void)
{
#pragma HLS resource core = AXI4Stream variable = inputs
#pragma HLS resource core = AXI4Stream variable = from_config
#pragma HLS resource core = AXI4Stream variable = to_scheduler

    while (true)
    {
        // Process
        from_config.read(state_length);

#ifndef __SYNTHESIS__
        cout << "[io_module] @" << sc_time_stamp() << " loading length (" << state_length << ")" << endl;
#endif
        for (unsigned int i = 0; i < state_length; i++)
        {
            state_loop_vector[i] = inputs.read();
        }

        // Process
        for (unsigned int i = 0; i < state_length; i++)
        {
            to_scheduler.write(state_loop_vector[i]);

#ifndef __SYNTHESIS__
            cout << "[io_module] @" << sc_time_stamp() << " writing value " << state_loop_vector[i] << " to vector (" << i + 1 << "/" << state_length << ")" << endl;
#endif
        }
    }
}