#include "../../bnfc/Skeleton.H"
#include <iostream>
#include "Type_checker.h"
#include "../Shared.h"

using namespace std;

void TypeChecker::visitBlk(Blk *blk)
{
  this->vars.push_front(map<string, string>());
  blk->liststmt_->accept(this);
  this->vars.pop_front();
}

void TypeChecker::visitBlkFun(Blk *blk)
{
  blk->liststmt_->accept(this);
}

void TypeChecker::visitListItem(ListItem *list_item)
{
  string dec_type = this->last_type;
  for (ListItem::iterator i = list_item->begin() ; i != list_item->end() ; ++i)
  {
    (*i)->accept(this);
    if(!this->vars.begin()->emplace(make_pair(this->last_ident, dec_type)).second){
      go_error((*i)->line_number, "This var was already declared in this scope.");
    }
  }
}

void TypeChecker::visitDecl(Decl *decl)
{
  decl->type_->accept(this);
  if (this->last_type == "void"){
    go_error(decl->line_number, "You should not declare variables of type void");
  }
  decl->listitem_->accept(this);
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

string TypeChecker::getVarType(string var, int line_number){
  for (auto const& mapa : this->vars){
    auto it = mapa.find(var);
    if(it != mapa.end()){
      return it->second;
    }
  }
  go_error(line_number, "This var was never declared");
  return "";
}

void TypeChecker::visitAss(Ass *ass)
{
  visitIdent(ass->ident_);
  string varType = this->getVarType(ass->ident_, ass->line_number);
  ass->expr_->accept(this);
  if(varType != this->expr_type){
    go_error(ass->line_number, "Type of expression does not match type of var.");
  }
}

void TypeChecker::visitIncr(Incr *incr)
{
  visitIdent(incr->ident_);
  if(this->getVarType(incr->ident_, incr->line_number) != "int"){
    go_error(incr->line_number, "You can use ++ and -- only on var with type 'int'.");
  }
}

void TypeChecker::visitDecr(Decr *decr)
{
  visitIdent(decr->ident_);
  if(this->getVarType(decr->ident_, decr->line_number) != "int"){
    go_error(decr->line_number, "You can use ++ and -- only on var with type 'int'.");
  }
}

void TypeChecker::visitFnDef(FnDef *fn_def)
{
  fn_def->type_->accept(this);
  this->act_fun_type = this->last_type;
  visitIdent(fn_def->ident_);
  fn_def->listarg_->accept(this);

  Blk* blk = dynamic_cast<Blk*>(fn_def->block_);
  this->vars.push_front(map<string, string>());

  for (Arg* arg: *(fn_def->listarg_)){
    Ar* ar = dynamic_cast<Ar*>(arg);
    ar->type_->accept(this);
    this->vars.begin()->emplace(make_pair(ar->ident_, this->last_type));
  }
  visitBlkFun(blk);
  this->vars.pop_front();
}

void TypeChecker::visitRet(Ret *ret)
{
  ret->expr_->accept(this);
  if(this->expr_type != this->act_fun_type){
    go_error(ret->line_number, "Return type does not match function type.");
  }
}

void TypeChecker::visitVRet(VRet *v_ret)
{
  if(this->act_fun_type != "void"){
    go_error(v_ret->line_number, "Return type does not match function type.");
  }
}

void TypeChecker::visitCond(Cond *cond)
{
  cond->expr_->accept(this);

  Decl* decl = dynamic_cast<Decl*>(cond->stmt_);
  if(decl){
    go_error(cond->line_number, "Declaration can't be only statment in if.");
  }

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
  Decl* decl1 = dynamic_cast<Decl*>(cond_else->stmt_1);
  Decl* decl2 = dynamic_cast<Decl*>(cond_else->stmt_2);
  if(decl1 || decl2){
    go_error(cond_else->line_number, "Declaration can't be only statment in if.");
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
  Decl* decl = dynamic_cast<Decl*>(while_->stmt_);
  if(decl){
    go_error(while_->line_number, "Declaration can't be only statment in while.");
  }
  while_->stmt_->accept(this);
}

void TypeChecker::visitEVar(EVar *e_var)
{
  visitIdent(e_var->ident_);
  this->expr_type = this->getVarType(e_var->ident_, e_var->line_number);
}

void TypeChecker::visitELitInt(ELitInt *e_lit_int)
{
  visitInteger(e_lit_int->integer_);
  this->expr_type = "int";
}

void TypeChecker::visitELitTrue(__attribute__((unused)) ELitTrue *e_lit_true)
{
  this->expr_type = "boolean";
}

void TypeChecker::visitELitFalse(__attribute__((unused)) ELitFalse *e_lit_false)
{
  this->expr_type = "boolean";
}

void TypeChecker::visitListExpr(ListExpr *list_expr, vector<string> args, int line)
{
  vector<string> argsExpr = vector<string>();
  for (ListExpr::iterator i = list_expr->begin() ; i != list_expr->end() ; ++i)
  {
    (*i)->accept(this);
    argsExpr.push_back(this->expr_type);
  }
  if(args.size() != argsExpr.size()){
    go_error(line, "Number of arguments does not match number of arguments in declaration.");
  }
  if(args != argsExpr){
    go_error(line, "Some arguments types are not matching with function declaration.");
  }
}

void TypeChecker::visitEApp(EApp *e_app)
{
  visitIdent(e_app->ident_);
  auto it = this->funs.find(e_app->ident_);
  if(it == this->funs.end()){
    go_error(e_app->line_number, "This function is not declared.");
  }
  this->visitListExpr(e_app->listexpr_, it->second.second, e_app->line_number);
  this->expr_type = it->second.first;
}

void TypeChecker::visitEString(EString *e_string)
{
  visitString(e_string->string_);
  this->expr_type = "string";
}

void TypeChecker::visitNeg(Neg *neg)
{
  neg->expr_->accept(this);
  if(this->expr_type != "int"){
    go_error(neg->line_number, "Expression is not 'int'.");
  }
  this->expr_type = "int";
}

void TypeChecker::visitNot(Not *not_)
{
  not_->expr_->accept(this);
  if(this->expr_type != "boolean"){
    go_error(not_->line_number, "Expression is not 'boolean'.");
  }
  this->expr_type = "boolean";
}

void TypeChecker::visitEMul(EMul *e_mul)
{
  e_mul->expr_1->accept(this);
  if(this->expr_type != "int"){
    go_error(e_mul->line_number, "Expression is not 'int'.");
  }
  e_mul->mulop_->accept(this);
  e_mul->expr_2->accept(this);
  if(this->expr_type != "int"){
    go_error(e_mul->line_number, "Expression is not 'int'.");
  }
  this->expr_type = "int";
}

void TypeChecker::visitEAdd(EAdd *e_add)
{
  e_add->expr_1->accept(this);
  string type1 = this->expr_type;
  e_add->addop_->accept(this);
  e_add->expr_2->accept(this);
  string type2 = this->expr_type;

  if(type1 != type2){
    go_error(e_add->line_number, "Types does not match.");
  }

  if(type1 != "string" && type1 != "int"){
    go_error(e_add->line_number, "Types for addition are not 'int' or 'string'.");
  }

  if(type1 == "string" && dynamic_cast<Minus*>(e_add->addop_)){
    go_error(e_add->line_number, "You cannot use '-' operation on yype 'string'.");
  }
  this->expr_type = type1;
}

void TypeChecker::visitERel(ERel *e_rel)
{
  e_rel->expr_1->accept(this);
  string type1 = this->expr_type;
  e_rel->relop_->accept(this);
  e_rel->expr_2->accept(this);
  string type2 = this->expr_type;
  if(type1 != type2){
    go_error(e_rel->line_number, "Types does not match.");
  }
  EQU* equ = dynamic_cast<EQU*>(e_rel->relop_);
  NE* ne = dynamic_cast<NE*>(e_rel->relop_);

  // operations like "<", "<="
  if(equ == NULL && ne == NULL){
    if(type1 != "int"){
      go_error(e_rel->line_number, "This operator works only with 'int'.");
    }
  }

  this->expr_type = "boolean";
}

void TypeChecker::visitEAnd(EAnd *e_and)
{
  e_and->expr_1->accept(this);
  if(this->expr_type != "boolean"){
    go_error(e_and->line_number, "Expression is not 'boolean'.");
  }
  e_and->expr_2->accept(this);
  if(this->expr_type != "boolean"){
    go_error(e_and->line_number, "Expression is not 'boolean'.");
  }
}

void TypeChecker::visitEOr(EOr *e_or)
{
  e_or->expr_1->accept(this);
  if(this->expr_type != "boolean"){
    go_error(e_or->line_number, "Expression is not 'boolean'.");
  }
  e_or->expr_2->accept(this);
  if(this->expr_type != "boolean"){
    go_error(e_or->line_number, "Expression is not 'boolean'.");
  }
}
