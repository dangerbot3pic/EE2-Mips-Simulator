

#include "pc.hpp"


ProgramCounter::ProgramCounter() : PC(0), PC_next(4), hold(false) {}


void ProgramCounter::increment()
{
    if (hold) PC += 4; hold = false; return;
    PC = PC_next;
    PC_next += 4;
}


void ProgramCounter::store(uint32_t newPC, bool jb)
{
    if (jb) hold = true;

    PC_next = newPC;
}


uint32_t ProgramCounter::get()
{
    return PC;
}