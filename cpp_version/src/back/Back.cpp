#include <iostream>
#include <fstream>
#include "../../bnfc/Parser.H"
#include "../../bnfc/Absyn.H"
#include "../Shared.h"
#include "../front/Find_def.h"
#include <filesystem>
#include "Compiler.h"
#include <string>

namespace fs = std::filesystem;
using namespace std;


string HEADER = "\t.intel_syntax noprefix\n" + string("\t.globl main\n");


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

	FindDef *fdef = new FindDef();
	fdef->run(parse_tree);

	Compiler *comp = new Compiler(fdef->funs);
	comp->run(parse_tree);
	string content = comp->content;
	
	string local_const_string = "";
	local_const_string += ".LC_empty_str:\n\t.string \"\"\n";
	for(auto pair: comp->local_const){
		local_const_string += ".LC" + to_string(pair.second) + ":\n\t.string \"" + pair.first + "\"\n";
	}



    string asm_file = fs::path(filepath).replace_extension("s");
	ofstream out(asm_file);

    out << (HEADER + local_const_string + content);
    out.close();

    asm_file = "\"" + asm_file + "\"";
    string target_file = "\"" + string(fs::path(filepath).replace_extension("")) + "\"";
    

	string to_binary = "gcc lib/runtime.o " + asm_file + " -no-pie -m32 -masm=intel -o " + target_file;
	system(to_binary.c_str());

	return 0;
}
