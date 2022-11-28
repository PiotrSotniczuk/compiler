#include "../bnfc/Skeleton.H"
#include <iostream>
#include "Front.h"
#include "Shared.h"

using namespace std;

void TypeChecker::visitBlk(Blk *blk)
{
  this->vars.push_front(map<string, string>());
  blk->liststmt_->accept(this);
  
  this->vars.pop_front();
}

void TypeChecker::visitListItem(ListItem *list_item)
{
  string dec_type = this->last_type;
  for (ListItem::iterator i = list_item->begin() ; i != list_item->end() ; ++i)
  {
    (*i)->accept(this);
    this->vars.begin()->emplace(make_pair(this->last_ident, dec_type));
  }
}

void TypeChecker::visitNoInit(NoInit *no_init)
{
  visitIdent(no_init->ident_);
  this->last_ident = no_init->ident_;
}

void TypeChecker::visitInit(Init *init)
{
  string dec_type = this->last_type;
  visitIdent(init->ident_);
  this->last_ident = init->ident_;
  init->expr_->accept(this);
  if(this->expr_type != dec_type){
    go_error(init->line_number, "Type of var does not match type of expresion");
  }
}

string TypeChecker::getVarType(string var){
  for (auto const& mapa : this->vars){
    auto it = mapa.find(var);
    if(it != mapa.end()){
      return it->second;
    }
  }
  return "";
}

void TypeChecker::visitAss(Ass *ass)
{
  visitIdent(ass->ident_);
  string varType = this->getVarType(ass->ident_);
  if(varType == ""){
    go_error(ass->line_number, "The var you want to assign to was not declared.");
  }
  ass->expr_->accept(this);
  if(varType != this->expr_type){
    go_error(ass->line_number, "Type of expression does not match type of var.");
  }
}

void TypeChecker::visitIncr(Incr *incr)
{
  visitIdent(incr->ident_);
  if(this->getVarType(incr->ident_) != "int"){
    go_error(incr->line_number, "You can use ++ and -- only on var with type 'int'.");
  }
}

void TypeChecker::visitDecr(Decr *decr)
{
  visitIdent(decr->ident_);
  if(this->getVarType(decr->ident_) != "int"){
    go_error(decr->line_number, "You can use ++ and -- only on var with type 'int'.");
  }
}

void TypeChecker::visitFnDef(FnDef *fn_def)
{
  fn_def->type_->accept(this);
  this->act_fun_type = this->last_type;
  visitIdent(fn_def->ident_);
  fn_def->listarg_->accept(this);
  fn_def->block_->accept(this);
}

void TypeChecker::visitRet(Ret *ret)
{
  ret->expr_->accept(this);
  if(this->expr_type != this->act_fun_type){
    go_error(ret->line_number, "Return type does not match function type.");
  }
}

void TypeChecker::visitCond(Cond *cond)
{
  cond->expr_->accept(this);
  if(this->expr_type != "boolean"){
    go_error(cond->line_number, "Expresion in condition should be of type boolean.");
  }
  cond->stmt_->accept(this);
}

void TypeChecker::visitCondElse(CondElse *cond_else)
{
  cond_else->expr_->accept(this);
  if(this->expr_type != "boolean"){
    go_error(cond_else->line_number, "Expresion in condition should be of type boolean.");
  }
  cond_else->stmt_1->accept(this);
  cond_else->stmt_2->accept(this);
}

void TypeChecker::visitWhile(While *while_)
{
  while_->expr_->accept(this);
  if(this->expr_type != "boolean"){
    go_error(while_->line_number, "Expresion in condition should be of type boolean.");
  }
  while_->stmt_->accept(this);
}

