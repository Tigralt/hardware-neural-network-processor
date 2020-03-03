#include <systemc.h>
#include <cnpy.h> // https://github.com/rogersce/cnpy
#include <regex>
#include "../headers/top.hpp"
#include "lib/tqdm.hpp"

void system_pause()
{
    std::cout << "Press enter to continue ...";
    std::cin.get();
}

int sc_main(int argc, char *argv[])
{
    // Progress bar
    tqdm bar;

    // Load numpy data
    cnpy::npz_t layers = cnpy::npz_load("./models/mnist/layers.npz");
    cnpy::npz_t dataset = cnpy::npz_load("./models/mnist/dataset.npz");

    // Init SystemC system
    sc_report_handler::set_actions("/IEEE_Std_1666/deprecated", SC_DO_NOTHING);

    sc_set_time_resolution(100.0, SC_PS);
    sc_clock clk("clk_0", 1.0, SC_NS);
    sc_signal<bool> reset;

    sc_fifo<sc_uint<32>> dma_config(512);
    sc_fifo<float> dma_weight(8388608);
    sc_fifo<float> dma_input(32768);
    sc_fifo<float> dma_output(32768);

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
    float *input = dataset["x"].data<float>();
    char *output = dataset["y"].data<char>();
    size_t correct_classification = 0;
    std::vector<float> results;
    float o;

    for (size_t n = 0; n < dataset["x"].shape[0]; n++)
    {
#if VERBOSITY_LEVEL == 0
        bar.progress(n, dataset["x"].shape[0]);
#endif

        // Instructions number
        dma_config.write(layers.size());

        // Load weights and instructions
        size_t core = CORE;
        for (cnpy::npz_t::iterator it = layers.begin(); it != layers.end(); it++)
        {
            // Instructions
            std::regex re("a\\d+\\_([a-z]+)\\_\\d+");
            std::smatch match;
            std::regex_search(it->first, match, re);
            std::string result = match.str(1);
            unsigned int activation = 0;
            if (result == "sigmoid")
            {
                activation = 1;
            }
            else if (result == "relu")
            {
                activation = 2;
            }
            else if (result == "softmax")
            {
                activation = 3;
            }
            dma_config.write((it->second.shape[0] << 17) + (it->second.shape[1] << 2) + activation);

            // Weights
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
        for (size_t i = 0; i < dataset["x"].shape[1]; i++)
        {
            dma_input.write(input[n * dataset["x"].shape[1] + i]);
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
        int maxElementIndex = std::max_element(results.begin(), results.end()) - results.begin();
        if (maxElementIndex == (int)output[n])
            correct_classification++;

#if VERBOSITY_LEVEL >= 2
        cout << "=== Result ===" << endl;
        for (std::vector<float>::iterator it = results.begin(); it != results.end(); it++)
        {
            cout << *it << endl;
        }
        cout << "======================" << endl;

        if (maxElementIndex == (int)output[n])
        {
            cout << "Classification is correct: found (#" << (int)output[n] << ")" << endl;
        }
        else
        {
            cout << "Classification is incorrect: found (#" << maxElementIndex << ") instead of (#" << (int)output[n] << ")" << endl;
        }
        system_pause();
#endif

        results.clear();
    }

#if VERBOSITY_LEVEL == 0
    bar.finish();
#endif

    cout << "Accuracy: " << (float)correct_classification / (float)dataset["x"].shape[0] * 100 << "%" << endl;

    return 0;
}