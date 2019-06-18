#include "../headers/config.hpp"

void config_module::process_drain_dma(void)
{
#pragma HLS resource core = AXI4Stream variable = from_dma
#pragma HLS resource core = AXI4Stream variable = to_weight
#pragma HLS resource core = AXI4Stream variable = to_io
#pragma HLS resource core = AXI4Stream variable = to_scheduler_current
#pragma HLS resource core = AXI4Stream variable = to_scheduler_next

    while (true)
    {
        // Init
        unsigned int old_layer, current_layer = 1;
        from_dma.read(old_layer);

        // Main process
        while (current_layer > 0)
        {
            from_dma.read(current_layer);

#ifndef __SYNTHESIS__
            cout << "[config_module] @" << sc_time_stamp() << " loading network configuration" << endl;
#endif

            if (current_layer > 0)
            { // Dispach data
                to_io.write(old_layer);
                to_weight.write(current_layer * old_layer);
                to_scheduler_current.write(old_layer);
                to_scheduler_next.write(current_layer);
                old_layer = current_layer;
            }
            else
            { // END
                to_io.write(current_layer);
            }
        }
    }
}