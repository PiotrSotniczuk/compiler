#include "Shared.h"
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <iterator>

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