#include <climits>
#include <iostream>
#include <cinttypes>
#include <cstdlib>
#include <exception>

#define MEM_SIZE 0x40000000
#define NULL_START 0x00000000
#define NULL_END 0x00000001
#define EXEC_START 0x4000000
#define EXEC_END 0x4400000
#define RW_START 0x8000000
#define RW_END 0x9000000
#define GET_C_START 0xC000000
#define GET_C_END 0xC000001
#define PUT_C_START 0xC000001
#define PUT_C_END 0xC000002


bool mem_range_error(uint32_t access_addr) // Error code -11
{
    access_addr /= 4;

    if (access_addr >= MEM_SIZE) return true;
    return false;
}

bool write_check(uint32_t access_addr)
{
    access_addr /= 4;

    if ((access_addr >= RW_START && access_addr < RW_END) || access_addr == PUT_C_START) return false;
    return true;
}


bool invalid_instruction(uint32_t access_addr) // Error code -12
{
    access_addr /= 4;

    if (access_addr < EXEC_START || access_addr >= EXEC_END) return true;
    return false;
}


bool addition_exception(uint32_t op1, uint32_t op2) // Error code -10
{
    try 
    {
        op1 = op1 + op2;
    }
    
    catch (const std::overflow_error &a)
    {
        return true;
    }
    return false;
}


bool subtraction_exception(uint32_t op1, uint32_t op2)
{
    try 
    {
        op1 = op1 - op2;
    }

    catch (const std::underflow_error &a)
    {
        return true;
    }
    return false;   
}


bool division_error(uint32_t op1, uint32_t op2) // Not an exception but yields unpredictable result
{
    if (op2 == 0) return true;
    return false;
}


bool write_to_zero(uint32_t access_addr)
{
    if (access_addr == 0) return true;

    access_addr /= 4;

    if (access_addr < 1) return true;

    return false;
}


bool access_zero(uint32_t access_addr)
{
    uint32_t loc = access_addr/4;

    if (loc == 0)
    {
        return true;
    }

    return false;
}