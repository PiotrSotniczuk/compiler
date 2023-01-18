#include <iostream>
#include "../Shared.h"
#include "../../bnfc/Absyn.H"
#include "../../bnfc/Parser.H"

int main(int argc, char **argv){
	if (argc != 2){
		cerr << "Bad number of args you can only pass the path to file\n";
		exit(1);
	}
	
 	char *filepath = argv[1];
	FILE *input = fopen(filepath, "r");
	if (!input){
		cerr << "Can't open file!\n";
		exit(1);
	}

	// parse input file
	Program *parse_tree = pProgram(input);
	if (!parse_tree){
		exit(1);
	}
	backend(parse_tree, filepath, argv);
	return 0;
}
