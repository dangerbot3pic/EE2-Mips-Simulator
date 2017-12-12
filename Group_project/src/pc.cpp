#include <iostream>
#include "headers/pc.hpp"


program_counter::program_counter()
: delay(false), PC(0), PC_prev(4) {}


uint32_t program_counter::get_PC() const
{
    return PC;
}


uint32_t program_counter::get_PC_prev() const 
{
    return PC_prev;
}


void program_counter::increment()
{
    if (delay == false)
    {
        PC = PC_prev;
        PC_prev += 4;
        return;
    }

        PC += 4;
        delay = false;
}


void program_counter::load_PC(uint32_t instr, bool lag)
{
    PC_prev = instr;
    delay = lag;
}
