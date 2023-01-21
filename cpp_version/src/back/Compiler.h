#ifndef COMPILER_HEADER
#define COMPILER_HEADER

#include "../../bnfc/Skeleton.H"
#include "../../bnfc/Absyn.H"
#include "../front/Find_def.h"
#include <map>
#include <list>

using namespace std;

// class to generate code from parse tree
class Compiler : public Skeleton {
    public:
        Compiler(map<string, pair<string, vector<string>>> funs, map<string, Klass> classes){
            this->full_code = "";
            this->act_code = "";
            this->local_const = map<string, int>();
            this->vars_offsets = list<map<string, pair<string, int>>>();
            this->label_count = 0;
            this->vars_size = 0;
            this->expr_type = "";
            this->funs = funs;
            this->classes = classes;
        };
        string full_code;
        // <var_name, place>
        map<string, int> local_const;
        map<string, Klass> classes;
    private:
        // functions definitions
        map<string, pair<string, vector<string>>> funs;
        int vars_size;
        string expr_type;
        // list of envs, map of <var_name, (type, offset on stack)>
        list<map<string, pair<string, int>>> vars_offsets;
        string act_code;
        int label_count;
        int get_l_count(){return label_count++;}
        // get var from envs
        pair<string, int> get_var(string var);
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
