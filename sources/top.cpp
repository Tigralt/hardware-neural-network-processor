#include "../headers/top.hpp"

top_module::top_module(sc_module_name name) : clk("clock"),
                                              mod_config("config"),
                                              mod_weight("weight"),
                                              mod_io("io"),
                                              mod_scheduler("scheduler"),
                                              mod_switch("switch"),
#if CORE == 4
                                              mod_core_1("mod_core_1"),
                                              mod_core_2("mod_core_2"),
                                              mod_core_3("mod_core_3"),
                                              mod_core_4("mod_core_4")
#elif CORE == 2
                                              mod_core_1("mod_core_1"),
                                              mod_core_2("mod_core_2")
#elif CORE == 1
                                              mod_core_1("mod_core_1")
#endif
{
    mod_config.clk(clk);
    mod_config.reset(reset);
    mod_config.from_dma(dma_config);
    mod_config.to_io(fifo_input_length);
    mod_config.to_weight(fifo_weight_length);
    mod_config.to_scheduler_current(fifo_current_length);
    mod_config.to_scheduler_next(fifo_next_length);

    mod_weight.clk(clk);
    mod_weight.reset(reset);
    mod_weight.from_dma(dma_weight);
    mod_weight.from_config(fifo_weight_length);
    mod_weight.to_scheduler(fifo_weight);

    mod_io.clk(clk);
    mod_io.reset(reset);
    mod_io.inputs(fifo_switch);
    mod_io.to_dma(dma_output);
    mod_io.from_config(fifo_input_length);
    mod_io.to_scheduler(fifo_input);

    mod_scheduler.clk(clk);
    mod_scheduler.reset(reset);
    mod_scheduler.from_weight(fifo_weight);
    mod_scheduler.from_io(fifo_input);
    mod_scheduler.to_io(fifo_output);
    mod_scheduler.from_config_current(fifo_current_length);
    mod_scheduler.from_config_next(fifo_next_length);

    mod_switch.clk(clk);
    mod_switch.reset(reset);
    mod_switch.in1(dma_input);
    mod_switch.in2(fifo_output);
    mod_switch.out(fifo_switch);

    for (unsigned int i = 0; i < CORE; i++)
    {
        mod_scheduler.npu_length[i](npu_length[i]);
        mod_scheduler.npu_weight[i](npu_weight[i]);
        mod_scheduler.npu_output[i](npu_output[i]);
        mod_scheduler.npu_input[i](npu_input[i]);
    }

#if CORE == 4
    CORE_BIND(mod_core_1, 0);
    CORE_BIND(mod_core_2, 1);
    CORE_BIND(mod_core_3, 2);
    CORE_BIND(mod_core_4, 3);
#elif CORE == 2
    CORE_BIND(mod_core_1, 0);
    CORE_BIND(mod_core_2, 1);
#elif CORE == 1
    CORE_BIND(mod_core_1, 0);
#endif
}