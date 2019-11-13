#include <systemc.h>
#include "../headers/top.hpp"

void system_pause() {
    std::cout << "Press enter to continue ...";
    std::cin.get();
}

int sc_main(int argc, char* argv[])
{
    float o;

    sc_report_handler::set_actions("/IEEE_Std_1666/deprecated", SC_DO_NOTHING);

    sc_set_time_resolution(100.0, SC_PS);
    sc_clock clk("clk_0",1.0,SC_NS);
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


    ///////////////////
    // SIMULATION #1 //
    ///////////////////


    // Init DMA
    dma_input.write(0.0f);
    dma_input.write(0.0f);

#if CORE == 8 || CORE == 4 || CORE == 2
    dma_weight.write(-1.347858071327209473e+00);
    dma_weight.write(1.556262016296386719e+00);
    dma_weight.write(1.326483130455017090e+00);
    dma_weight.write(-1.615458250045776367e+00); // END LAYER 1
    dma_weight.write(-2.849910736083984375e+00);
    dma_weight.write(-2.041214227676391602e+00);
    dma_weight.write(-2.712538480758666992e+00);
    dma_weight.write(-9.970329403877258301e-01); // END LAYER 2
    dma_weight.write(-2.038681268692016602e+00);
    dma_weight.write(-1.361018419265747070e+00);  // END LAYER OUTPUT
#endif

#if CORE == 1
    dma_weight.write(-1.347858071327209473e+00);
    dma_weight.write(1.326483130455017090e+00);
    dma_weight.write(1.556262016296386719e+00);
    dma_weight.write(-1.615458250045776367e+00); // END LAYER 1
    dma_weight.write(-2.849910736083984375e+00);
    dma_weight.write(-2.712538480758666992e+00);
    dma_weight.write(-2.041214227676391602e+00);
    dma_weight.write(-9.970329403877258301e-01); // END LAYER 2
    dma_weight.write(-2.038681268692016602e+00);
    dma_weight.write(-1.361018419265747070e+00);  // END LAYER OUTPUT
#endif

    dma_config.write((2 << 2)); // Input
    dma_config.write((2 << 2) + 2); // Hidden 1
    dma_config.write((2 << 2) + 0); // Hidden 2
    dma_config.write((1 << 2) + 1); // Output
    dma_config.write((0 << 2)); // End Of Layers

    // Start simulation
    #ifndef __SYNTHESIS__
        cout << "@" << sc_time_stamp() << " Start simulation #1" << endl;
    #endif

    sc_start(100, SC_NS);

    #ifndef __SYNTHESIS__
        cout << "@" << sc_time_stamp() << " Terminating simulation #1" << endl;
    #endif

    #ifndef __SYNTHESIS__
        cout << endl << "=== Result ===" << endl;
        while(dma_output.nb_read(o)) {
            cout  << o << endl;
        }
        cout << "======================" << endl << endl;
        system_pause();
    #endif

    ///////////////////
    // SIMULATION #2 //
    ///////////////////


    // Init DMA
    dma_input.write(1.0f);
    dma_input.write(0.0f);

#if CORE == 8 || CORE == 4 || CORE == 2
    dma_weight.write(-1.347858071327209473e+00);
    dma_weight.write(1.556262016296386719e+00);
    dma_weight.write(1.326483130455017090e+00);
    dma_weight.write(-1.615458250045776367e+00); // END LAYER 1
    dma_weight.write(-2.849910736083984375e+00);
    dma_weight.write(-2.041214227676391602e+00);
    dma_weight.write(-2.712538480758666992e+00);
    dma_weight.write(-9.970329403877258301e-01); // END LAYER 2
    dma_weight.write(-2.038681268692016602e+00);
    dma_weight.write(-1.361018419265747070e+00);  // END LAYER OUTPUT
#endif

#if CORE == 1
    dma_weight.write(-1.347858071327209473e+00);
    dma_weight.write(1.326483130455017090e+00);
    dma_weight.write(1.556262016296386719e+00);
    dma_weight.write(-1.615458250045776367e+00); // END LAYER 1
    dma_weight.write(-2.849910736083984375e+00);
    dma_weight.write(-2.712538480758666992e+00);
    dma_weight.write(-2.041214227676391602e+00);
    dma_weight.write(-9.970329403877258301e-01); // END LAYER 2
    dma_weight.write(-2.038681268692016602e+00);
    dma_weight.write(-1.361018419265747070e+00);  // END LAYER OUTPUT
#endif

    dma_config.write((2 << 2)); // Input
    dma_config.write((2 << 2) + 2); // Hidden 1
    dma_config.write((2 << 2) + 0); // Hidden 2
    dma_config.write((1 << 2) + 1); // Output
    dma_config.write((0 << 2)); // End Of Layers

    // Start simulation
    #ifndef __SYNTHESIS__
        cout << "@" << sc_time_stamp() << " Start simulation #1" << endl;
    #endif

    sc_start(100, SC_NS);

    #ifndef __SYNTHESIS__
        cout << "@" << sc_time_stamp() << " Terminating simulation #1" << endl;
    #endif

    #ifndef __SYNTHESIS__
        cout << endl << "=== Result ===" << endl;
        while(dma_output.nb_read(o)) {
            cout  << o << endl;
        }
        cout << "======================" << endl << endl;
        system_pause();
    #endif


    ///////////////////
    // SIMULATION #3 //
    ///////////////////

    // Input
    for (unsigned int i = 0; i < 784; i++)
        dma_input.write(((float)((i+1) % 10)) / 10000000.0f);

    // Weights
    for (unsigned int i = 0; i < 784 * 128 + 128 * 32 + 32 * 10; i++)
        dma_weight.write(((float)(i % 10)) / 10000000.0f);

    dma_config.write((784 << 2)); // Input
    dma_config.write((128 << 2) + 2); // Hidden 1
    dma_config.write((32 << 2) + 0); // Hidden 2
    dma_config.write((10 << 2) + 3); // Output
    dma_config.write(0); // End Of Layers

    // Start simulation
    #ifndef __SYNTHESIS__
        cout << "@" << sc_time_stamp() << " Start simulation #3" << endl;
    #endif

    sc_start(100000, SC_NS);

    #ifndef __SYNTHESIS__
        cout << "@" << sc_time_stamp() << " Terminating simulation #3" << endl;
    #endif

    #ifndef __SYNTHESIS__
        cout << endl << "=== Result ===" << endl;
        while(dma_output.nb_read(o)) {
            cout << o << endl;
        }
    #endif

    return 0;
}