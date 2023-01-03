#ifndef SHARED_HEADER
#define SHARED_HEADER
/* You might want to change the above name. */

#include "../bnfc/Skeleton.H"
#include <map>
#include "../bnfc/Absyn.H"

using namespace std;
void go_error(int line, string msg);
string sanitize(string s);

#endif
