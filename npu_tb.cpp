#include "systemc.h"
#define DEBUG 1

#include "top.cpp"

int sc_main(int argc, char* argv[])
{
    sc_fifo<int> dma_config(128);
    sc_fifo<float> dma_weight(2048);
    sc_fifo<float> dma_input(2048);
    sc_fifo<float> dma_output(2048);

    top_module mod_top("top_0");
    mod_top.dma_config(dma_config);
    mod_top.dma_weight(dma_weight);
    mod_top.dma_input(dma_input);
    mod_top.dma_output(dma_output);

    // Init DMA
    dma_input.write(0.5f);
    dma_input.write(0.2f);

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

    // FOR 1 CORE
    // dma_weight.write(0.1f);
    // dma_weight.write(0.2f);
    // dma_weight.write(0.4f);
    // dma_weight.write(0.7f);
    // dma_weight.write(1.f);
    // dma_weight.write(0.f);
    // dma_weight.write(0.5f);
    // dma_weight.write(0.5f); // END LAYER 1
    // dma_weight.write(0.2f);
    // dma_weight.write(0.3f);
    // dma_weight.write(0.7f);
    // dma_weight.write(0.9f);
    // dma_weight.write(0.9f);
    // dma_weight.write(0.1f);
    // dma_weight.write(0.4f);
    // dma_weight.write(0.4f);
    // dma_weight.write(0.5f);
    // dma_weight.write(0.5f);
    // dma_weight.write(1.f);
    // dma_weight.write(1.f); // END LAYER 2
    // dma_weight.write(0.1f);
    // dma_weight.write(0.4f);
    // dma_weight.write(0.2f);
    // dma_weight.write(0.5f);
    // dma_weight.write(0.7f);
    // dma_weight.write(0.1f); // END LAYER 3
    // dma_weight.write(0.5f);
    // dma_weight.write(0.2f);
    // dma_weight.write(0.2f);
    // dma_weight.write(0.5f); // END LAYER OUTPUT

    // FOR 2 CORE
    // dma_weight.write(0.1f);
    // dma_weight.write(0.4f);
    // dma_weight.write(0.2f);
    // dma_weight.write(0.7f);
    // dma_weight.write(1.f);
    // dma_weight.write(0.5f);
    // dma_weight.write(0.f);
    // dma_weight.write(0.5f); // END LAYER 1
    // dma_weight.write(0.2f);
    // dma_weight.write(0.9f);
    // dma_weight.write(0.3f);
    // dma_weight.write(0.1f);
    // dma_weight.write(0.7f);
    // dma_weight.write(0.4f);
    // dma_weight.write(0.9f);
    // dma_weight.write(0.4f);
    // dma_weight.write(0.5f);
    // dma_weight.write(0.5f);
    // dma_weight.write(1.f);
    // dma_weight.write(1.f); // END LAYER 2
    // dma_weight.write(0.1f);
    // dma_weight.write(0.5f);
    // dma_weight.write(0.4f);
    // dma_weight.write(0.7f);
    // dma_weight.write(0.2f);
    // dma_weight.write(0.1f); // END LAYER 3
    // dma_weight.write(0.5f);
    // dma_weight.write(0.2f);
    // dma_weight.write(0.2f);
    // dma_weight.write(0.5f); // END LAYER OUTPUT

    dma_config.write(2);
    dma_config.write(4);
    dma_config.write(3);
    dma_config.write(2);
    dma_config.write(2);
    dma_config.write(0); // END

    // Start simulation
    cout << "@" << sc_time_stamp() << " Start simulation" << endl;
    sc_start(1, SC_NS);
    cout << "@" << sc_time_stamp() << " Terminating simulation" << endl;

    cout << endl << "=== Result ===" << endl;
    float _o;
    while(dma_output.nb_read(_o))
        cout << _o << endl;

    return 0;
}