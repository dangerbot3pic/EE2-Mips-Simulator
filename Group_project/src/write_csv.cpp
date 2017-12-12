#include <iostream>
#include <fstream>

using namespace std;


void write_csv(string id, string instruction, bool status, string author, string argument);


int main(int argc, char **argv)
{
    string pass = argv[1];
    string test_name =argv[2];
    string name = argv[3];
    string ref = argv[4];
    string type = argv[5];
    string argument;

    if(type =="-1") argument = "No errors";
    else if(type == "-10") argument = "Arithmetic errror";
    else if(type == "-11") argument =  "Memory exception";
    else if(type == "-12") argument =  "Invalid instruction";
    else if(type == "-20") argument =  "Internal error";
    else argument = "Unkown error";
        
    bool passed = false;

    if (pass == "pass") passed = true;

    write_csv(ref, test_name, passed, name, argument);

    return 0;
}


void write_csv(string id, string instruction, bool status, string author, string argument)
{
  if (status)
  {
    cout<<id<<", "<<instruction<<", "<<"Pass"<<", "<<author<<", "<<argument<<", "<<endl; 
    return;
  }

  cout<<id<<", "<<instruction<<", "<<"Fail"<<", "<<author<<", "<<argument<<", "<<endl; 
}