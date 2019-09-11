#include "../headers/scheduler.hpp"

void scheduler_module::process(void)
{
#pragma HLS resource core = AXI4Stream variable = from_weight
#pragma HLS resource core = AXI4Stream variable = from_io
#pragma HLS resource core = AXI4Stream variable = from_config_next
#pragma HLS resource core = AXI4Stream variable = from_config_current

#pragma HLS resource core = AXI4Stream variable = to_io
#pragma HLS resource core = AXI4Stream variable = to_dma

#pragma HLS array_partition variable = npu_length complete dim = 1
#pragma HLS array_partition variable = npu_weight complete dim = 1
#pragma HLS array_partition variable = npu_output complete dim = 1
#pragma HLS array_partition variable = npu_input complete dim = 1

#pragma HLS resource core = AXI4Stream variable = npu_length
#pragma HLS resource core = AXI4Stream variable = npu_weight
#pragma HLS resource core = AXI4Stream variable = npu_input
#pragma HLS resource core = AXI4Stream variable = npu_output

    while (true)
    {
        // Init
        from_config_current.read(state_current_vector_size);

        // Process
        for (unsigned int i = 0; i < state_current_vector_size; i++)
        {
            state_current_vector[i] = from_io.read();
        }

#ifndef __SYNTHESIS__
        cout << "[scheduler_module] @" << sc_time_stamp() << " inputs loaded (" << state_current_vector_size << ")" << endl;
#endif

        from_config_next.read(state_next_length);

#ifndef __SYNTHESIS__
        cout << "[scheduler_module] @" << sc_time_stamp() << " next loaded (" << state_next_length << ")" << endl;
#endif

        do_return = (state_next_length == 0);
        if (do_return) {
            for (unsigned int i = 0; i < state_current_vector_size; i++)
            {
                to_dma.write(state_current_vector[i]);
            }
        } else {
			// Schedule
			for (unsigned int core_done = 0; core_done < state_next_length; core_done += CORE)
			{
				// Load cores
				// If there is less nodes than cores, do not start unused cores
				for (unsigned int i = 0; i + core_done < state_next_length && i < CORE; i++)
				{
					npu_length[i].write(state_current_vector_size);
				}

				// Send data to cores
				for (unsigned int current_counter = 0; current_counter < state_current_vector_size; current_counter++)
				{
					#pragma HLS pipeline II=1 enable_flush
					for (unsigned int i = 0; i + core_done < state_next_length && i < CORE; i++)
					{
						npu_weight[i % CORE].write(from_weight.read());
						npu_input[i % CORE].write(state_current_vector[current_counter % state_current_vector_size]);
					}
				}

				// Return output
				#pragma HLS pipeline II=1 enable_flush
				for (unsigned int i = 0; i + core_done < state_next_length && i < CORE; i++) {
					to_io.write(npu_output[i % CORE].read());
				}
			}
        }
    }
}