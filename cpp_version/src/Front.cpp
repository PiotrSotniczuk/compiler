#include "../bnfc/Skeleton.H"
#include <iostream>
#include "Front.h"

using namespace std;

void MyVisitor::compile(Program *p){
    p->accept(this);
};
void MyVisitor::visitNoInit(NoInit *no_init){
    cout << "No initi\n";
    visitIdent(no_init->ident_);
};