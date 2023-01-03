#ifndef SHARED_HEADER
#define SHARED_HEADER

#include<string>
#include<vector>

using namespace std;

void go_error(int line, string msg);
string sanitize(string s);
bool endsWith(std::string const &str, std::string const &suffix);
bool startsWith(std::string const &str, std::string const &prefix);
vector<string> split_str(const string &s, char delim);

#endif
