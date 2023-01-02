#include "Compiler.h"
#include <vector>
#include <cassert>

string add_t_n(vector<string> instr){
    string ret = "";
    for (string s: instr){
        char first = s[0];
        if(first != '_'){
            ret += "\t";
        }
        ret += s + "\n";
    }
    return ret;
}

void Compiler::visitString(String x){
    int size = this->local_const.size();
    this->local_const.emplace(make_pair(x, size));
}

void Compiler::visitFnDef(FnDef *fn_def){
    string fun_prefix = fn_def->ident_ + ":\n";
    fun_prefix += add_t_n(vector<string>({"push ebp", "mov ebp, esp"}));

    this->act_content = "";
    this->vars_size = 0;

    fn_def->type_->accept(this);
    visitIdent(fn_def->ident_);


    this->vars_offsets.push_front(map<string, pair<string, int>>());
    fn_def->listarg_->accept(this);

    Blk* blk = dynamic_cast<Blk*>(fn_def->block_);
    blk->liststmt_->accept(this);
    this->vars_offsets.pop_front();

    fun_prefix += add_t_n(vector<string>({"sub esp, " + to_string(this->vars_size*4)}));

    // add return if type void
    Void* type_void = dynamic_cast<Void*>(fn_def->type_);
    if (type_void){
        this->act_content += add_t_n(vector<string>({"leave", "ret"}));
    }

    this->content += fun_prefix + this->act_content;
}

void Compiler::visitVRet(__attribute__((unused)) VRet *v_ret){
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
    this->expr_type = "int";
}

void Compiler::visitEMul(EMul *e_mul){
    e_mul->expr_1->accept(this);
    e_mul->mulop_->accept(this);
    e_mul->expr_2->accept(this);
    this->expr_type = "int";

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
    // push first expression "a" 
    e_add->expr_1->accept(this);

    e_add->addop_->accept(this);
    // push second "c"
    e_add->expr_2->accept(this);

    if(this->expr_type == "string"){
        this->act_content += add_t_n(vector<string>({
            "pop ecx", "pop eax", "push ecx", "push eax", "call __concat", "add esp, 8", "push eax"
        }));
        return;
    }

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
    this->expr_type = "boolean";
}

void Compiler::visitEApp(EApp *e_app){
    visitIdent(e_app->ident_);
    e_app->listexpr_->accept(this);
    int size = e_app->listexpr_->size();
    this->act_content += add_t_n(vector<string>({
        "call " + e_app->ident_, "add esp, " + to_string(size * 4), "push eax"
    }));
    this->expr_type = this->funs.find(e_app->ident_)->second.first;
}

void Compiler::visitListExpr(ListExpr *list_expr){
    // do it from last to first
    auto it = list_expr->rbegin();
    while (it != list_expr->rend()){
        (*it)->accept(this);
        it++;
    }
}

void Compiler::visitEString(EString *e_string){
    this->visitString(e_string->string_);
    int index = this->local_const.find(e_string->string_)->second;
    this->act_content += add_t_n(vector<string>({
        "lea eax, .LC" + to_string(index), "push eax"
    }));
    this->expr_type = "string";
}

void Compiler::visitELitTrue(__attribute__((unused)) ELitTrue *e_lit_true){
    this->act_content += add_t_n(vector<string>({
        "push 1"
    }));
    this->expr_type = "boolean";
}

void Compiler::visitELitFalse(__attribute__((unused)) ELitFalse *e_lit_false){
    this->act_content += add_t_n(vector<string>({
        "push 0"
    }));
    this->expr_type = "boolean";
}

void Compiler::visitNot(Not *not_){
    not_->expr_->accept(this);
    this->act_content += add_t_n(vector<string>({
        "pop eax", "add eax, 1", "and eax, 1", "push eax"
    }));
    this->expr_type = "boolean";
}

void Compiler::visitEAnd(EAnd *e_and){
    int c = this->get_l_count();
    string lazy = "_lazy" + to_string(c);

    // lazy evaluation
    e_and->expr_1->accept(this);
    this->act_content += add_t_n(vector<string>({
        "pop eax", "test eax, eax", "jz " + lazy
    }));

    e_and->expr_2->accept(this);
    this->act_content += add_t_n(vector<string>({
        "pop eax", lazy+":", "push eax"
    }));
    this->expr_type = "boolean";
}

void Compiler::visitEOr(EOr *e_or){
    int c = this->get_l_count();
    string lazy = "_lazy" + to_string(c);

    e_or->expr_1->accept(this);
    this->act_content += add_t_n(vector<string>({
        "pop eax", "test eax, eax", "jnz " + lazy
    }));

    e_or->expr_2->accept(this);
    this->act_content += add_t_n(vector<string>({
        "pop eax", lazy+":", "push eax"
    }));
    this->expr_type = "boolean";
}

void Compiler::visitCond(Cond *cond){
    int c = this->get_l_count();
    string if_cond = "_if" + to_string(c);
    cond->expr_->accept(this);
    this->act_content += add_t_n(vector<string>({
        "pop eax", "test eax, eax", "jz " + if_cond
    }));

    cond->stmt_->accept(this);
    this->act_content += add_t_n(vector<string>({
        if_cond + ":"
    }));
}

void Compiler::visitCondElse(CondElse *cond_else){
    int c = this->get_l_count();
    string if_else = "_if_else" + to_string(c);
    string end = "_end_if" + to_string(c);

    cond_else->expr_->accept(this);
    this->act_content += add_t_n(vector<string>({
        "pop eax", "test eax, eax", "jz " + if_else
    }));

    cond_else->stmt_1->accept(this);
    this->act_content += add_t_n(vector<string>({
        "jmp " + end, if_else + ":"
    }));

    cond_else->stmt_2->accept(this);
    this->act_content += add_t_n(vector<string>({
        end + ":"
    }));
}

void Compiler::visitWhile(While *while_){
    int c = this->get_l_count();
    string start = "_while" + to_string(c);
    string end = "_end_while" + to_string(c);

    this->act_content += add_t_n(vector<string>({
        start + ":"
    }));
    while_->expr_->accept(this);
    this->act_content += add_t_n(vector<string>({
        "pop eax", "test eax, eax", "jz " + end
    }));

    while_->stmt_->accept(this);
    this->act_content += add_t_n(vector<string>({
        "jmp " + start, end + ":"
    }));
}

void Compiler::visitERel(ERel *e_rel){
    e_rel->expr_1->accept(this);
    e_rel->relop_->accept(this);
    e_rel->expr_2->accept(this);

    string t = this->expr_type;
    this->expr_type = "boolean";

    if(t == "string"){
        if(dynamic_cast<EQU*>(e_rel->relop_)){
            this->act_content += add_t_n(vector<string>({
                "call __compare_str", "add esp, 8", "push eax"
            }));
        }else{
            this->act_content += add_t_n(vector<string>({
                "call __compare_str", "add esp, 8", "add eax, 1", "and eax, 1", "push eax"
            }));
        }
        return;
    }

    string op = "";
    if(dynamic_cast<LTH*>(e_rel->relop_)){op = "l";}
    if(dynamic_cast<LE*>(e_rel->relop_)){op = "le";}
    if(dynamic_cast<GTH*>(e_rel->relop_)){op = "g";}
    if(dynamic_cast<GE*>(e_rel->relop_)){op = "ge";}
    if(dynamic_cast<EQU*>(e_rel->relop_)){op = "e";}
    if(dynamic_cast<NE*>(e_rel->relop_)){op = "ne";}

    this->act_content += add_t_n(vector<string>({
        "pop ecx", "pop eax", "xor edx, edx", "cmp eax, ecx",
        "set" + op + " dl", "push edx"
    }));
}

void Compiler::visitBlk(Blk *blk){
    this->vars_offsets.push_front(map<string, pair<string, int>>());
    blk->liststmt_->accept(this);
    this->vars_offsets.pop_front();
}

void Compiler::visitListArg(ListArg *list_arg){
    // add arguments to vars_offsets (above ret in stack)
    int offset = 2*4;
    for (ListArg::iterator i = list_arg->begin() ; i != list_arg->end() ; ++i){
        (*i)->accept(this);
        string t = this->last_type;
        Ar* ar = dynamic_cast<Ar*>(*i);
        assert(ar);
        this->vars_offsets.begin()->emplace(make_pair(ar->ident_, make_pair(t, offset)));
        offset += 4;
    }
}

void Compiler::visitNoInit(NoInit *no_init){
    visitIdent(no_init->ident_);
    int offset = -1 * 4*this->vars_size - 4;

    string type = this->last_type;
    this->vars_offsets.begin()->emplace(make_pair(no_init->ident_, make_pair(type, offset)));

    if(type == "string"){
        this->act_content += add_t_n(vector<string>({
            "mov eax, .LC_empty_str", "mov dword ptr [ebp" + to_string(offset) + "], eax"
        }));
    }else{
        this->act_content += add_t_n(vector<string>({
            "mov dword ptr [ebp" + to_string(offset) + "], 0"
        }));
    }
    this->vars_size++;
}

void Compiler::visitInit(Init *init){
    visitIdent(init->ident_);
    int offset = -1 * 4*this->vars_size - 4;
    string t = this->last_type;
    this->vars_size++;
    init->expr_->accept(this);

    this->vars_offsets.begin()->emplace(make_pair(init->ident_, make_pair(t, offset)));

    this->act_content += add_t_n(vector<string>({
        "pop eax", "mov dword ptr [ebp" + to_string(offset) + ("], eax")
    }));  
}

pair<string, int> Compiler::get_var(string var){
    for (auto const& mapa : this->vars_offsets){
        auto it = mapa.find(var);
        if(it != mapa.end()){
            return it->second;
        }
    }
    assert(false);
}

string offset_str(int offset){
    string maybe_plus = "";
    if(offset >= 0) {maybe_plus = "+";}
    return maybe_plus + to_string(offset);
}

void Compiler::visitAss(Ass *ass){
    visitIdent(ass->ident_);
    ass->expr_->accept(this);
    int byte_off = get_var(ass->ident_).second;

    this->act_content += add_t_n(vector<string>({
        "pop eax", "mov dword ptr [ebp" + offset_str(byte_off) + ("], eax")
    })); 
}

void Compiler::visitIncr(Incr *incr){
    visitIdent(incr->ident_);
    int byte_off = get_var(incr->ident_).second;

    this->act_content += add_t_n(vector<string>({
        "inc dword ptr [ebp" + offset_str(byte_off) + "]"
    }));
}

void Compiler::visitDecr(Decr *decr){
    visitIdent(decr->ident_);
    int byte_off = get_var(decr->ident_).second;
    this->act_content += add_t_n(vector<string>({
        "dec dword ptr [ebp" + offset_str(byte_off) + "]"
    }));
}

void Compiler::visitEVar(EVar *e_var){
    visitIdent(e_var->ident_);
    int byte_off = get_var(e_var->ident_).second;
    this->act_content += add_t_n(vector<string>({
        "push [ebp" + offset_str(byte_off) + "]"
    }));
    this->expr_type = get_var(e_var->ident_).first;
}