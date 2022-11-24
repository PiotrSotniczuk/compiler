#include <iostream>
#include <string.h>
#include <fstream>
#include <string>
#include "../bnfc/Parser.H"
#include "../bnfc/Absyn.H"
#include "Front.h"
#include <map>
#include <filesystem>
#include <cassert>

namespace fs = std::filesystem;
using namespace std;

// class MyVisitor : public Skeleton {
//     public:
//         void compile(Program *p){
//             p->accept(this);
//         }

//     private:        
//         void visitNoInit(NoInit *no_init){
//             cout << "No initi\n";
//             visitIdent(no_init->ident_);
//         }
// };


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

    cerr << "OK\n";

	MyVisitor *myV = new MyVisitor();

    myV->compile(parse_tree);

	return 0;
}
