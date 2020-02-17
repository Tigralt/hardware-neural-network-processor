#include <systemc.h>
#include <cnpy.h> // https://github.com/rogersce/cnpy
#include "../headers/top.hpp"

void system_pause()
{
    std::cout << "Press enter to continue ...";
    std::cin.get();
}

int sc_main(int argc, char *argv[])
{
    // Load numpy data
    cnpy::npz_t mnist_layers = cnpy::npz_load("./nn_arch/mnist/layers/mnist.npz");
    cnpy::npz_t mnist_dataset = cnpy::npz_load("./nn_arch/mnist/dataset/mnist.npz");

    // Init SystemC system
    sc_report_handler::set_actions("/IEEE_Std_1666/deprecated", SC_DO_NOTHING);

    sc_set_time_resolution(100.0, SC_PS);
    sc_clock clk("clk_0", 1.0, SC_NS);
    sc_signal<bool> reset;

    sc_fifo<unsigned int> dma_config(128);
    sc_fifo<float> dma_weight(131072);
    sc_fifo<float> dma_input(2048);
    sc_fifo<float> dma_output(2048);

    top_module mod_top("top");
    mod_top.clk(clk);
    mod_top.reset(reset);
    mod_top.dma_config(dma_config);
    mod_top.dma_weight(dma_weight);
    mod_top.dma_input(dma_input);
    mod_top.dma_output(dma_output);

    ////////////////////
    //   SIMULATION   //
    ////////////////////
    float *input = mnist_dataset["x"].data<float>();
    char *output = mnist_dataset["y"].data<char>();
    size_t correct_classification = 0;
    std::vector<float> results;
    float o;

    for (size_t n = 0; n < mnist_dataset["x"].shape[0]; n++)
    {
        // Instructions
        dma_config.write(3);
        dma_config.write((mnist_layers["layer_0"].shape[0] << 17) + (mnist_layers["layer_0"].shape[1] << 2) + 2); // Relu
        dma_config.write((mnist_layers["layer_1"].shape[0] << 17) + (mnist_layers["layer_1"].shape[1] << 2) + 0); // Linear
        dma_config.write((mnist_layers["layer_2"].shape[0] << 17) + (mnist_layers["layer_2"].shape[1] << 2) + 3); // Softmax

        // Weights
        size_t core = CORE;
        for (cnpy::npz_t::iterator it = mnist_layers.begin(); it != mnist_layers.end(); it++)
        {
            float *data = it->second.data<float>();
            for (size_t offset = 0; offset < it->second.shape[1]; offset += CORE)
            {
                size_t range = std::min(std::min(core, it->second.shape[1]), it->second.shape[1] - offset);
                for (size_t node = 0; node < it->second.shape[0]; node++)
                {
                    for (size_t i = 0; i < range; i++)
                    {
                        dma_weight.write(data[node * it->second.shape[1] + offset + i]);
                    }
                }
            }
        }

        // Inputs
        for (size_t i = 0; i < mnist_dataset["x"].shape[1]; i++)
        {
            dma_input.write(input[n * mnist_dataset["x"].shape[1] + i]);
        }

// Start simulation
#if VERBOSITY_LEVEL >= 1
        cout << "@" << sc_time_stamp() << " Start simulation #" << (n + 1) << endl;
#endif

        sc_start(100000, SC_NS);

#if VERBOSITY_LEVEL >= 1
        cout << "@" << sc_time_stamp() << " Terminating simulation #" << (n + 1) << endl;
#endif

        // Determine accuracy
        while (dma_output.nb_read(o))
        {
            results.push_back(o);
        }
        int maxElementIndex = std::max_element(results.begin(),results.end()) - results.begin();
        if (maxElementIndex == (int) output[n])
            correct_classification++;

#if VERBOSITY_LEVEL >= 2
        cout << "=== Result ===" << endl;
        for (std::vector<float>::iterator it = results.begin(); it != results.end(); it++)
        {
            cout << *it << endl;
        }
        cout << "======================" << endl;

        if (maxElementIndex == (int) output[n]) {
            cout << "Classification is correct: found (#" << (int)output[n] << ")" << endl;
        } else {
            cout << "Classification is incorrect: found (#" << maxElementIndex << ") instead of (#" << (int)output[n] << ")" << endl;
        }
        cout << "Class should be: #" << (int)output[n] << endl;
        system_pause();
#endif

        results.clear();
    }

    cout << "Accuracy: " << (float) correct_classification / (float) mnist_dataset["x"].shape[0] << endl;

    return 0;
}