#include "Compiler.h"
#include <vector>

string add_t_n(vector<string> instr){
    string ret = "";
    for (string s: instr){
        ret += "\t" + s + "\n";
    }
    return ret;
}

void Compiler::visitString(String x){
    int size = this->local_const.size();
    this->local_const.emplace(make_pair(x, size));
}

void Compiler::visitFnDef(FnDef *fn_def){
    string fun_content = fn_def->ident_ + ":\n";
    fun_content += add_t_n(vector<string>({"push ebp", "mov ebp, esp"}));

    this->act_content = "";

    fn_def->type_->accept(this);
    visitIdent(fn_def->ident_);
    fn_def->listarg_->accept(this);
    fn_def->block_->accept(this);

    Void* type_void = dynamic_cast<Void*>(fn_def->type_);
    if (type_void){
        this->act_content += add_t_n(vector<string>({"leave", "ret"}));
    }

    this->content += fun_content + this->act_content;
}

void Compiler::visitVRet(VRet *v_ret){
    this->act_content += add_t_n(vector<string>({"leave", "ret"}));
}

void Compiler::visitRet(Ret *ret){
    ret->expr_->accept(this);
    this->act_content += add_t_n(vector<string>({
       "pop eax", "leave", "ret"
    }));
}

void Compiler::visitELitInt(ELitInt *e_lit_int){
    visitInteger(e_lit_int->integer_);
    this->act_content += add_t_n(vector<string>({
        "push " + to_string(e_lit_int->integer_)
    }));
}

void Compiler::visitEMul(EMul *e_mul){
    e_mul->expr_1->accept(this);
    e_mul->mulop_->accept(this);
    e_mul->expr_2->accept(this);

    Times* type_times = dynamic_cast<Times*>(e_mul->mulop_);
    if(type_times){
        this->act_content += add_t_n(vector<string>({
            "pop eax", "pop ecx", "imul eax, ecx", "push eax"
        }));
        return;
    }

    this->act_content += add_t_n(vector<string>({
        "pop ecx", "pop eax", "cdq", "idiv ecx"
    }));

    Div* type_div = dynamic_cast<Div*>(e_mul->mulop_);
    if(type_div){
        this->act_content += add_t_n(vector<string>({
            "push eax"
        }));
    }else{
        this->act_content += add_t_n(vector<string>({
            "push edx"
        }));
    }
}

void Compiler::visitEAdd(EAdd *e_add){
    // TODO strings

    // push first expression "a" 
    e_add->expr_1->accept(this);

    e_add->addop_->accept(this);
    // push second "c"
    e_add->expr_2->accept(this);

    Plus* type_plus = dynamic_cast<Plus*>(e_add->addop_);
    string op = "";
    if(type_plus){
        // a = a + c
        op = "add eax, ecx";
    }else{
        op = "sub eax, ecx";
    }

    this->act_content += add_t_n(vector<string>({
        "pop ecx", "pop eax", op, "push eax"
    }));
}

void Compiler::visitNeg(Neg *neg){
    neg->expr_->accept(this);
    this->act_content += add_t_n(vector<string>({
        "pop eax", "neg eax", "push eax"
    }));
}