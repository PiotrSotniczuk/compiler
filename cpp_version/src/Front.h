#ifndef FRONT_HEADER
#define FRONT_HEADER
/* You might want to change the above name. */

#include "../bnfc/Skeleton.H"

class MyVisitor : public Skeleton {
    public:
        void compile(Program *p);
    private:
        virtual void visitNoInit(NoInit *no_init);
};

#endif
