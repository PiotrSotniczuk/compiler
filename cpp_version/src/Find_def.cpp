#include <iostream>
#include "Find_def.h"
#include "Shared.h"

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