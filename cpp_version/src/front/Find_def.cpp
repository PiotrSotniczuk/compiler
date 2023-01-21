#include <iostream>
#include "Find_def.h"
#include "../Shared.h"

using namespace std;

void FindDef::visitFnDef(FnDef *fn_def)
{
    fn_def->type_->accept(this);
    string ret_type = this->last_type;
  visitIdent(fn_def->ident_);
  if(fn_def->ident_ == "main"){
    this->main_line = fn_def->type_->line_number;
  }
  this->arg_types.clear();
  fn_def->listarg_->accept(this);

  if(!this->funs.emplace(make_pair(fn_def->ident_, make_pair(ret_type, this->arg_types))).second){
    go_error(fn_def->line_number, "Multiple functions with same name " + fn_def->ident_);
  };
  fn_def->block_->accept(this);
}

void FindDef::visitListArg(ListArg *list_arg){
	this->fun_args.clear();
	for (ListArg::iterator i = list_arg->begin() ; i != list_arg->end() ; ++i){
		(*i)->accept(this);
	}
}

void FindDef::visitAr(Ar *ar)
{
  ar->type_->accept(this);
  if (this->last_type == "void"){
    go_error(ar->line_number, "Arguments in function declaration cannot be of type void");
  }
  this->arg_types.push_back(this->last_type);
  visitIdent(ar->ident_);
  if(this->fun_args.count(ar->ident_) > 0){
	  go_error(ar->line_number, "Multiple arguments with name \"" + ar->ident_ + "\" in function declaration");
  }
  this->fun_args.emplace(ar->ident_);
}

void FindDef::visitClsDef(ClsDef *cls_def)
{
  visitIdent(cls_def->ident_);
  string cls_name = cls_def->ident_;
  Klass new_class = Klass();
  
  DoExt* ext = dynamic_cast<DoExt*>(cls_def->ext_);
  if(ext){
    new_class.ext = ext->ident_;
    auto base_class = this->classes.find(ext->ident_);
    if(base_class == this->classes.end()){
      go_error(cls_def->line_number, "Base Class " + ext->ident_ + " was not declared");
    }
    map<pair<string, string>, pair<string, int>> base_attrs(base_class->second.attrs);
    new_class.attrs = base_attrs;
    new_class.size = base_class->second.size;

    map<string, tuple<string, string, vector<string>, int>> base_vtab(base_class->second.vtab);
    new_class.vtab = base_vtab;
  }
  auto iter = cls_def->listclsdecl_->begin();
  auto end = cls_def->listclsdecl_->end();
  for(; iter < end; iter++){
    ClsAtr* atr = dynamic_cast<ClsAtr*>(*iter);
    if(atr){
      atr->type_->accept(this);
      string dec_t = this->last_type;
      for (ListItem::iterator i = atr->listitem_->begin() ; i != atr->listitem_->end() ; ++i){
        string atr_ident = "";
        NoInit* ni = dynamic_cast<NoInit*>(*i);
        if(ni){
          atr_ident = ni->ident_;
        }
        Init* init = dynamic_cast<Init*>(*i);
        if(init){
          atr_ident = init->ident_;
        }

        auto to_insert = make_pair(make_pair(atr_ident, cls_name), make_pair(dec_t, new_class.size));
        new_class.size++;
        if(!new_class.attrs.emplace(to_insert).second){
          go_error(atr->line_number, "Attribute " + atr_ident + " was already declared in this class");
        }
      }
    }

    ClsFun* fun = dynamic_cast<ClsFun*>(*iter);
    if(fun){
      fun->type_->accept(this);
      string ret_type = this->last_type;

      this->arg_types.clear();
      fun->listarg_->accept(this);
      
      int fun_offset = new_class.vtab.size();
      auto base_fun = new_class.vtab.find(fun->ident_);
      if(base_fun == new_class.vtab.end()){
        // insert normal function
        auto to_insert_sec = make_tuple(cls_name, ret_type, this->arg_types, fun_offset);
        new_class.vtab.emplace(make_pair(fun->ident_, to_insert_sec));
      }else{
        // there already is a funciton in this class
        if(get<0>(base_fun->second) == cls_name){
          go_error(fun->line_number, "This method " + fun->ident_ + " was already declared in this class");
        }
        auto to_insert_sec = make_tuple(cls_name, ret_type, this->arg_types, get<3>(base_fun->second));
        base_fun->second = to_insert_sec;
      }
    }
  }
  this->classes.emplace(make_pair(cls_name, new_class));
}

void CheckReturn::visitFnDef(FnDef *fn_def)
{
  this->there_is_return = false;
  fn_def->type_->accept(this);
  if(this->last_type != "void"){
    fn_def->block_->accept(this);
    if(!this->there_is_return){
      go_error(fn_def->line_number, "There is a missing return in function " + fn_def->ident_);
    }
  }
}

void CheckReturn::visitClsFun(ClsFun *fn_def)
{
  this->there_is_return = false;
  fn_def->type_->accept(this);
  if(this->last_type != "void"){
    fn_def->block_->accept(this);
    if(!this->there_is_return){
      go_error(fn_def->line_number, "There is a missing return in function " + fn_def->ident_);
    }
  }
}

void CheckReturn::visitListStmt(ListStmt *list_stmt)
{
  for (ListStmt::iterator i = list_stmt->begin() ; i != list_stmt->end() ; ++i)
  {
    (*i)->accept(this);
    if(this->there_is_return){
      break;
    }
  }
}

void CheckReturn::visitRet(__attribute__((unused)) Ret *ret)
{
  this->there_is_return = true;
}

void CheckReturn::visitEApp(EApp *e_app)
{
  visitIdent(e_app->ident_);
  if (e_app->ident_ == "error"){
    this->there_is_return = true;
  }
  e_app->listexpr_->accept(this);
}

void CheckReturn::visitCond(Cond *cond)
{
  ELitTrue* tr = dynamic_cast<ELitTrue*>(cond->expr_);
  if(tr){
    cond->stmt_->accept(this);
  }
}

void CheckReturn::visitCondElse(CondElse *cond_else)
{
  ELitTrue* tr = dynamic_cast<ELitTrue*>(cond_else->expr_);
  if(tr){
    cond_else->stmt_1->accept(this);
    return;
  }
  ELitFalse* fl = dynamic_cast<ELitFalse*>(cond_else->expr_);
  if(fl){
    cond_else->stmt_2->accept(this);
    return;
  }
  cond_else->stmt_1->accept(this);
  bool ret1 = this->there_is_return;

  this->there_is_return = false;
  cond_else->stmt_2->accept(this);
  bool ret2 = this->there_is_return;
  this->there_is_return = ret1 && ret2;
}

void CheckReturn::visitWhile(While *while_)
{
  ELitTrue* tr = dynamic_cast<ELitTrue*>(while_->expr_);
  if(tr){
    while_->stmt_->accept(this);
  }
}