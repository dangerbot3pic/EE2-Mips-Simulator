#ifndef REGISTER
#define REGISTER


#include <cinttypes>

class Register 
{
    public: 
    uint32_t *reg;
    uint32_t hi;
    uint32_t lo;

    Register();
};


#endif