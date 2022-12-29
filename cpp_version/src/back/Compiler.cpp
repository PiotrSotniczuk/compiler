#include <iostream>
#include <fstream>
#include "../../bnfc/Parser.H"
#include "../../bnfc/Absyn.H"
#include "../Shared.h"
#include <filesystem>

namespace fs = std::filesystem;
using namespace std;


string HEADER = ".data\n" + string(".text\n") + ".globl main\n";


int main(int argc, char **argv)
{
	if (argc != 2) {
		cerr << "Bad number of args you can only pass the path to file\n";
		exit(1);
	}
	
 	char *filepath = argv[1];
	FILE *input = fopen(filepath, "r");
	if (!input) {
		cerr << "Can't open file!\n";
		exit(1);
	}

	Program *parse_tree = pProgram(input);
	if (!parse_tree) {
		exit(1);
	}

    string main_fun = "main:\n";





    
	

    string asm_file = fs::path(filepath).replace_extension("s");
	ofstream out(asm_file);

    out << (HEADER + main_fun);
    out.close();

    asm_file = "\"" + asm_file + "\"";
    string target_file = "\"" + string(fs::path(filepath).replace_extension("")) + "\"";
    

	string to_binary = "gcc lib/runtime.o " + asm_file + " -no-pie -masm=intel -o " + target_file;
    cout << to_binary;
	system(to_binary.c_str());

	return 0;
}
