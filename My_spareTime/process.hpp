#ifndef DECODER
#define DECODER


#include <stdexcept>
#include <cstdlib>
#include <cinttyes>

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
        exit(-11);// Invalid memory access prompts exit
    }
}


int R_Type(uint32_t instruction)
{
    try 
    {
        const uint8_t rs = (instruction >> 21) & 0b11111;
        const uint8_t rt = (instruction >> 16) & 0b11111;
        const uint8_t rd = (instruction >> 11) & 0b11111;
        const uint8_t shift_size = (instruction >> 6) & 0b11111;
        const uint8_t funct = instruction & 0b111111;

        unsigned long long temp = 0;


         switch (funct)
    {
    		case 0x20    : if (addition_exception(rs, rt)) {return 1;} CPUreg.reg[rd] = CPUreg.reg[rs] + CPUreg.reg[rt]; return_val = CPUreg.reg[rd];return 0;//add
            case 0x21    : CPUreg.reg[rd] = (unsigned)CPUreg.reg[rs] + (unsigned)CPUreg.reg[rt]; return_val = CPUreg.reg[rd];return 0;// addu
            case 0x22    : if (subtraction_exception(CPUreg.reg[rs], CPUreg.reg[rt])) {exit (1);} CPUreg.reg[rd] = CPUreg.reg[rs] - CPUreg.reg[rt]; return_val = CPUreg.reg[rd];return 0; // sub
            case 0x23    : if (subtraction_exception(CPUreg.reg[rs], CPUreg.reg[rt])) {exit (1);} CPUreg.reg[rd] = (unsigned)CPUreg.reg[rs] - (unsigned)CPUreg.reg[rt]; return_val = CPUreg.reg[rd];return 0; // subu
            case 0x18    : temp=(CPUreg.reg[rs]*CPUreg.reg[rt]);// mult
                           CPUreg.hi=(temp>>32);
                           CPUreg.lo=(temp & 0xFFFF); return 0;
            case 0x19    : temp=((unsigned)CPUreg.reg[rt]*(unsigned)CPUreg.reg[rs]);// multu
                           CPUreg.hi=temp>>32;
                           CPUreg.lo=(temp<<32)>>32; return 0;
            case 0x1a    : if(division_error(rs, rt)) {CPUreg.reg[rd] = 1;} // div
                            else {CPUreg.hi = CPUreg.reg[rs] % CPUreg.reg[rt]; CPUreg.lo = CPUreg.reg[rs]/CPUreg.reg[rt]; } return 0;
            case 0x1b    : if(division_error(rs, rt)) {CPUreg.reg[rd] = 1;} // divu
                            else {CPUreg.hi = (unsigned)CPUreg.reg[rs] % (unsigned)CPUreg.reg[rt]; CPUreg.lo = (unsigned)CPUreg.reg[rs]/(unsigned)CPUreg.reg[rt]; } return 0;
            case 0x10    : CPUreg.reg[rd]=CPUreg.hi; return_val = CPUreg.reg[rd];return 0;// mfhi
            case 0x12    : CPUreg.reg[rd]=CPUreg.lo; return_val = CPUreg.reg[rd];return 0;// mflo
            case 0x24    : CPUreg.reg[rd] = CPUreg.reg[rs] & CPUreg.reg[rt]; return_val = CPUreg.reg[rd];return 0;// and
            case 0x25    : CPUreg.reg[rd] = CPUreg.reg[rs] | CPUreg.reg[rt]; return_val = CPUreg.reg[rd];return 0;// or
            case 0x26    : CPUreg.reg[rd] = CPUreg.reg[rs] ^ CPUreg.reg[rt]; return_val = CPUreg.reg[rd];return 0;// xor
            case 0x27    : CPUreg.reg[rd] = ~(CPUreg.reg[rs] | CPUreg.reg[rt]);return_val = CPUreg.reg[rd];return 0; // nor
            case 0x2a    : if(CPUreg.reg[rt]<CPUreg.reg[rd]) CPUreg.reg[rs]=1; return 0; // slt
            case 0x2b    : if(CPUreg.reg[rt]<CPUreg.reg[rd]) CPUreg.reg[rs]=1; return 0; // sltu
            case 0x00    : CPUreg.reg[rd] = CPUreg.reg[rt]<<shift_size; return_val = CPUreg.reg[rd]; return 0;// sll
            case 0x04    : CPUreg.reg[rd] = CPUreg.reg[rt]<<(CPUreg.reg[rs]); return_val = CPUreg.reg[rd]; return 0; // sllv
            case 0x02    : CPUreg.reg[rd] = CPUreg.reg[rt]>>shift_size; return_val = CPUreg.reg[rd]; return 0; // srl
            case 0x06    : CPUreg.reg[rd] = CPUreg.reg[rt]>>(CPUreg.reg[rs]); return_val = CPUreg.reg[rd];return 0; // srlv
            case 0x03    : CPUreg.reg[rd] = arithmetic_shift_right(CPUreg.reg[rt], shift_size); return_val = CPUreg.reg[rd];return 0; // sra
            case 0x07    : CPUreg.reg[rd] = arithmetic_shift_right(CPUreg.reg[rt], CPUreg.reg[rs]); return_val = CPUreg.reg[rd];return 0;// srav
            case 0x11    : CPUreg.hi = CPUreg.reg[rd]; // mthi
            case 0x13    : CPUreg.lo = CPUreg.reg[rd]; // mtlo
            case 0x08    : if (access_zero(CPUreg.reg[rs]) == true) {return 10;} if (invalid_instruction(CPUreg.reg[rs])) {exit (3);} PC.load_PC(CPUreg.reg[rs], true);if (access_zero(CPUreg.reg[rs]) == true) {return 10;} return 0; //jr
            case 0x09    : CPUreg.reg[31] = PC.get_PC(); if (access_zero(CPUreg.reg[rs]) == true) PC.load_PC(CPUreg.reg[rs], true); if (access_zero(CPUreg.reg[rs]) == true) {return 10;}return 0; //jalr
            default: exit (3);
    }
    }
}


#endif