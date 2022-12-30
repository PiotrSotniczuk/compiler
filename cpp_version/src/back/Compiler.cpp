#include "Compiler.h"

void Compiler::visitString(String x){
    int size = this->local_const.size();
    this->local_const.emplace(make_pair(x, size));
}

void Compiler::visitFnDef(FnDef *fn_def){
    string fun_content = fn_def->ident_ + ":\n";
    fun_content += "\tpushq rbp\n\tmovq rbp, rsp\n";

    this->act_content = "";

    fn_def->type_->accept(this);
    visitIdent(fn_def->ident_);
    fn_def->listarg_->accept(this);
    fn_def->block_->accept(this);

    Void* type_void = dynamic_cast<Void*>(fn_def->type_);
    if (type_void){
        this->act_content += "\tleave\n\tret\n";
    }

    this->content += fun_content + this->act_content;
}