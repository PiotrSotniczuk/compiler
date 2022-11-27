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
        virtual void visitBlk(Blk *blk);
        virtual void visitListItem(ListItem *list_item);
        virtual void visitNoInit(NoInit *no_init);
        virtual void visitInit(Init *init);

};

#endif
