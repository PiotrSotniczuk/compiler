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
        if(last_line == "\tsub esp, 0" || last_line == "\tadd esp, 0"){
            last_line = line;
            continue;
        }

        if(last_line == "\tpush eax" && line == "\tpop eax"){
            last_line = "";
            continue;
        }

        if(last_line == "\tpush 0" && line == "\tpop eax"){
            last_line = "\txor eax, eax";
            continue;
        }

        vector<string> split_line = split_str(line, ' ');
        vector<string> split_last_line = split_str(last_line, ' ');
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