#ifndef DECODER
#define DECODER


#include <stdexcept>

#include "memory.hpp"
#include "pc.hpp"


int decoder(Memory &RAM, ProgramCounter &PC)
{
    try 
    {
        uint32_t instruction = RAM.load_instruction(PC.get()); // Load instruction
    }

    catch (std::range_error &a) // Catch out of range access
    {
        return -1;
    }
}


int R_Type(uint32_t instruction)
{
    
}


#endif