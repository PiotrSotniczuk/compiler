#ifndef COMPILER_HEADER
#define COMPILER_HEADER
/* You might want to change the above name. */

#include "../../bnfc/Skeleton.H"
#include "../../bnfc/Absyn.H"
#include <map>

using namespace std;

class Compiler : public Skeleton {
    public:
        Compiler(void){
            this->content = "";
            this->act_content ="";
            this->local_const = map<string, int>();
        };
        string content;
        string act_content;
        map<string, int> local_const;
    private:
        virtual void visitString(String x);
        virtual void visitFnDef(FnDef *fn_def);
        virtual void visitRet(Ret *p);
        virtual void visitVRet(VRet *p);
        virtual void visitELitInt(ELitInt *e_lit_int);
        virtual void visitEAdd(EAdd *e_add);
        virtual void visitNeg(Neg *neg);
        virtual void visitEMul(EMul *e_mul);
        // virtual void ;
        // virtual void ;
        // virtual void ;
        // virtual void ;
        // virtual void ;
        // virtual void ;
        // virtual void ;
        // virtual void ;
        // virtual void ;
        // virtual void ;
        // virtual void ;
        // virtual void ;
};

#endif
