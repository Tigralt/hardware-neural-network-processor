#include "../headers/weight.hpp"

void weight_module::process(void)
{
#pragma HLS resource core = AXI4Stream variable = from_dma

    while (true)
    {
        // Wait for config
        from_config.read(state_length);

        // Init
        float value;

        // Process
        for (unsigned int i = 0; i < state_length; i++)
        {
            from_dma.read(value);
            to_scheduler.write(value);

#ifndef __SYNTHESIS__
            cout << "[weight_module] @" << sc_time_stamp() << " writing value " << value << " to vector" << endl;
#endif
        }
    }
}