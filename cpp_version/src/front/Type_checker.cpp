#include "../../bnfc/Skeleton.H"
#include <iostream>
#include "Type_checker.h"
#include "../Shared.h"
#include <cassert>

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
    this->last_type = dec_type;
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

void TypeChecker::visitClsAtr(ClsAtr *cls_atr)
{
  cls_atr->type_->accept(this);
  string dec_type = this->last_type;
  if (this->last_type == "void"){
    go_error(cls_atr->line_number, "You should not declare variables of type void");
  }
  for (ListItem::iterator i = cls_atr->listitem_->begin() ; i != cls_atr->listitem_->end() ; ++i)
  {
    this->last_type = dec_type;
    (*i)->accept(this);
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
  if(!type_compatible(dec_type, this->expr_type)){
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
  if(!type_compatible(varType, this->expr_type)){
    go_error(ass->line_number, "Type of expression " + this->expr_type +" does not match type of var " + varType);
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

void TypeChecker::visitClsDef(ClsDef *cls)
{
  this->vars.push_front(map<string, string>());
  this->vars.begin()->emplace(make_pair("self", cls->ident_));

  auto cls_obj = this->classes.find(cls->ident_);
  assert(cls_obj != this->classes.end());
  for(auto atr : cls_obj->second.attrs){
    // TODO moze tu dodaj tylko pierwszy jesli sie dziedzicza
    this->vars.begin()->emplace(make_pair(atr.first.first, atr.second.first));
  }
  cls->listclsdecl_->accept(this);
  this->vars.pop_front();
}

void TypeChecker::visitClsFun(ClsFun *cls_fun){
  cls_fun->type_->accept(this);
  this->act_fun_type = this->last_type;

  Blk* blk = dynamic_cast<Blk*>(cls_fun->block_);
  this->vars.push_front(map<string, string>());

  for (Arg* arg: *(cls_fun->listarg_)){
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
  if(!type_compatible(this->act_fun_type, this->expr_type)){
    go_error(ret->line_number, "Returned type " + this->expr_type + " does not match function type " + this->act_fun_type);
  }
}

void TypeChecker::visitVRet(VRet *v_ret)
{
  if(this->act_fun_type != "void"){
    go_error(v_ret->line_number, "Returns void but function type is " + this->act_fun_type);
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
  for (long unsigned int i=0; i<args.size(); i++){
    if(!type_compatible(args[i], argsExpr[i])){
      go_error(line, "Some arguments types are not matching with function declaration.");
    }
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
  this->expr_type = "boolean";
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
  this->expr_type = "boolean";
}

void TypeChecker::visitEClsAt(EClsAt *e_cls_at)
{
  /* Code For EClsAt Goes Here */

  e_cls_at->expr_->accept(this);
  string cls_type = this->expr_type;
  auto cls_it = this->classes.find(cls_type);
  if(cls_it == this->classes.end()){
    go_error(e_cls_at->line_number, "You are trying to reach Atr of class " + cls_type + " that was not decleared.");
  }

  for (auto const& [key, val] : cls_it->second.attrs){
    if(key.first == e_cls_at->ident_){
      this->expr_type = val.first;
      return;
    }
  }
  go_error(e_cls_at->line_number, "There is no Atr " + e_cls_at->ident_ + " in class " + cls_type); 
}

void TypeChecker::visitEClsApp(EClsApp *e_cls_app)
{
  /* Code For EClsApp Goes Here */

  e_cls_app->expr_->accept(this);
  string cls_type = this->expr_type;
  auto cls_it = this->classes.find(cls_type);
  assert(cls_it != this->classes.end());

  auto fun_it = cls_it->second.vtab.find(e_cls_app->ident_);
  if(fun_it == cls_it->second.vtab.end()){
     go_error(e_cls_app->line_number, "Class " + cls_type + " doesn't have method " + e_cls_app->ident_);
  } 

  this->visitListExpr(e_cls_app->listexpr_, get<2>(fun_it->second), e_cls_app->line_number);
  this->expr_type = get<1>(fun_it->second);
}

void TypeChecker::visitNewCls(NewCls *new_cls)
{
  /* Code For NewCls Goes Here */

  visitIdent(new_cls->ident_);
  if(this->classes.find(new_cls->ident_) == this->classes.end()){
    go_error(new_cls->line_number, "This class is not declared.");
  }
  this->expr_type = new_cls->ident_;
}

void TypeChecker::visitENull(ENull *e_null)
{
  /* Code For ENull Goes Here */
  e_null->type_->accept(this);
  if(this->classes.find(this->last_type) == this->classes.end()){
    go_error(e_null->line_number, "You can only cast Object types.");
  }
  this->expr_type = this->last_type;
}

void TypeChecker::visitClsType(ClsType *cls_type)
{
  /* Code For ClsType Goes Here */
  visitIdent(cls_type->ident_);
  if(this->classes.find(cls_type->ident_) == this->classes.end()){
    go_error(cls_type->line_number, "This class is not declared.");
  }
  this->last_type = cls_type->ident_;
}

bool TypeChecker::type_compatible(string l_type, string r_type){
  if(l_type == r_type){
    return true;
  }
  auto l_it = this->classes.find(l_type);
  auto r_it = this->classes.find(r_type);
  if(l_it == this->classes.end() || r_it == this->classes.end()){
    return false;
  }

  string ext = r_it->second.ext;
  while(ext != ""){
    if(ext == l_type){
      return true;
    }
    auto next_ext_it = this->classes.find(ext);
    assert(next_ext_it != this->classes.end());
    ext = next_ext_it->second.ext;
  }
  return false;  
}

void TypeChecker::visitAtrAss(AtrAss *atr_ass)
{
  /* Code For AtrAss Goes Here */
  EClsAt* l_side = dynamic_cast<EClsAt*>(atr_ass->expr_1);
  if(!l_side){
    go_error(atr_ass->line_number, "You are trying to assing to something that is not Class.Atr.");
  }

  atr_ass->expr_1->accept(this);
  string l_side_t = this->expr_type;

  atr_ass->expr_2->accept(this);
  if(!type_compatible(l_side_t, this->expr_type)){
    go_error(atr_ass->line_number, "Type of atr does not match type of expresion");
  }
}
