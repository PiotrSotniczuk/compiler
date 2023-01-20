#ifndef FRONT_HEADER
#define FRONT_HEADER
/* You might want to change the above name. */

#include "../../bnfc/Skeleton.H"
#include <list>
#include <map>
#include "Find_def.h"

using namespace std;

class TypeChecker : public Skeleton {
    public:
        TypeChecker(map<string, pair<string, vector<string>>> funs, map<string, Klass> classes){
            this->funs = funs;
            this->classes = classes;
            vars = list<map<string, string>>();
        }
        // env, var,type
        list<map<string, string>> vars;
    private:
        // name, ret_type, args
        map<string, Klass> classes;
        map<string, pair<string, vector<string>>> funs;
        string last_ident;
        string expr_type;
        string act_fun_type;
        string getVarType(string var, int line_number);
        virtual void visitBlk(Blk *blk);
        virtual void visitBlkFun(Blk *blk);
        virtual void visitListItem(ListItem *list_item);
        virtual void visitNoInit(NoInit *no_init);
        virtual void visitInit(Init *init);
        virtual void visitAss(Ass *ass);
        virtual void visitIncr(Incr *incr);
        virtual void visitDecr(Decr *decr);
        virtual void visitFnDef(FnDef *fn_def);
        virtual void visitRet(Ret *ret);
        virtual void visitVRet(VRet *v_ret);
        virtual void visitCond(Cond *cond);
        virtual void visitCondElse(CondElse *cond_else);
        virtual void visitWhile(While *while_);
        virtual void visitEVar(EVar *p);
        virtual void visitELitInt(ELitInt *p);
        virtual void visitELitTrue(ELitTrue *p);
        virtual void visitELitFalse(ELitFalse *p);
        virtual void visitListExpr(ListExpr *list_expr, vector<string> args, int line);
        virtual void visitEApp(EApp *p);
        virtual void visitEString(EString *p);
        virtual void visitNeg(Neg *p);
        virtual void visitNot(Not *p);
        virtual void visitEMul(EMul *p);
        virtual void visitEAdd(EAdd *p);
        virtual void visitERel(ERel *p);
        virtual void visitEAnd(EAnd *p);
        virtual void visitEOr(EOr *p);
        virtual void visitDecl(Decl *decl);
        virtual void visitClsDef(ClsDef *cls);
        virtual void visitClsFun(ClsFun *cls_fun);
        virtual void visitEClsAt(EClsAt *e_cls_at);
        virtual void visitEClsApp(EClsApp *e_cls_app);
        virtual void visitNewCls(NewCls *new_cls);
        virtual void visitENull(ENull *e_null);
        virtual void visitClsType(ClsType *cls_type);
        virtual bool type_compatible(string l_type, string r_type);
        virtual void visitAtrAss(AtrAss *atr_ass);
        virtual void visitClsAtr(ClsAtr *cls_atr);
};

#endif
