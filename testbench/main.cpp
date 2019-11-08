#include <systemc.h>
#include "../headers/top.hpp"

int sc_main(int argc, char* argv[])
{
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

    // Test results:
    float assert[4] {0.11978f, 0.141543f, 0.129079f, 0.151855f};


    ///////////////////
    // SIMULATION #1 //
    ///////////////////


    // Init DMA
    dma_input.write(0.2f);
    dma_input.write(0.5f);

#if CORE == 8 || CORE == 4
    // FOR 4 CORE
    dma_weight.write(0.1f);
    dma_weight.write(0.4f);
    dma_weight.write(1.f);
    dma_weight.write(0.5f);
    dma_weight.write(0.2f);
    dma_weight.write(0.7f);
    dma_weight.write(0.f);
    dma_weight.write(0.5f); // END LAYER 1
    dma_weight.write(0.2f);
    dma_weight.write(0.9f);
    dma_weight.write(0.5f);
    dma_weight.write(0.3f);
    dma_weight.write(0.1f);
    dma_weight.write(0.5f);
    dma_weight.write(0.7f);
    dma_weight.write(0.4f);
    dma_weight.write(1.f);
    dma_weight.write(0.9f);
    dma_weight.write(0.4f);
    dma_weight.write(1.f); // END LAYER 2
    dma_weight.write(0.1f);
    dma_weight.write(0.5f);
    dma_weight.write(0.4f);
    dma_weight.write(0.7f);
    dma_weight.write(0.2f);
    dma_weight.write(0.1f); // END LAYER 3
    dma_weight.write(0.5f);
    dma_weight.write(0.2f);
    dma_weight.write(0.2f);
    dma_weight.write(0.5f); // END LAYER OUTPUT
#endif

#if CORE == 2
    // FOR 2 CORE
    dma_weight.write(0.1f);
    dma_weight.write(0.4f);
    dma_weight.write(0.2f);
    dma_weight.write(0.7f);
    dma_weight.write(1.f);
    dma_weight.write(0.5f);
    dma_weight.write(0.f);
    dma_weight.write(0.5f); // END LAYER 1
    dma_weight.write(0.2f);
    dma_weight.write(0.9f);
    dma_weight.write(0.3f);
    dma_weight.write(0.1f);
    dma_weight.write(0.7f);
    dma_weight.write(0.4f);
    dma_weight.write(0.9f);
    dma_weight.write(0.4f);
    dma_weight.write(0.5f);
    dma_weight.write(0.5f);
    dma_weight.write(1.f);
    dma_weight.write(1.f); // END LAYER 2
    dma_weight.write(0.1f);
    dma_weight.write(0.5f);
    dma_weight.write(0.4f);
    dma_weight.write(0.7f);
    dma_weight.write(0.2f);
    dma_weight.write(0.1f); // END LAYER 3
    dma_weight.write(0.5f);
    dma_weight.write(0.2f);
    dma_weight.write(0.2f);
    dma_weight.write(0.5f); // END LAYER OUTPUT
#endif

#if CORE == 1
    // FOR 1 CORE
    dma_weight.write(0.1f);
    dma_weight.write(0.2f);
    dma_weight.write(0.4f);
    dma_weight.write(0.7f);
    dma_weight.write(1.f);
    dma_weight.write(0.f);
    dma_weight.write(0.5f);
    dma_weight.write(0.5f); // END LAYER 1
    dma_weight.write(0.2f);
    dma_weight.write(0.3f);
    dma_weight.write(0.7f);
    dma_weight.write(0.9f);
    dma_weight.write(0.9f);
    dma_weight.write(0.1f);
    dma_weight.write(0.4f);
    dma_weight.write(0.4f);
    dma_weight.write(0.5f);
    dma_weight.write(0.5f);
    dma_weight.write(1.f);
    dma_weight.write(1.f); // END LAYER 2
    dma_weight.write(0.1f);
    dma_weight.write(0.4f);
    dma_weight.write(0.2f);
    dma_weight.write(0.5f);
    dma_weight.write(0.7f);
    dma_weight.write(0.1f); // END LAYER 3
    dma_weight.write(0.5f);
    dma_weight.write(0.2f);
    dma_weight.write(0.2f);
    dma_weight.write(0.5f); // END LAYER OUTPUT
#endif

    dma_config.write(2);
    dma_config.write(4);
    dma_config.write(3);
    dma_config.write(2);
    dma_config.write(2);
    dma_config.write(0); // END

    // Start simulation
    #ifndef __SYNTHESIS__
        cout << "@" << sc_time_stamp() << " Start simulation #1" << endl;
    #endif

    sc_start(100, SC_NS);

    #ifndef __SYNTHESIS__
        cout << "@" << sc_time_stamp() << " Terminating simulation #1" << endl;
    #endif

    #ifndef __SYNTHESIS__
        bool assertion = true;

        cout << endl << "=== Result ===" << endl;
        unsigned int i = 0;
        float o;
        while(dma_output.nb_read(o)) {
            cout  << o << endl;
            assertion = assertion && (fabs(o - assert[i++]) < 0.00001f);
        }
        assertion = assertion && i != 0;

        cout << endl << "Test: ";
        if (assertion) cout << "OK" << endl;
        else cout << "NOK" << endl;
    #endif

    cout << endl << "======================" << endl << endl;


    ///////////////////
    // SIMULATION #2 //
    ///////////////////


    // Init DMA
    dma_input.write(0.5f);
    dma_input.write(0.2f);

#if CORE == 4 || CORE == 8
    // FOR 4 CORE
    dma_weight.write(0.1f);
    dma_weight.write(0.4f);
    dma_weight.write(1.f);
    dma_weight.write(0.5f);
    dma_weight.write(0.2f);
    dma_weight.write(0.7f);
    dma_weight.write(0.f);
    dma_weight.write(0.5f); // END LAYER 1
    dma_weight.write(0.2f);
    dma_weight.write(0.9f);
    dma_weight.write(0.5f);
    dma_weight.write(0.3f);
    dma_weight.write(0.1f);
    dma_weight.write(0.5f);
    dma_weight.write(0.7f);
    dma_weight.write(0.4f);
    dma_weight.write(1.f);
    dma_weight.write(0.9f);
    dma_weight.write(0.4f);
    dma_weight.write(1.f); // END LAYER 2
    dma_weight.write(0.1f);
    dma_weight.write(0.5f);
    dma_weight.write(0.4f);
    dma_weight.write(0.7f);
    dma_weight.write(0.2f);
    dma_weight.write(0.1f); // END LAYER 3
    dma_weight.write(0.5f);
    dma_weight.write(0.2f);
    dma_weight.write(0.2f);
    dma_weight.write(0.5f); // END LAYER OUTPUT
#endif

#if CORE == 2
    // FOR 2 CORE
    dma_weight.write(0.1f);
    dma_weight.write(0.4f);
    dma_weight.write(0.2f);
    dma_weight.write(0.7f);
    dma_weight.write(1.f);
    dma_weight.write(0.5f);
    dma_weight.write(0.f);
    dma_weight.write(0.5f); // END LAYER 1
    dma_weight.write(0.2f);
    dma_weight.write(0.9f);
    dma_weight.write(0.3f);
    dma_weight.write(0.1f);
    dma_weight.write(0.7f);
    dma_weight.write(0.4f);
    dma_weight.write(0.9f);
    dma_weight.write(0.4f);
    dma_weight.write(0.5f);
    dma_weight.write(0.5f);
    dma_weight.write(1.f);
    dma_weight.write(1.f); // END LAYER 2
    dma_weight.write(0.1f);
    dma_weight.write(0.5f);
    dma_weight.write(0.4f);
    dma_weight.write(0.7f);
    dma_weight.write(0.2f);
    dma_weight.write(0.1f); // END LAYER 3
    dma_weight.write(0.5f);
    dma_weight.write(0.2f);
    dma_weight.write(0.2f);
    dma_weight.write(0.5f); // END LAYER OUTPUT
#endif

#if CORE == 1
    // FOR 1 CORE
    dma_weight.write(0.1f);
    dma_weight.write(0.2f);
    dma_weight.write(0.4f);
    dma_weight.write(0.7f);
    dma_weight.write(1.f);
    dma_weight.write(0.f);
    dma_weight.write(0.5f);
    dma_weight.write(0.5f); // END LAYER 1
    dma_weight.write(0.2f);
    dma_weight.write(0.3f);
    dma_weight.write(0.7f);
    dma_weight.write(0.9f);
    dma_weight.write(0.9f);
    dma_weight.write(0.1f);
    dma_weight.write(0.4f);
    dma_weight.write(0.4f);
    dma_weight.write(0.5f);
    dma_weight.write(0.5f);
    dma_weight.write(1.f);
    dma_weight.write(1.f); // END LAYER 2
    dma_weight.write(0.1f);
    dma_weight.write(0.4f);
    dma_weight.write(0.2f);
    dma_weight.write(0.5f);
    dma_weight.write(0.7f);
    dma_weight.write(0.1f); // END LAYER 3
    dma_weight.write(0.5f);
    dma_weight.write(0.2f);
    dma_weight.write(0.2f);
    dma_weight.write(0.5f); // END LAYER OUTPUT
#endif

    dma_config.write(2);
    dma_config.write(4);
    dma_config.write(3);
    dma_config.write(2);
    dma_config.write(2);
    dma_config.write(0); // END

    // Start simulation
    #ifndef __SYNTHESIS__
        cout << "@" << sc_time_stamp() << " Start simulation #2" << endl;
    #endif

    sc_start(100, SC_NS);

    #ifndef __SYNTHESIS__
        cout << "@" << sc_time_stamp() << " Terminating simulation #2" << endl;
    #endif

    #ifndef __SYNTHESIS__
        assertion = true;

        cout << endl << "=== Result ===" << endl;
        while(dma_output.nb_read(o)) {
            cout << o << endl;
            assertion = assertion && (fabs(o - assert[i++]) < 0.00001f);
        }
        assertion = assertion && i != 0;

        cout << endl << "Test: ";
        if (assertion) cout << "OK" << endl;
        else cout << "NOK" << endl;
    #endif

    ///////////////////
    // SIMULATION #3 //
    ///////////////////

    // Input
    for (unsigned int i = 0; i < 784; i++)
        dma_input.write(((float)((i+1) % 10)) / 10.0f);

    // Weights
    for (unsigned int i = 0; i < 784 * 128 + 128 * 32 + 32 * 10; i++)
        dma_weight.write(((float)(i % 10)) / 10.0f);

    dma_config.write(784);
    dma_config.write(128);
    dma_config.write(32);
    dma_config.write(10);
    dma_config.write(0); // END

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