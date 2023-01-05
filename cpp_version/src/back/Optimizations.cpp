#include<string>
#include <sstream>
#include "../Shared.h"
#include <vector>
#include <iostream>

using namespace std;

string remove_redundant_lines(string in_code){
    string out_code = "";
    istringstream f(in_code);

    string line = "";
    string last_line = "";    
    while (std::getline(f, line)){
        vector<string> split_line = split_str(line, ' ');
        vector<string> split_last_line = split_str(last_line, ' ');
        
        // add and sub with 0 doesn't make sense
        if(split_last_line.size() > 0  && 
            (split_last_line[0] == "\tsub" || split_last_line[0] == "\tadd") &&
            split_last_line[2] == "0"){
            last_line = line;
            continue;
        }

        // pushing and popping same register
        if(split_last_line.size() > 0  && split_last_line[0] == "\tpush" &&
            split_line.size() > 0 && split_line[0] == "\tpop" &&
            split_last_line[1] == split_line[1]){
            last_line = "";
            continue;
        }

        // xor is faster than pushing and popping 0 and only one instruction
        if(last_line == "\tpush 0" && split_line.size() > 0 && split_line[0] == "\tpop"){
            last_line = "\txor " + split_line[1] + ", " + split_line[1];
            continue;
        }

        // instead of pushing and poping just move
        if(split_last_line.size() > 0 && split_line.size() > 0 &&
            split_last_line[0] == "\tpush" && split_line[0] == "\tpop"){    
            last_line = "\tmov " + split_line[1] + ", " + split_last_line[1];
            continue;
        }

        if(last_line != ""){
            out_code += (last_line + "\n");
        }
        last_line = line;
    }
    out_code += (last_line + "\n");
    return out_code;
}