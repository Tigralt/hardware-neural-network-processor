#include "../headers/config.hpp"
#include <bitset>

void config_module::process_drain_dma(void)
{
#pragma HLS resource core = AXI4Stream variable = from_dma
#pragma HLS resource core = AXI4Stream variable = to_weight
#pragma HLS resource core = AXI4Stream variable = to_io
#pragma HLS resource core = AXI4Stream variable = to_scheduler_instructions

    while (true)
    {
        // Init
        unsigned int config, activation_function, old_layer, current_layer;
        from_dma.read(config);
        old_layer = config >> 2;

#ifndef __SYNTHESIS__
            cout << "[config_module] @" << sc_time_stamp() << " loading network configuration (" << old_layer << ")" << endl;
#endif

        // Main process
        do
        {
            from_dma.read(config);
            current_layer = config >> 2;
            activation_function = config & 0b11;

#ifndef __SYNTHESIS__
            cout << "[config_module] @" << sc_time_stamp() << " loading network configuration (" << current_layer << ")" << endl;
#endif
            to_scheduler_instructions.write((old_layer << 17) + (current_layer << 2) + activation_function);
            old_layer = current_layer;
            
        } while (current_layer > 0);
    }
}