#include "Shared.h"
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <iterator>
#include "../bnfc/Parser.H"
#include "../bnfc/Absyn.H"
#include "front/Find_def.h"
#include "back/Compiler.h"
#include "back/Optimizations.h"
#include <filesystem>
#include <fstream>

using namespace std;

void go_error(int line, string msg){
	cerr << "ERROR\nline: " << to_string(line) << "\n" <<  msg << "\n";
	exit(1);
}

// https://stackoverflow.com/questions/42711201/output-a-c-string-including-all-escape-characters
string sanitize(string s){
	string out = "";
    for ( auto ch : s ){
      switch (ch){
         case '\'':
            out +=  "\\'";
            break;
         case '\"':
            out +=  "\\\"";
            break;
         case '\?':
            out +=  "\\?";
            break;
         case '\\':
            out +=  "\\\\";
            break;
         case '\a':
            out +=  "\\a";
            break;
         case '\b':
            out +=  "\\b";
            break;
         case '\f':
            out +=  "\\f";
            break;
         case '\n':
            out +=  "\\n";
            break;
         case '\r':
            out +=  "\\r";
            break;
         case '\t':
            out +=  "\\t";
            break;
         case '\v':
            out +=  "\\v";
            break;
         default:
            out +=  ch;
      }
    }
    return out;
}
 
// https://www.techiedelight.com/check-if-a-string-ends-with-another-string-in-cpp/
bool endsWith(std::string const &str, std::string const &suffix) {
    if (str.length() < suffix.length()) {
        return false;
    }
    return str.compare(str.length() - suffix.length(), suffix.length(), suffix) == 0;
}

bool startsWith(std::string const &str, std::string const &prefix) {
    if (str.length() < prefix.length()) {
        return false;
    }
    return str.compare(0, prefix.length(), prefix) == 0;
}

template <typename Out>
void split(const std::string &s, char delim, Out result) {
    std::istringstream iss(s);
    std::string item;
    while (std::getline(iss, item, delim)) {
        *result++ = item;
    }
}

// https://stackoverflow.com/questions/236129/how-do-i-iterate-over-the-words-of-a-string
vector<string> split_str(const string &s, char delim) {
    std::vector<string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}

namespace fs = std::filesystem;
string HEADER = "\t.intel_syntax noprefix\n" + string("\t.globl main\n");

// generate code for vtab
string get_vtabs(map<string, Klass> classes){
	string ret = "";
   	for (auto const& [name, klass] : classes){
    	ret += ".vtab_" + name + ":\n";
		vector<string> in_order(klass.vtab.size());
		for (auto const& [fun_name, tup] : klass.vtab){
			int offset = get<3>(tup);
			string cls_name = get<0>(tup);
			in_order[offset] = cls_name + "_" + fun_name;
		}
		for (auto const& elem : in_order){
			ret += "\t.long _" + elem + "\n";
		}
  	}
	return ret;
}

void backend(Program *parse_tree, char *filepath, char **argv){
   // get function definitions
	FindDef *fdef = new FindDef();
	fdef->run(parse_tree);

	// generate x86 code
	Compiler *comp = new Compiler(fdef->funs, fdef->classes);
	comp->run(parse_tree);
	string content = comp->full_code;
	
	// add string literals to Local Constants at the begining
	string local_const_string = "";
	local_const_string += ".LC_empty_str:\n\t.string \"\"\n";
	for(auto pair: comp->local_const){
		string safe_str = sanitize(pair.first);
		local_const_string += ".LC" + to_string(pair.second) + ":\n\t.string \"" + safe_str + "\"\n";
	}
	string vtabs = get_vtabs(fdef->classes);

	string optimized = remove_redundant_lines(HEADER + local_const_string + vtabs + content);

	// generating assembly file
    string asm_file = fs::path(filepath).replace_extension("s");
	ofstream out(asm_file);
    out << optimized;
    out.close();

    asm_file = "\"" + asm_file + "\"";
    string target_file = "\"" + string(fs::path(filepath).replace_extension("")) + "\"";

	string path_to_root =  string(fs::path(argv[0]).remove_filename());
	string path_to_runtime = "\"" + path_to_root + "lib/runtime.o\"";

	// generating binary executable
	string to_binary = "i686-linux-gnu-gcc " + path_to_runtime + " " + asm_file + " -no-pie -m32 -masm=intel -o " + target_file;
	system(to_binary.c_str());
}
