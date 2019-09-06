#include "../headers/processing_engine.hpp"

void processing_engine_module::process(void)
{
    while (true)
    {
        from_scheduler_length.read(state_length);

#ifndef __SYNTHESIS__
            cout << "[processing_engine_module] @" << sc_time_stamp() << " loading length (" << state_length << ")" << endl;
#endif

        // Init
        float input, weight;
        float output = 0.f;

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
    return input / (1 + abs(input));
}