#include "../headers/io.hpp"

void io_module::process(void)
{
#pragma HLS resource core = AXI4Stream variable = from_dma
#pragma HLS resource core = AXI4Stream variable = to_dma

    to_scheduler_valid.write(false);

    while (true)
    {
        // Wait for event
        while (!from_scheduler_ready.read())
            wait();
        to_scheduler_valid.write(false);

        // Process
        state_to_return = state_length;
        from_config.read(state_length);
        do_return = (state_length == 0);

#ifndef __SYNTHESIS__
        cout << "[io_module] @" << sc_time_stamp() << " loading length (" << state_length << ")" << endl;
#endif      

        // Init
        float value;

        // Process
        if (do_load_from_dma)
        {
            // DMA to Scheduler
            for (unsigned int i = 0; i < state_length; i++)
            {
                from_dma.read(value);
                to_scheduler.write(value);

#ifndef __SYNTHESIS__
                cout << "[io_module] @" << sc_time_stamp() << " writing value " << value << " to vector (" << i + 1 << "/" << state_length << ")" << endl;
#endif
            }
            do_load_from_dma = false;
        }
        else
        {
            // Layers loop
            for (unsigned int i = 0; !do_return && i < state_length; i++)
            {
                from_scheduler.read(value);
                to_scheduler.write(value);

#ifndef __SYNTHESIS__
                cout << "[io_module] @" << sc_time_stamp() << " writing value " << value << " to vector (" << i + 1 << "/" << state_length << ")" << endl;
#endif
            }
        }

        to_scheduler_valid.write(true);
        wait();

        // Return values to DMA
        if (do_return)
        {
            float value;

            for (unsigned int i = 0; i < state_to_return; i++)
            {
                from_scheduler.read(value);
                to_dma.write(value);

#ifndef __SYNTHESIS__
                cout << "[io_module] @" << sc_time_stamp() << " return [" << value << "] to DMA" << endl;
#endif
            }

            do_return = false;
        }
    }
}