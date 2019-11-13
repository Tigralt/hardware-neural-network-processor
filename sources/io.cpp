#include "../headers/io.hpp"

void io_module::process(void)
{
#pragma HLS resource core = AXI4Stream variable = inputs
#pragma HLS resource core = AXI4Stream variable = from_config
#pragma HLS resource core = AXI4Stream variable = to_scheduler

    while (true)
    {
        // Wait for config
        unsigned int instructions;
        from_config.read(instructions);
        state_length = instructions >> 2;
        state_activation_function = instructions & 0b11;

#ifndef __SYNTHESIS__
        cout << "[io_module] @" << sc_time_stamp() << " loading length (" << state_length << ")" << endl;
#endif

        for (unsigned int i = 0; i < state_length; i++)
        {
            to_scheduler.write(inputs.read());

#ifndef __SYNTHESIS__
            cout << "[io_module] @" << sc_time_stamp() << " writing value  to vector (" << i + 1 << "/" << state_length << ")" << endl;
#endif
        }
    }
}