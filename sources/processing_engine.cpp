#include "../headers/processing_engine.hpp"

void processing_engine_module::process(void)
{
#pragma HLS resource core = AXI4Stream variable = from_scheduler_weight
#pragma HLS resource core = AXI4Stream variable = from_scheduler_input
#pragma HLS resource core = AXI4Stream variable = from_scheduler_instructions
#pragma HLS resource core = AXI4Stream variable = to_scheduler

    // Init
    unsigned int instructions;
    float input, weight, output;

    while (true)
    {
        output = 0.f;
        from_scheduler_instructions.read(instructions);
        state_length = instructions >> 2;
        state_activation_function = instructions & 0b11;

#ifndef __SYNTHESIS__
        cout << "[processing_engine_module] @" << sc_time_stamp() << " loading length (" << state_length << ")" << endl;
#endif

// Process
#pragma HLS pipeline II = 1 enable_flush
        for (unsigned int i = 0; i < state_length; i++)
        {
            from_scheduler_input.read(input);
            from_scheduler_weight.read(weight);
            output += weight * input;
        }

        if (state_activation_function == 1) // Sigmoid
        {
            output = sigmoid(output);
        }
        else if (state_activation_function == 2) // Relu
        {
            output = relu(output);
        }
        else if (state_activation_function == 3) // Softmax
        {
            output = softmax(output);
        }

        to_scheduler.write(output);

#ifndef __SYNTHESIS__
        cout << "[processing_engine_module] @" << sc_time_stamp() << " returning result (" << output << ")" << endl;
#endif
    }
}

float processing_engine_module::sigmoid(float input)
{
    return 1.f / (1.f + exp(-input));
}

float processing_engine_module::relu(float input)
{
    return input < 0.f? 0.f: input;
}

float processing_engine_module::softmax(float input)
{
    return exp(input);
}