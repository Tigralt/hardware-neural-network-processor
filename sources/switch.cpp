#include "../headers/switch.hpp"

void switch_module::process(void)
{
    float value;

    while (true)
    {
        if (in1.nb_read(value))
        {
            out.write(value);
        }
        else if (in2.nb_read(value))
        {
            out.write(value);
        }

        wait();
    }
}