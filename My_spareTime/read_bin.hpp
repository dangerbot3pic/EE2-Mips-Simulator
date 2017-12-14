#ifndef READ_BIN
#define READ_BIN

#include <fstream>

#include "memory.hpp"

int read_bin(std::ifstream binfile, Memory &RAM)
{
    char in;
    uint8_t out = 0;
    uint32_t location = 0;

    while (!binfile.eof())
    {
        for (int i = 0; i < 8; i++)
        {
            binfile>>in;
            switch (in)
            {
                case '1': out += 1<<(7-i); break;
                default: ;
            }
            if (binfile.eof()) break;
        }

        if (RAM.store_instruction(location, out) == -1) return -1;
        out = 0;
        location++;
    }
    return 0;
}


#endif