#include <iostream>
#include <string>
#include <fstream>
#include <cinttypes>


// My headers
#include "read_bin.hpp"
#include "memory.hpp"
#include "process.hpp"
#include "pc.hpp"


using namespace std;


int main(int argc, char **argv)
{
    //Initialise objects
    Memory RAM;
    ProgramCounter PC;

    std::string bin_name = argv[1];

    ifstream binfile;
    binfile.open(bin_name.c_str(), std::ios::binary | std::ios::in);

    if (!binfile.is_open()) std::cout<<"File not found"<<std::endl; exit (-25);

    if (read_bin(binfile, RAM) == -25) == -1) std::cout<<"Too large"<<std::endl; //Binary too large

    binfile.close();

    if (decoder(RAM, PC) == 2) //

}