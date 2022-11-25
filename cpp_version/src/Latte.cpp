#include <iostream>
#include <fstream>
#include "../bnfc/Parser.H"
#include "../bnfc/Absyn.H"
#include "Front.h"

using namespace std;


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

	MyVisitor *myV = new MyVisitor();
    myV->compile(parse_tree);

	cerr << "OK\n";

	return 0;
}
