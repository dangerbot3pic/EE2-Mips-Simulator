#include <cinttypes>
#include <stdexcept>

#include "memory.hpp"

#define MEM_SIZE 0x40000000 // 1 GB of RAM
#define NULL_START 0x00000000
#define NULL_END 0x00000000
#define EXEC_START 0x10000000
#define EXEC_END 0x11000000
#define RW_START 0x20000000
#define RW_END 0x24000000
#define GET_C_START 0x30000000
#define GET_C_END 0x30000004
#define PUT_C_START 0x30000004
#define PUT_C_END 0x30000008

Memory::Memory()
{
    uint8_t *byte = new uint8_t [MEM_SIZE];

    for (int i = 0; i < MEM_SIZE; i++) byte[i] = 0;
}


int Memory::store_instruction(uint32_t location, uint8_t st)
{
    location += EXEC_START;

    if (location > EXEC_END)
    {
        return -1;
    }

    byte[location] = st;

    return 0;
}


uint32_t Memory::load_instruction(uint32_t location)
{
    if (location < RW_START || location >= RW_END) throw std::range_error("");

    uint32_t tmp = byte[location]<<24;
    tmp += byte[location+1]<<16;
    tmp += byte[location+2]<<8;
    tmp += byte[location+3];
    
    return tmp;
}