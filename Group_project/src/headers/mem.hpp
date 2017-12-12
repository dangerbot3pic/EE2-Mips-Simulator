#ifndef MEM_HPP
#define MEM_HPP

#include <iostream>
#include <cinttypes>

class memory
{
    public:

    memory();
    uint32_t get_instruction(uint32_t location) const;
    void store_instruction(uint32_t loc, uint32_t inst);
    uint32_t load_word(uint32_t loc);
    uint32_t load_byte(uint32_t loc) const;
    uint32_t load_hword(uint32_t loc) const;
    void store_word(uint32_t loc, const int32_t input);
    void store_hword(uint32_t loc, const int32_t input);
    void store_byte(uint32_t loc, const int32_t input);
    uint32_t get_c(uint32_t loc_corrected);
    void put_c(int32_t item);
    uint32_t load_word_left(uint32_t loc);
    uint32_t load_word_right(uint32_t loc);

    private: 

    uint32_t *mem;
};


#endif
