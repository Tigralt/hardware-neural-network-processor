#include <systemc.h>
#include "../headers/top.hpp"

int sc_main(int argc, char* argv[])
{
    sc_report_handler::set_actions("/IEEE_Std_1666/deprecated", SC_DO_NOTHING);

    sc_set_time_resolution(100.0, SC_PS);
    sc_clock clk("clk_0",1.0,SC_NS);
    sc_signal<bool> reset;

    sc_fifo<unsigned int> dma_config(128);
    sc_fifo<float> dma_weight(2048);
    sc_fifo<float> dma_input(2048);
    sc_fifo<float> dma_output(2048);

    top_module mod_top("top");
    mod_top.clk(clk);
    mod_top.reset(reset);
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
    // dma_weight_data.write(0.1f);
    // dma_weight_data.write(0.2f);
    // dma_weight_data.write(0.4f);
    // dma_weight_data.write(0.7f);
    // dma_weight_data.write(1.f);
    // dma_weight_data.write(0.f);
    // dma_weight_data.write(0.5f);
    // dma_weight_data.write(0.5f); // END LAYER 1
    // dma_weight_data.write(0.2f);
    // dma_weight_data.write(0.3f);
    // dma_weight_data.write(0.7f);
    // dma_weight_data.write(0.9f);
    // dma_weight_data.write(0.9f);
    // dma_weight_data.write(0.1f);
    // dma_weight_data.write(0.4f);
    // dma_weight_data.write(0.4f);
    // dma_weight_data.write(0.5f);
    // dma_weight_data.write(0.5f);
    // dma_weight_data.write(1.f);
    // dma_weight_data.write(1.f); // END LAYER 2
    // dma_weight_data.write(0.1f);
    // dma_weight_data.write(0.4f);
    // dma_weight_data.write(0.2f);
    // dma_weight_data.write(0.5f);
    // dma_weight_data.write(0.7f);
    // dma_weight_data.write(0.1f); // END LAYER 3
    // dma_weight_data.write(0.5f);
    // dma_weight_data.write(0.2f);
    // dma_weight_data.write(0.2f);
    // dma_weight_data.write(0.5f); // END LAYER OUTPUT

    // FOR 2 CORE
    // dma_weight_data.write(0.1f);
    // dma_weight_data.write(0.4f);
    // dma_weight_data.write(0.2f);
    // dma_weight_data.write(0.7f);
    // dma_weight_data.write(1.f);
    // dma_weight_data.write(0.5f);
    // dma_weight_data.write(0.f);
    // dma_weight_data.write(0.5f); // END LAYER 1
    // dma_weight_data.write(0.2f);
    // dma_weight_data.write(0.9f);
    // dma_weight_data.write(0.3f);
    // dma_weight_data.write(0.1f);
    // dma_weight_data.write(0.7f);
    // dma_weight_data.write(0.4f);
    // dma_weight_data.write(0.9f);
    // dma_weight_data.write(0.4f);
    // dma_weight_data.write(0.5f);
    // dma_weight_data.write(0.5f);
    // dma_weight_data.write(1.f);
    // dma_weight_data.write(1.f); // END LAYER 2
    // dma_weight_data.write(0.1f);
    // dma_weight_data.write(0.5f);
    // dma_weight_data.write(0.4f);
    // dma_weight_data.write(0.7f);
    // dma_weight_data.write(0.2f);
    // dma_weight_data.write(0.1f); // END LAYER 3
    // dma_weight_data.write(0.5f);
    // dma_weight_data.write(0.2f);
    // dma_weight_data.write(0.2f);
    // dma_weight_data.write(0.5f); // END LAYER OUTPUT

    dma_config.write(2);
    dma_config.write(4);
    dma_config.write(3);
    dma_config.write(2);
    dma_config.write(2);
    dma_config.write(0); // END

    // Start simulation
    #ifndef __SYNTHESIS__
        cout << "@" << sc_time_stamp() << " Start simulation" << endl;
    #endif

    sc_start(100, SC_NS);

    #ifndef __SYNTHESIS__
        cout << "@" << sc_time_stamp() << " Terminating simulation" << endl;
    #endif

    #ifndef __SYNTHESIS__
        cout << endl << "=== Result ===" << endl;
        float o;
        while(dma_output.nb_read(o))
            cout << o << endl;
    #endif

    return 0;
}