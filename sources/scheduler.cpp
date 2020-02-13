#include "../headers/scheduler.hpp"

void scheduler_module::process(void)
{
#pragma HLS resource core = AXI4Stream variable = from_dma_weight
#pragma HLS resource core = AXI4Stream variable = from_dma_input
#pragma HLS resource core = AXI4Stream variable = from_dma_instructions
#pragma HLS resource core = AXI4Stream variable = to_dma

#pragma HLS array_partition variable = npu_instructions complete dim = 1
#pragma HLS array_partition variable = npu_weight complete dim = 1
#pragma HLS array_partition variable = npu_output complete dim = 1
#pragma HLS array_partition variable = npu_input complete dim = 1

#pragma HLS resource core = AXI4Stream variable = npu_instructions
#pragma HLS resource core = AXI4Stream variable = npu_weight
#pragma HLS resource core = AXI4Stream variable = npu_input
#pragma HLS resource core = AXI4Stream variable = npu_output

	sc_uint<15> instruction_input_layer, instruction_output_layer;

	while (true)
	{
		// Load instructions
		from_dma_instructions.read(state_instruction_counter);
		for (unsigned int i = 0; i < state_instruction_counter; i++)
		{
			state_instruction_buffer[i] = from_dma_instructions.read();
		}
		instruction_input_layer = (state_instruction_buffer[0] & 0b11111111111111100000000000000000) >> 17;
		instruction_output_layer = (state_instruction_buffer[state_instruction_counter - 1] & 0b00000000000000011111111111111100) >> 2;

		// Load inputs
		for (unsigned int i = 0; i < instruction_input_layer; i++)
		{
			state_input_buffer[i] = from_dma_input.read();
		}

		// Process neural network
		for (unsigned int instruction_index = 0; instruction_index < state_instruction_counter; instruction_index++)
		{
			sc_uint<15> state_current_length = (state_instruction_buffer[instruction_index] & 0b11111111111111100000000000000000) >> 17;
			sc_uint<15> state_next_length = (state_instruction_buffer[instruction_index] & 0b00000000000000011111111111111100) >> 2;
			sc_uint<2>  state_activation_function = state_instruction_buffer[instruction_index] & 0b11;

#ifndef __SYNTHESIS__
			cout << "[scheduler_module] @" << sc_time_stamp() << " inputs loaded (" << state_current_length << ")" << endl;
			cout << "[scheduler_module] @" << sc_time_stamp() << " next loaded (" << state_next_length << ")" << endl;
#endif

			// Schedule
			for (unsigned int core_done = 0; core_done < state_next_length; core_done += CORE)
			{
				// Load cores
				// If there is less nodes than cores, do not start unused cores
				for (unsigned int i = 0; i + core_done < state_next_length && i < CORE; i++)
				{
					npu_instructions[i].write((state_current_length << 2) + state_activation_function);
				}

				// Send data to cores
				for (unsigned int current_counter = 0; current_counter < state_current_length; current_counter++)
				{
					#pragma HLS pipeline II = 1 enable_flush
					for (unsigned int i = 0; i + core_done < state_next_length && i < CORE; i++)
					{
						npu_weight[i % CORE].write(from_dma_weight.read());
						npu_input[i % CORE].write(state_input_buffer[current_counter % state_current_length]);
					}
				}

				// Return output
				#pragma HLS pipeline II = 1 enable_flush
				for (unsigned int i = 0; i + core_done < state_next_length && i < CORE; i++)
				{
					state_output_buffer[i + core_done] = npu_output[i % CORE].read();
				}
			}

			// Process activation function that need full output vector
			if (state_activation_function == 3) // Softmax
			{
				float sum = 0;
				for (unsigned int i = 0; i < state_next_length; i++)
				{
					sum += state_output_buffer[i];
				}

				for (unsigned int i = 0; i < state_next_length; i++)
				{
					state_output_buffer[i] /= sum;
				}
			}

			for (unsigned int i = 0; i < instruction_output_layer; i++)
			{
				state_input_buffer[i] = state_output_buffer[i];
			}
		}

		for (unsigned int i = 0; i < instruction_output_layer; i++)
		{
			to_dma.write(state_input_buffer[i]);
		}
	}
}