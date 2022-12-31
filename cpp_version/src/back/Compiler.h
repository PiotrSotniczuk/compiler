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
            this->lazy_count = 0;
        };
        string content;
        map<string, int> local_const;
    private:
        string act_content;
        int lazy_count;
        virtual void visitString(String x);
        virtual void visitFnDef(FnDef *fn_def);
        virtual void visitRet(Ret *p);
        virtual void visitVRet(VRet *p);
        virtual void visitELitInt(ELitInt *e_lit_int);
        virtual void visitEAdd(EAdd *e_add);
        virtual void visitNeg(Neg *neg);
        virtual void visitEMul(EMul *e_mul);
        virtual void visitEApp(EApp *e_app);
        virtual void visitListExpr(ListExpr *list_expr);
        virtual void visitEString(EString *e_string);
        virtual void visitELitTrue(ELitTrue *e_lit_true);
        virtual void visitELitFalse(ELitFalse *e_lit_false);
        virtual void visitNot(Not *not_);
        virtual void visitEAnd(EAnd *e_and);
        virtual void visitEOr(EOr *e_or);
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
