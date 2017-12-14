#ifndef MEMORY
#define MEMORY


#include <cinttypes>

class Memory 
{
    public:
    Memory();
    int store_instruction(uint32_t location, uint8_t st);
    uint32_t load_instruction(uint32_t location);



    private: 
    uint8_t *byte;
};


#endif