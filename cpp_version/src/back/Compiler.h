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
};

#endif
