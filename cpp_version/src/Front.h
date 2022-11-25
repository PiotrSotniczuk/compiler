#ifndef FRONT_HEADER
#define FRONT_HEADER
/* You might want to change the above name. */

#include "../bnfc/Skeleton.H"
#include <set>

using namespace std;

class MyVisitor : public Skeleton {
    public:
        MyVisitor(void){
            fun_args = set<string>();
        };
        void compile(Program *p);
    private:
        virtual void visitListArg(ListArg *list_arg);
        virtual void visitAr(Ar *ar);
        set<string> fun_args;
};

#endif
