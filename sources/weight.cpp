#include "../headers/weight.hpp"

void weight_module::process(void)
{
#pragma HLS resource core = AXI4Stream variable = from_dma
#pragma HLS resource core = AXI4Stream variable = from_config
#pragma HLS resource core = AXI4Stream variable = to_scheduler

    while (true)
    {
        // Wait for config
        from_config.read(state_length);


#ifndef __SYNTHESIS__
        cout << "[weight_module] @" << sc_time_stamp() << " loading length (" << state_length << ")" << endl;
#endif

        // Process
        #pragma HLS pipeline II=1 enable_flush
        for (unsigned int i = 0; i < state_length; i++)
        {
            to_scheduler.write(from_dma.read());

#ifndef __SYNTHESIS__
            // cout << "[weight_module] @" << sc_time_stamp() << " writing value to vector (" << i + 1 << "/" << state_length << ")" << endl;
#endif
        }
    }
}