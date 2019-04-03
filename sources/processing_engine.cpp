#include "../headers/processing_engine.hpp"

void processing_engine_module::process(void)
{
    while (true)
    {
        // Wait for length
        if (state_length == 0)
        {
            from_scheduler_length.read(state_length);

#ifndef __SYNTHESIS__
            cout << "[processing_engine_module] @" << sc_time_stamp() << " loading length (" << state_length << ")" << endl;
#endif
        }

        // Init
        float input, weight;
        float output = 0.f;

        // Process
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

        // If no more data, reset length
        if (from_scheduler_input.num_available() == 0 && from_scheduler_weight.num_available() == 0)
        {
            state_length = 0;
        }

        wait();
    }
}

float processing_engine_module::activation_function(float input)
{
    return input / (1 + abs(input));
}