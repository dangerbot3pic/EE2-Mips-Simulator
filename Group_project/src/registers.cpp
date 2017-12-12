#include <iostream>
#include "headers/registers.hpp"


registers::registers()
{
    for (int i = 0; i < 32; i++)
    {
        reg[i] = 0;
    }
    
    hi = 0;
    lo = 0;
}
