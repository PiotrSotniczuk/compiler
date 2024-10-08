#include "Compiler.h"
#include <vector>
#include "../Shared.h"
#include <cassert>
#include <iostream>

using namespace std;

string offset_str(int offset);

// adds /t and /n to line of asm code
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

// get directed list of extenions
vector<string> Compiler::get_ext_vec(string c){
    vector<string> ret = vector<string>();
    while(c != ""){
        ret.push_back(c);
        auto c_it = this->classes.find(c);
        // assert(c_it != this->classes.end());
                
        c = c_it->second.ext;
    }
    return ret;
}

// get type and offset of atribute
pair<string, int> Compiler::get_atr_vals(string c, string ident){
    // assert(c != "");
    auto cls_it = this->classes.find(c);
    // assert(cls_it != this->classes.end());
    auto exts = get_ext_vec(c);
    auto cls = cls_it->second;
    for(string ext : exts){
        auto attr_it = cls.attrs.find(make_pair(ident, ext));
        if(attr_it != cls.attrs.end()){
            return attr_it->second;
        }
    }
    // assert(false);
    return make_pair("", 0);
}

void Compiler::decr_env_str(map<string, pair<string, int>> env){
    for (auto const& [name, var_type_offs] : env){
        string typ = var_type_offs.first;
        int var_off = var_type_offs.second;
        if(typ == "string"){
            this->act_code += add_t_n({
                "push [ebp" + offset_str(var_off) + "]", 
                "call __garbDown",
                "add esp, 4"
            });
            continue;
        }
        // remove strings from objects
        auto cls_it = this->classes.find(typ);
        if(cls_it != this->classes.end()){
            auto klas = cls_it->second;
            for (auto const& [name_cls, atr_type_offs] : klas.attrs){
                if(atr_type_offs.first == "string"){
                    int atr_off = atr_type_offs.second;
                    this->act_code += add_t_n({
                        "push [ebp" + offset_str(var_off) + "]", // self on top
                        "pop eax", // self in 
                        "push dword ptr [eax+" + to_string(atr_off*4) + "]", // str on top
                        "call __garbDown",
                        "add esp, 4"
                    });
                }
            }
        }
    }
}

void Compiler::decr_str_leaving(bool end_block){
    // clean when leaving block only block 
    // clean when leaving function all until function
    if(end_block){
        auto last_env = this->vars_offsets.begin()->first;
        // decr all strings in map
        decr_env_str(last_env);
        return;
    }

    auto env_it = this->vars_offsets.begin();
    while(true){
        auto env = env_it->first;
        decr_env_str(env);
        bool fun_env = env_it->second;
        if(fun_env){
            return;
        } 
        env_it++;
    }
}

// give string a place in Local Constants
void Compiler::visitString(String x){
    int size = this->local_const.size();
    this->local_const.emplace(make_pair(x, size));
}

void Compiler::visitFnDef(FnDef *fn_def){
    this->act_fun = fn_def->ident_;
    string fun_prefix = fn_def->ident_ + ":\n";
    fun_prefix += add_t_n({"push ebp", "mov ebp, esp"});

    this->act_code = "";
    this->vars_size = 0;

    fn_def->type_->accept(this);
    visitIdent(fn_def->ident_);

    // add first env
    this->vars_offsets.push_front(make_pair(map<string, pair<string, int>>(), true));
    fn_def->listarg_->accept(this);

    Blk* blk = dynamic_cast<Blk*>(fn_def->block_);
    blk->liststmt_->accept(this);

    // reserve space for local variables
    fun_prefix += add_t_n({"sub esp, " + to_string(this->vars_size*4)});

    // add return at the end if type is void
    Void* type_void = dynamic_cast<Void*>(fn_def->type_);
    if (type_void && (!endsWith(this->act_code, "\tret\n"))){
        decr_str_leaving(true);
        this->act_code += add_t_n({"leave", "ret"});
    }

    this->full_code += fun_prefix + this->act_code;
    this->act_fun = "";
    this->vars_offsets.pop_front();
}


void Compiler::visitClsDef(ClsDef *cls){
    this->vars_offsets.push_front(make_pair(map<string, pair<string, int>>(), false));
    this->vars_offsets.begin()->first.emplace(make_pair("self", make_pair(cls->ident_, 8)));

    this->act_class = cls->ident_;
    cls->listclsdecl_->accept(this);
    decr_str_leaving(true);
    this->vars_offsets.pop_front();
    this->act_class = "";
}

void Compiler::visitClsFun(ClsFun *cls_fun){
    string fun_prefix = "_" + this->act_class + "_" + cls_fun->ident_ + ":\n";
    fun_prefix += add_t_n({"push ebp", "mov ebp, esp"});

    this->act_code = "";
    this->vars_size = 0;

    // add first env
    this->vars_offsets.push_front(make_pair(map<string, pair<string, int>>(), true));
    cls_fun->listarg_->accept(this);

    Blk* blk = dynamic_cast<Blk*>(cls_fun->block_);
    blk->liststmt_->accept(this);
    this->vars_offsets.pop_front();

    // reserve space for local variables
    fun_prefix += add_t_n({"sub esp, " + to_string(this->vars_size*4)});

    // add return at the end if type is void
    Void* type_void = dynamic_cast<Void*>(cls_fun->type_);
    if (type_void && (!endsWith(this->act_code, "\tret\n"))){
        decr_str_leaving(true);
        this->act_code += add_t_n({"leave", "ret"});
    }

    this->full_code += fun_prefix + this->act_code;
}

void Compiler::visitVRet(__attribute__((unused)) VRet *v_ret){
    decr_str_leaving(false);
    this->act_code += add_t_n({"leave", "ret"});
}

void Compiler::visitRet(Ret *ret){
    ret->expr_->accept(this);

    decr_str_leaving(false);

    if(this->act_fun == "main"){
        // return val on top
        // Uncoment to see how many strings missed freeing
        //this->act_code += add_t_n({"call __printLen"});
        this->act_code += add_t_n({"call __garbPurge"});
    }
    this->act_code += add_t_n({"pop eax", "leave", "ret"});
}

void Compiler::visitELitInt(ELitInt *e_lit_int){
    visitInteger(e_lit_int->integer_);
    this->act_code += add_t_n({"push " + to_string(e_lit_int->integer_)});
    this->expr_type = "int";
}

void Compiler::visitEMul(EMul *e_mul){
    e_mul->expr_1->accept(this);
    e_mul->mulop_->accept(this);
    e_mul->expr_2->accept(this);
    this->expr_type = "int";

    // multiplying
    Times* op_times = dynamic_cast<Times*>(e_mul->mulop_);
    if(op_times){
        this->act_code += add_t_n({
            "pop eax", "pop ecx", "imul ecx", "push eax"
        });
        return;
    }

    // dividing
    this->act_code += add_t_n({
        "pop ecx", "pop eax", "cdq", "idiv ecx"
    });

    Div* op_div = dynamic_cast<Div*>(e_mul->mulop_);
    if(op_div){
        this->act_code += add_t_n({"push eax"});
    }else{
        // modulo
        this->act_code += add_t_n({"push edx"});
    }
}

void Compiler::visitEAdd(EAdd *e_add){
    e_add->expr_1->accept(this);
    e_add->addop_->accept(this);
    e_add->expr_2->accept(this);

    // warning on stack expr_1 is deeper
    if(this->expr_type == "string"){
        this->act_code += add_t_n({
            "pop eax", "xchg eax, dword ptr [esp]", "push eax", 
            "call __concat",
            //put eax on stack below args
            "pop ecx",
            "pop edx",
            "push eax",
            "push edx",
            "push ecx",

            "call __garbDown", 
            "add esp, 4",
            "call __garbDown", 
            "add esp, 4"
        });
        return;
    }

    Plus* op_plus = dynamic_cast<Plus*>(e_add->addop_);
    string op = op_plus? "add" : "sub";

    this->act_code += add_t_n({
        "pop eax", op + " [esp], eax"
    });
}

void Compiler::visitNeg(Neg *neg){
    neg->expr_->accept(this);
    this->act_code += add_t_n({"neg dword ptr [esp]"});
    this->expr_type = "int";
}

void Compiler::visitEApp(EApp *e_app){
    visitIdent(e_app->ident_);
    e_app->listexpr_->accept(this);
    int size = e_app->listexpr_->size();

    this->expr_type = this->funs.find(e_app->ident_)->second.first;
    
    if(e_app->ident_ == "printString"){ // clean argument
        this->act_code += add_t_n({
            "call printString",
            "pop ecx", // to clean
            "push eax",
            "push ecx",
            "call __garbDown", // cleaned
            "add esp, 4", // 
        });
        return;
    }

    this->act_code += add_t_n({
        "call " + e_app->ident_, 
        "add esp, " + to_string(size * 4)
    });
    if(this->expr_type != "void"){
        this->act_code += add_t_n({"push eax"});
    }
}

void Compiler::visitListExpr(ListExpr *list_expr){
    // place aruments on stack in reversed order
    auto it = list_expr->rbegin();
    while (it != list_expr->rend()){
        (*it)->accept(this);
        it++;
    }
}

void Compiler::visitEString(EString *e_string){
    this->visitString(e_string->string_);
    int index = this->local_const.find(e_string->string_)->second;
    this->act_code += add_t_n({
        "lea eax, .LC" + to_string(index), "push eax", 
        "call __allocString", "add esp, 4", "push eax"
    });
    this->expr_type = "string";
}

void Compiler::visitELitTrue(__attribute__((unused)) ELitTrue *e_lit_true){
    this->act_code += add_t_n({"push 1"});
    this->expr_type = "boolean";
}

void Compiler::visitELitFalse(__attribute__((unused)) ELitFalse *e_lit_false){
    this->act_code += add_t_n({"push 0"});
    this->expr_type = "boolean";
}

void Compiler::visitNot(Not *not_){
    not_->expr_->accept(this);
    // boolean not done ass adding 1 modulo 2
    this->act_code += add_t_n({
        "add dword ptr [esp], 1", "and dword ptr [esp], 1"
    });
    this->expr_type = "boolean";
}

void Compiler::visitEAnd(EAnd *e_and){
    int c = this->get_l_count();
    string lazy = "_lazy" + to_string(c);

    // lazy evaluation
    // evaluate first and jump if false
    e_and->expr_1->accept(this);
    this->act_code += add_t_n({
        "pop eax", "test eax, eax", "jz " + lazy
    });

    // evaluate second and push the evaluation
    e_and->expr_2->accept(this);
    this->act_code += add_t_n({"pop eax", lazy+":", "push eax"});
    this->expr_type = "boolean";
}

void Compiler::visitEOr(EOr *e_or){
    int c = this->get_l_count();
    string lazy = "_lazy" + to_string(c);

    // lazy evaluation like above
    e_or->expr_1->accept(this);
    this->act_code += add_t_n({
        "pop eax", "test eax, eax", "jnz " + lazy
    });

    e_or->expr_2->accept(this);
    this->act_code += add_t_n({"pop eax", lazy+":", "push eax"});
    this->expr_type = "boolean";
}

void Compiler::visitCond(Cond *cond){
    int c = this->get_l_count();
    string if_cond = "_if" + to_string(c);
    cond->expr_->accept(this);
    this->act_code += add_t_n({
        "pop eax", "test eax, eax", "jz " + if_cond
    });

    cond->stmt_->accept(this);
    this->act_code += add_t_n({if_cond + ":"});
}

void Compiler::visitCondElse(CondElse *cond_else){
    int c = this->get_l_count();
    string if_else = "_if_else" + to_string(c);
    string end = "_end_if" + to_string(c);

    cond_else->expr_->accept(this);
    this->act_code += add_t_n({
        "pop eax", "test eax, eax", "jz " + if_else
    });

    cond_else->stmt_1->accept(this);
    this->act_code += add_t_n({"jmp " + end, if_else + ":"});

    cond_else->stmt_2->accept(this);
    this->act_code += add_t_n({end + ":"});
}

void Compiler::visitWhile(While *while_){
    int c = this->get_l_count();
    string start = "_while" + to_string(c);
    string end = "_end_while" + to_string(c);

    this->act_code += add_t_n({start + ":"});
    while_->expr_->accept(this);
    this->act_code += add_t_n({
        "pop eax", "test eax, eax", "jz " + end
    });

    while_->stmt_->accept(this);
    this->act_code += add_t_n({"jmp " + start, end + ":"});
}

void Compiler::visitERel(ERel *e_rel){
    e_rel->expr_1->accept(this);
    e_rel->relop_->accept(this);
    e_rel->expr_2->accept(this);

    string t = this->expr_type;
    this->expr_type = "boolean";

    if(t == "string"){
        if(dynamic_cast<EQU*>(e_rel->relop_)){
            this->act_code += add_t_n({
                "call __compare_str", "add esp, 8", "push eax"
            });
        }else{
            // make boolean not at the end
            this->act_code += add_t_n({
                "call __compare_str", "add esp, 8", "add eax, 1",
                "and eax, 1", "push eax"
            });
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

    // compare values from stack and set value depending on flags
    int c = this->get_l_count();
    this->act_code += add_t_n({
        "pop ecx", "pop eax", "push 1", "cmp eax, ecx", 
        "j" + op + " _rel_end" + to_string(c), "mov dword ptr [esp], 0", 
        "_rel_end" + to_string(c) + ":"
    });
}

void Compiler::visitBlk(Blk *blk){
    // new env
    this->vars_offsets.push_front(make_pair(map<string, pair<string, int>>(), false));
    blk->liststmt_->accept(this);
    decr_str_leaving(true);
    this->vars_offsets.pop_front();
}

// sets directions to arguments in vats_offsets (above ret in stack)
void Compiler::visitListArg(ListArg *list_arg){
    int offset = 2*4;
    if(this->act_class != ""){
        offset += 4; // place for self
    }
    for (ListArg::iterator i = list_arg->begin() ; i != list_arg->end() ; ++i){
        (*i)->accept(this);
        string t = this->last_type;
        Ar* ar = dynamic_cast<Ar*>(*i);
        this->vars_offsets.begin()->first.emplace(make_pair(ar->ident_, make_pair(t, offset)));
        offset += 4;
    }
}

void Compiler::visitNoInit(NoInit *no_init){
    visitIdent(no_init->ident_);
    int offset = -1 * 4*this->vars_size - 4;

    string type = this->last_type;
    // set direction to this variable
    this->vars_offsets.begin()->first.emplace(make_pair(no_init->ident_, make_pair(type, offset)));

    if(type == "string"){
        // init strings as ""
        this->act_code += add_t_n({
            "lea eax, .LC_empty_str",
            "push eax",
            "call __allocString", 
            "add esp, 4",
            "mov dword ptr [ebp" + to_string(offset) + "], eax"
        });
    }else{
        // init other things as 0
        this->act_code += add_t_n({
            "mov dword ptr [ebp" + to_string(offset) + "], 0"
        });
    }
    this->vars_size++;
}

// similar as above
void Compiler::visitInit(Init *init){
    visitIdent(init->ident_);
    int offset = -1 * 4*this->vars_size - 4;
    string t = this->last_type;
    this->vars_size++;
    init->expr_->accept(this);

    this->vars_offsets.begin()->first.emplace(make_pair(init->ident_, make_pair(t, offset)));

    // if string garb colec incr on var but decr of expr so zero
    this->act_code += add_t_n({
        "pop eax", "mov dword ptr [ebp" + to_string(offset) + ("], eax")
    });  
}

// get type and offset on stack from var_name
tuple<bool, string, int> Compiler::get_var(string var){
    for (auto const& p : this->vars_offsets){
        auto mapa = p.first;
        auto it = mapa.find(var);
        if(it != mapa.end()){
            return make_tuple(true, it->second.first, it->second.second);
        }
    }
    return make_tuple(false, "", 0);
}

// when writing offset print '+' if positive and nothing if negative
string offset_str(int offset){
    string maybe_plus = "";
    if(offset >= 0) {maybe_plus = "+";}
    return maybe_plus + to_string(offset);
}

void Compiler::visitAss(Ass *ass){
    ass->expr_->accept(this);
    auto var_val = get_var(ass->ident_);
    if(get<0>(var_val) == true){
        // garb coll we decr the expr but also incr on var so zero
        // we have to decr old val
        if(get<1>(var_val) == "string"){
            this->act_code += add_t_n({
                "push dword ptr [ebp" + offset_str(get<2>(var_val)) + "]",
                "call __garbDown",
                "add esp, 4"
            });
        }

        // normal
        this->act_code += add_t_n({
            "pop eax", "mov dword ptr [ebp" + offset_str(get<2>(var_val)) + ("], eax")
        }); 
        return;
    }

    auto atr_vals = get_atr_vals(this->act_class, ass->ident_);
    this->act_code += add_t_n({
        "pop eax",
        "mov ecx, dword ptr [ebp+8]", // self 
        "mov dword ptr [ecx+" + to_string(atr_vals.second*4) + "], eax" 
    });   
}

void Compiler::visitAtrAss(AtrAss *atr_ass){
  /* Code For AtrAss Goes Here */
    EClsAt* l_side = dynamic_cast<EClsAt*>(atr_ass->expr_1);
    // assert(l_side);
    // znajdz jaka to klasa
    // self na [esp]
    l_side->expr_->accept(this);
    string cls_typ = this->expr_type;
     
    auto atr_vals = get_atr_vals(cls_typ, l_side->ident_);

    // if string down on old value
    if(atr_vals.first == "string"){
            // self on stack
            this->act_code += add_t_n({
            "pop eax", // self
            "push eax", // copy of self
            "push dword ptr [eax+" + to_string(atr_vals.second*4) + "]", // push string
            "call __garbDown",
            "add esp, 4" // back self on top
        });
    }

    atr_ass->expr_2->accept(this);
    this->act_code += add_t_n({
        "pop ecx",
        "pop eax", // self
        "mov dword ptr [eax+" + to_string(atr_vals.second*4) + "], ecx", // self  
    });
}

void Compiler::visitIncr(Incr *incr){
    auto var_val = get_var(incr->ident_);
    // normal
    if(get<0>(var_val) == true){
        this->act_code += add_t_n({
            "inc dword ptr [ebp" + offset_str(get<2>(var_val)) + "]"
        }); 
        return;
    }

    // attr in class
    auto atr_vals = get_atr_vals(this->act_class, incr->ident_);
    this->act_code += add_t_n({
        "mov ecx, dword ptr [ebp+8]", // self // self na [esp]
        "inc dword ptr [ecx+" + to_string(atr_vals.second*4) + "]"
    });
}

void Compiler::visitDecr(Decr *decr){
    auto var_val = get_var(decr->ident_);
    if(get<0>(var_val) == true){
        this->act_code += add_t_n({
            "dec dword ptr [ebp" + offset_str(get<2>(var_val)) + "]"
        }); 
        return;
    }
    auto atr_vals = get_atr_vals(this->act_class, decr->ident_);
    this->act_code += add_t_n({
        "mov ecx, [ebp+8]", // self 
        "dec dword ptr [ecx+" + to_string(atr_vals.second*4) + "]"
    });
}

void Compiler::visitEVar(EVar *e_var){
    // normal
    auto in_vars = get_var(e_var->ident_);
    if(get<0>(in_vars)){
        this->act_code += add_t_n({
            "push [ebp" + offset_str(get<2>(in_vars)) + "]"
        });
        string typ = get<1>(in_vars);
        if(typ == "string"){
            this->act_code += add_t_n({
                "call __garbUp"
            });
        }
        this->expr_type = typ;
        return;
    }
    
    // what if it is class var
    auto atr_vals = get_atr_vals(this->act_class, e_var->ident_);
    this->act_code += add_t_n({
        "mov eax, dword ptr [ebp+8]", // self 
        "push [eax+" + to_string(atr_vals.second*4) + "]" 
    });
    this->expr_type = atr_vals.first;
}

void Compiler::visitEClsAt(EClsAt *e_cls_at){
  /* Code For EClsAt Goes Here */
    e_cls_at->expr_->accept(this);
    string cls_typ = this->expr_type;
    auto atr_vals = get_atr_vals(cls_typ, e_cls_at->ident_);
    this->act_code += add_t_n({
        "pop eax", // self 
        "push [eax+" + to_string(atr_vals.second*4) + "]"
    });
    this->expr_type = atr_vals.first;
}

void Compiler::visitEClsApp(EClsApp *e_cls_app){
    /* Code For EClsApp Goes Here */
    // firstly put arguments on stack
    e_cls_app->listexpr_->accept(this);

    // put self on stack
    e_cls_app->expr_->accept(this);
    string e_typ = this->expr_type;
    auto cls_it = this->classes.find(e_typ);
    // assert(cls_it != this->classes.end());

    auto fun_it = cls_it->second.vtab.find(e_cls_app->ident_);
    // assert(fun_it != cls_it->second.vtab.end());

    int offset = get<3>(fun_it->second);
    int size = e_cls_app->listexpr_->size() + 1; // +1 for self in args
    
   // on top stack is self
    this->act_code += add_t_n({
        "mov eax, dword ptr [esp]", // eax points to self
        "mov eax, dword ptr [eax]", // eax points to vtab
        "mov eax, dword ptr [eax+4*" + to_string(offset)+"]", // eax points to fun vtab
        "call eax", "add esp, " + to_string(size * 4)
    });

    this->expr_type = get<1>(fun_it->second);
    if(this->expr_type != "void"){
        this->act_code += add_t_n({"push eax"});
    }
}

void Compiler::visitNewCls(NewCls *new_cls){
  /* Code For NewCls Goes Here */
    auto cls_it = classes.find(new_cls->ident_);
    // assert(cls_it != this->classes.end());
    
    int size = cls_it->second.size;

    this->act_code += add_t_n({
        "push 4",
        "push " + to_string(size),
        "call calloc",
        "add esp, 8",
        "lea ecx, .vtab_" + new_cls->ident_,
        "mov dword ptr [eax], ecx",
        "push eax",
    });
    this->expr_type = new_cls->ident_;
}

void Compiler::visitENull(ENull *e_null){
  /* Code For ENull Goes Here */
    e_null->type_->accept(this);
    this->act_code += add_t_n({
        "push 0"
    });
    this->expr_type = this->last_type;
}

void Compiler::visitSExp(SExp *s_exp){
    /* Code For SExp Goes Here */
    s_exp->expr_->accept(this);
    if(this->expr_type == "string"){
        this->act_code += add_t_n({
            "call __garbDown"
        });
    }
}