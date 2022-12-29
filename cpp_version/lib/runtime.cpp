#include <iostream>

using namespace std;

void printInt(int x) { cout << x; }

void printString(string s) { cout << s; }

void error() {
  cerr << "runtime error\n";
  exit(1);
}

int readInt() {
  int x;
  cin >> x;
  return x;
}

string readString() {
  string str;
  cin >> str;
  return str;
}
