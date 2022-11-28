#ifndef FIND_DEF_HEADER
#define FIND_DEF_HEADER
/* You might want to change the above name. */

#include "../bnfc/Skeleton.H"
#include <map>
#include <set>
#include "../bnfc/Absyn.H"

using namespace std;

class FindDef : public Skeleton {
    public:
        map<string, pair<string, vector<string>>> funs;
        int main_line;
        FindDef(void){
            funs = map<string, pair<string, vector<string>>>();
            funs.emplace(make_pair( "printInt", make_pair("void", vector<string>({"int"}))));
            funs.emplace(make_pair( "printString", make_pair("void", vector<string>({"string"}))));
            funs.emplace(make_pair( "error", make_pair("void", vector<string>())));
            funs.emplace(make_pair( "readInt", make_pair("int", vector<string>())));
            funs.emplace(make_pair( "readString", make_pair("string", vector<string>())));
            arg_types = vector<string>();
            fun_args = set<string>();
            main_line = 0;
        };
    private:
        vector<string> arg_types;
        set<string> fun_args;
        virtual void visitFnDef(FnDef *fn_def);
        virtual void visitListArg(ListArg *list_arg);
        virtual void visitAr(Ar *ar);
};

class CheckReturn : public Skeleton {
    public:
        
    private:
        bool there_is_return;
        virtual void visitFnDef(FnDef *fn_def);
        virtual void visitListStmt(ListStmt *list_stmt);
        virtual void visitRet(Ret *ret);
        virtual void visitCond(Cond *cond);
        virtual void visitCondElse(CondElse *cond_else); 
};

#endif
