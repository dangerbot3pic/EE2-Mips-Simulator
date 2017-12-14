#ifndef PROGRAMCOUNTER
#define PROGRAMCOUNTER

#include <cinttypes>


class ProgramCounter 
{
    public: 
    ProgramCounter();
    void increment();
    void store(uint32_t newPC, bool jb);
    uint32_t get();


    private: 
    uint32_t PC;
    uint32_t PC_next;
    bool hold;
};


#endif