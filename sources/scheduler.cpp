#include "../headers/scheduler.hpp"

void scheduler_module::process(void)
{
#pragma HLS resource core = AXI4Stream variable = from_weight
#pragma HLS resource core = AXI4Stream variable = from_io
#pragma HLS resource core = AXI4Stream variable = from_config_instructions

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
		unsigned int instructions;
        from_config_instructions.read(instructions);
		state_current_vector_size = (instructions & 0b11111111111111100000000000000000) >> 17;
		state_next_length = (instructions & 0b00000000000000011111111111111100) >> 2;
		state_activation_function = instructions & 0b11;

		// Read first inputs
        if (!state_get_external_inputs) {
			for (unsigned int i = 0; i < state_current_vector_size; i++)
			{
				state_current_vector[i] = from_dma_input.read();
			}
			state_get_external_inputs = true;
		}

#ifndef __SYNTHESIS__
        cout << "[scheduler_module] @" << sc_time_stamp() << " inputs loaded (" << state_current_vector_size << ")" << endl;
        cout << "[scheduler_module] @" << sc_time_stamp() << " next loaded (" << state_next_length << ")" << endl;
#endif

        if (state_next_length == 0) { // End of layers
            for (unsigned int i = 0; i < state_current_vector_size; i++)
            {
                to_dma.write(state_current_vector[i]);
            }
			state_get_external_inputs = false;
        } else {
			// Schedule
			for (unsigned int core_done = 0; core_done < state_next_length; core_done += CORE)
			{
				// Load cores
				// If there is less nodes than cores, do not start unused cores
				for (unsigned int i = 0; i + core_done < state_next_length && i < CORE; i++)
				{
					npu_instructions[i].write((state_current_vector_size << 2) + state_activation_function);
				}

				// Send data to cores
				for (unsigned int current_counter = 0; current_counter < state_current_vector_size; current_counter++)
				{
					#pragma HLS pipeline II=1 enable_flush
					for (unsigned int i = 0; i + core_done < state_next_length && i < CORE; i++)
					{
						npu_weight[i % CORE].write(from_dma_weight.read());
						npu_input[i % CORE].write(state_current_vector[current_counter % state_current_vector_size]);
					}
				}

				// Return output
				#pragma HLS pipeline II=1 enable_flush
				for (unsigned int i = 0; i + core_done < state_next_length && i < CORE; i++) {
					state_current_vector[i + core_done] = npu_output[i % CORE].read();
				}				
			}
        }

		// Process activation function that need full output vector
		if (state_activation_function == 3) // Softmax
        {
            float sum = 0;
            for (unsigned int i = 0; i < state_next_length; i++)
            {
                sum += state_current_vector[i];
            }

            for (unsigned int i = 0; i < state_next_length; i++)
            {
                state_current_vector[i] /= sum;
            }
        }
    }
}