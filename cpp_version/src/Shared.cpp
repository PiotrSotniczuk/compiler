#include "Shared.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <regex>

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