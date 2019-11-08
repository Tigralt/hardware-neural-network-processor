#include "../headers/processing_engine.hpp"

void processing_engine_module::process(void)
{
#pragma HLS resource core = AXI4Stream variable = from_scheduler_weight
#pragma HLS resource core = AXI4Stream variable = from_scheduler_input
#pragma HLS resource core = AXI4Stream variable = from_scheduler_length
#pragma HLS resource core = AXI4Stream variable = to_scheduler

    // Init
    float input, weight, output;

    while (true)
    {
    	output = 0.f;
		from_scheduler_length.read(state_length);

#ifndef __SYNTHESIS__
		cout << "[processing_engine_module] @" << sc_time_stamp() << " loading length (" << state_length << ")" << endl;
#endif

        // Process
		#pragma HLS pipeline II=1 enable_flush
        for (unsigned int i = 0; i < state_length; i++)
        {
            from_scheduler_input.read(input);
            from_scheduler_weight.read(weight);

            output += weight * input;
        }
        output = activation_function(output);
        to_scheduler.write(output);

#ifndef __SYNTHESIS__
        cout << "[processing_engine_module] @" << sc_time_stamp() << " returning result (" << output << ")" << endl;
#endif
    }
}

float processing_engine_module::activation_function(float input)
{
    return 1 / (1 + exp(-input));
}