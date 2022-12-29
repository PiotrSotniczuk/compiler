#include <iostream>
#include <fstream>
#include "../../bnfc/Parser.H"
#include "../../bnfc/Absyn.H"
#include "Type_checker.h"
#include "Find_def.h"
#include "../Shared.h"

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

	FindDef *fdef = new FindDef();
	fdef->run(parse_tree);

	auto main_fun = fdef->funs.find("main");
	if(main_fun == fdef->funs.end()){
		go_error(0, "There is no 'main' function");
	}

	if(main_fun->second.first != "int"){
		go_error(fdef->main_line, "'main' function doesn't return 'int'");
	}

	if(main_fun->second.second.size() > 0){
		go_error(fdef->main_line, "'main' function shouldn't have arguments");
	}

	TypeChecker *myV = new TypeChecker(fdef->funs);
    myV->run(parse_tree);

	CheckReturn *ret = new CheckReturn();
	ret->run(parse_tree);
	

	


	cerr << "OK\n";

	return 0;
}
