#ifndef FRONT_HEADER
#define FRONT_HEADER
/* You might want to change the above name. */

#include "../bnfc/Skeleton.H"
#include <set>

using namespace std;

class MyVisitor : public Skeleton {
    public:
        
        void compile(Program *p);
    private:
        
};

#endif
