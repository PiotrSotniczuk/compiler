#ifndef FRONT_HEADER
#define FRONT_HEADER
/* You might want to change the above name. */

#include "../bnfc/Skeleton.H"
#include <list>
#include <map>

using namespace std;

class TypeChecker : public Skeleton {
    public:
        TypeChecker(map<string, pair<string, vector<string>>> funs){
            this->funs = funs;
            vars = list<map<string, string>>();
        }
        list<map<string, string>> vars;
    private:
        
        map<string, pair<string, vector<string>>> funs;
        string last_ident;
        string expr_type;
        string act_fun_type;
        string getVarType(string);
        virtual void visitBlk(Blk *blk);
        virtual void visitListItem(ListItem *list_item);
        virtual void visitNoInit(NoInit *no_init);
        virtual void visitInit(Init *init);
        virtual void visitAss(Ass *ass);
        virtual void visitIncr(Incr *incr);
        virtual void visitDecr(Decr *decr);
        virtual void visitFnDef(FnDef *fn_def);
        virtual void visitRet(Ret *ret);
        virtual void visitCond(Cond *cond);
        virtual void visitCondElse(CondElse *cond_else);
        virtual void visitWhile(While *while_);
        // virtual void ;
        // virtual void ;

};

#endif
