#include "../headers/io.hpp"

void io_module::process(void)
{
#pragma HLS resource core = AXI4Stream variable = inputs
#pragma HLS resource core = AXI4Stream variable = to_dma
#pragma HLS resource core = AXI4Stream variable = from_config
#pragma HLS resource core = AXI4Stream variable = to_scheduler

    while (true)
    {
        // Process
        state_to_return = state_length;
        from_config.read(state_length);
        do_return = (state_length == 0);

        // Return values to DMA
        if (do_return)
        {
            float value;

            for (unsigned int i = 0; i < state_to_return; i++)
            {
                inputs.read(value);
                to_dma.write(value);

#ifndef __SYNTHESIS__
                cout << "[io_module] @" << sc_time_stamp() << " return [" << value << "] to DMA" << endl;
#endif
            }

            // Reset
            do_return = false;
            state_to_return = 0;
        } else {
#ifndef __SYNTHESIS__
            cout << "[io_module] @" << sc_time_stamp() << " loading length (" << state_length << ")" << endl;
#endif      

            // Init
            float value;

            // Process
            for (unsigned int i = 0; i < state_length; i++)
            {
                inputs.read(value);
                to_scheduler.write(value);

#ifndef __SYNTHESIS__
                cout << "[io_module] @" << sc_time_stamp() << " writing value " << value << " to vector (" << i + 1 << "/" << state_length << ")" << endl;
#endif
            }
        }
    }
}