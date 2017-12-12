#include <iostream>
#include "headers/mem.hpp"
#include "headers/registers.hpp"
#include "headers/pc.hpp"
#include <fstream>
#include "headers/error_check.hpp"
#include <cinttypes>
#include <exception>
#include <bitset>

//Error code -10 = 1
//-11 = 2
//-12 = 3
//-20 = 4


int mother(memory &mem, registers &CPUreg, program_counter &PC, uint32_t &return_val);
int read_file(memory &mem, std::fstream &infile);
int decode(memory &mem, registers &CPUreg,program_counter &PC, const  uint32_t instruction, uint32_t &return_val);
int i_type(memory &mem, registers &CPUreg, program_counter &PC, const uint32_t instruction, uint32_t &return_val);
int j_type(memory &mem, registers &CPUreg, program_counter &PC, const uint32_t instruction);
int r_type(registers &CPUreg, program_counter &PC, const uint32_t instruction, uint32_t &return_val);
uint32_t arithmetic_shift_right(uint32_t input,uint32_t shift_size);


int main(int argc, char *argv[])
{
    memory mem;
    registers CPUreg;
    program_counter PC;
    int result = 0; // Stores result of execution
    std::string tmp;
    uint32_t return_val;

    std::string filename = argv[1];

    std::fstream infile;

    infile.open(filename.c_str(), std::ios::binary | std::ios::in);

    if (!infile.is_open()) {exit(4);}

    if (read_file(mem, infile) == 3) exit(3); //If 3 then binary too large to store in instruction memory

    infile.close();

    while (result == 0)
    {
        result = mother(mem, CPUreg, PC, return_val);
        CPUreg.reg[0] = 0;
        PC.increment();
    }

    return return_val;//Return output
}


int mother(memory &mem, registers &CPUreg, program_counter &PC, uint32_t &return_val)
{
    uint32_t instruction = mem.get_instruction(PC.get_PC()); // Retrieve instruction from memory

    return (decode(mem, CPUreg, PC, instruction, return_val)); // Decode and execute
}


int read_file(memory &mem, std::fstream &infile)
{
    char c;
    uint32_t t = 0;
    uint32_t count = 0;

    while (!infile.eof())
    {
        for (int i = 0; i < 32; i++)
        {
            infile >> c;
            if (c == '1') t += (1 << (31-i));
            if (count > (0x4400000<<2)) exit(3);
            if (infile.eof()) return 0;
        }

        mem.store_instruction(count, t);
        t = 0;
        count += 4;
    }

    return 0;
}


int decode(memory &mem, registers &CPUreg,program_counter &PC, const uint32_t instruction, uint32_t &return_val)
{
    uint32_t opcode = instruction >> 26;

    if (instruction == 0) return 10;

    switch (opcode)
    {
        case 0b000000: return (r_type(CPUreg, PC, instruction, return_val)); // R - Type
        case 0b000010: return (j_type(mem, CPUreg, PC, instruction)); // J - Type
        case 0b000011: return (j_type(mem, CPUreg, PC, instruction)); // J - Type
        default: return (i_type(mem, CPUreg, PC, instruction, return_val)); // I - Type
    }
}


int i_type(memory &mem, registers &CPUreg, program_counter &PC, const unsigned int instruction, uint32_t &return_val)
{
    try
    {
    const unsigned short rs = (instruction >> 21) & 0b11111;
    const unsigned short rt = (instruction >> 16) & 0b11111;
    uint32_t IMM = instruction & 0XFFFF;
    const unsigned short opcode = instruction >> 26;

    //IMM = sign_extend(IMM);

    switch (opcode)
    {
        case 0x04: if (mem_range_error(PC.get_PC()+4+(IMM<<2))) {exit(2);} if (CPUreg.reg[rs] == CPUreg.reg[rt]) {PC.load_PC(PC.get_PC()+4+(IMM<<2), true);} return 0; // beq
        case 0x05: if (mem_range_error(PC.get_PC()+4+(IMM<<2))) {exit(2);} if (CPUreg.reg[rs] != CPUreg.reg[rt]) {PC.load_PC(PC.get_PC()+4+(IMM<<2), true);} return 0; // bne
        case 0x01: if (mem_range_error(PC.get_PC()+4+(IMM<<2))) {exit(2);} 
                    if (CPUreg.reg[rt] == 0x1) {if (CPUreg.reg[rs] >= 0) PC.load_PC(PC.get_PC()+4+(IMM<<2), true); return 0;} //bgez
                    else if (CPUreg.reg[rt] == 0b10001) {if (CPUreg.reg[rs] >= 0) {CPUreg.reg[31] = PC.get_PC(); PC.load_PC(PC.get_PC()+4+(IMM<<2), true);} {return 10;}return 0;} //bgezal
                    else if (CPUreg.reg[rt] == 0x0) {if (CPUreg.reg[rs] < 0) {PC.load_PC(PC.get_PC()+4+(IMM), true);} return 0;} //bltz
                    else if (CPUreg.reg[rt] == 0x10) {if (CPUreg.reg[rs] < 0) {CPUreg.reg[31] = PC.get_PC(); PC.load_PC(PC.get_PC()+4+(IMM<<2), true);} return 0;} //bltzal
                    exit(3);
        case 0x07: if (mem_range_error(PC.get_PC()+4+(IMM<<2))) {exit(2);}
                    if (CPUreg.reg[rs] > 0) {PC.load_PC(PC.get_PC()+4+(IMM<<2), true);} return 0; // bgtz
        case 0x06: if (mem_range_error(PC.get_PC()+4+(IMM<<2))) {exit(2);} 
                    if (CPUreg.reg[rs] <= 0) {PC.load_PC(PC.get_PC()+4+(IMM<<2), true);} return 0; // blez
        case 0x08: if (addition_exception(CPUreg.reg[rs], IMM)) {exit(1);} CPUreg.reg[rt] = CPUreg.reg[rs] + IMM; return_val = CPUreg.reg[rt]; return 0; //addi
        case 0x09: CPUreg.reg[rt] = (CPUreg.reg[rs]) + IMM; return_val = CPUreg.reg[rt];return 0; // addiu
        case 0x0A: if (CPUreg.reg[rs] < IMM) CPUreg.reg[rt] = 1; else CPUreg.reg[rt] = 0; return 0; // slti
        case 0x0B: if ((unsigned)CPUreg.reg[rs] < (unsigned)(IMM)) CPUreg.reg[rt] = 1;else CPUreg.reg[rt] = 0;return 0; // sltiu
        case 0x0C: CPUreg.reg[rs] = CPUreg.reg[rt] & IMM; return_val = CPUreg.reg[rs];return 0; // andi
        case 0x0D: CPUreg.reg[rs] = CPUreg.reg[rt] | IMM; return_val = CPUreg.reg[rs];return 0; // ori
        case 0x0F: CPUreg.reg[rs] = (IMM << 16); return_val = CPUreg.reg[rs]; return 0; // lui
        case 0x0E: CPUreg.reg[rs] = CPUreg.reg[rt] ^ IMM; return_val = CPUreg.reg[rs]; return 0;// xori
        case 0x23: CPUreg.reg[rs] = mem.load_word(IMM + CPUreg.reg[rt]); return_val = CPUreg.reg[rt];return 0; // lw
        case 0x24: CPUreg.reg[rs] = (unsigned)(mem.load_byte(IMM + CPUreg.reg[rt])); return_val = CPUreg.reg[rs];return 0; // lbu
        case 0x25: CPUreg.reg[rs] = (unsigned)(mem.load_hword(IMM + CPUreg.reg[rt])); return_val = CPUreg.reg[rs];return 0; // lhu
        case 0x28: if (write_to_zero(IMM + CPUreg.reg[rs])) {return 10;} if (write_check(IMM + CPUreg.reg[rs])) {exit(3);} mem.store_byte(IMM + CPUreg.reg[rs], CPUreg.reg[rt]); return 0; // sb
        case 0x29: if (write_to_zero(IMM + CPUreg.reg[rs])) {return 10;} if (write_check(IMM + CPUreg.reg[rs])) {exit(3);} mem.store_hword(IMM + CPUreg.reg[rs], CPUreg.reg[rt]); return 0; // sh
        case 0x2B: if (write_to_zero(IMM + CPUreg.reg[rs])) {return 10;} if (write_check(IMM + CPUreg.reg[rs])) {exit(3);} mem.store_word(IMM + CPUreg.reg[rs], CPUreg.reg[rt]); return 0; // sw
        case 0x22: CPUreg.reg[rs] = mem.load_word_left(IMM + CPUreg.reg[rt]); return_val = CPUreg.reg[rs];return 0;// lwl
        case 0x26: CPUreg.reg[rs] = mem.load_word_right(IMM + CPUreg.reg[rt]); return_val = CPUreg.reg[rs];return 0;// lwr
        case 0x20: CPUreg.reg[rs] = (mem.load_byte(IMM + CPUreg.reg[rt])); return_val = CPUreg.reg[rs];return 0; // lb
        case 0x21: CPUreg.reg[rs] = (mem.load_hword(IMM + CPUreg.reg[rt])); return_val = CPUreg.reg[rs];return 0; // lh
        default: exit(3);
    }
    }

    catch (const std::system_error &a)
    {
        exit(4);
    }
}


int j_type(memory &mem, registers &CPUreg, program_counter &PC, const uint32_t instruction)
{
    try 
    {
    uint32_t address = instruction & 0x3FFFFC;
    const unsigned short type = instruction >> 26;
    uint32_t tmp = PC.get_PC()+4;

    address = address << 2;
    tmp = tmp & 0xF0000000;
    address += tmp;

    if (mem_range_error(address)) exit(2);
    if (access_zero(address) == true) {return 10;}

    switch (type)
    {
        case 0x02:
        	PC.load_PC(address, true); // j
        	return 0;
        case 0x03:
        	CPUreg.reg[31] = PC.get_PC(); // jal
        	PC.load_PC(address, true);
        	return 0;
        default: exit(3);
    }
    }

    catch(const std::system_error &a)
    {
        exit(4);
    }
}


int r_type(registers &CPUreg, program_counter &PC, const uint32_t instruction, uint32_t &return_val)
{
    try {
    const unsigned short rs = (instruction >> 21) & 0b11111;
    const unsigned short rt = (instruction >> 16) & 0b11111;
    const unsigned short rd = (instruction >> 11) & 0b11111;
    const unsigned short shift_size = (instruction >> 6) & 0b11111;
    const unsigned short funct = instruction & 0b111111;

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

    catch (const std::system_error &a)
    {
        exit (4);
    }
}


uint32_t arithmetic_shift_right(uint32_t input,uint32_t shift_size){

    uint32_t temp;
    temp=input;
    temp=temp>>31;
    
    long sign_extend_size=4294967296;
    
    if(temp==1){
        
        for(int i=0;i<shift_size;i++)
        {
            input=input>>1;
            input=input+sign_extend_size;  
        }        
    }
    if(temp==0)
    {
        input=input>>shift_size;
    
    }

    return input;
}



