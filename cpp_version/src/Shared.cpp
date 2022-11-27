#include "Shared.h"
#include <iostream>

using namespace std;

void go_error(int line, string msg){
	cerr << "ERROR\nline: " << to_string(line) << "\n" <<  msg << "\n";
	exit(1);
}