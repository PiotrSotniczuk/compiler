#ifndef COMPILER_HEADER
#define COMPILER_HEADER
/* You might want to change the above name. */

#include "../../bnfc/Skeleton.H"
#include "../../bnfc/Absyn.H"
#include <map>
#include <list>

using namespace std;

class Compiler : public Skeleton {
    public:
        Compiler(void){
            this->content = "";
            this->act_content = "";
            this->local_const = map<string, int>();
            this->vars_offsets = list<map<string, int>>();
            this->label_count = 0;
            this->vars_size = 0;
        };
        string content;
        map<string, int> local_const;
    private:
        int vars_size;
        list<map<string, int>> vars_offsets;
        string act_content;
        int label_count;
        int get_l_count(){return label_count++;}
        int get_var_offset(string var);
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
        virtual void visitCond(Cond *cond);
        virtual void visitCondElse(CondElse *cond_else);
        virtual void visitWhile(While *while_);
        virtual void visitERel(ERel *e_rel);
        virtual void visitBlk(Blk *blk);
        virtual void visitListArg(ListArg *list_arg);
        virtual void visitNoInit(NoInit *no_init);
        virtual void visitInit(Init *init);
        virtual void visitAss(Ass *ass);
        virtual void visitIncr(Incr *incr);
        virtual void visitDecr(Decr *decr);
        virtual void visitEVar(EVar *e_var);
        // virtual void ;
        // virtual void ;
        // virtual void ;
        // virtual void ;

};

#endif
