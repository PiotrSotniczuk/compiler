#include "../bnfc/Skeleton.H"
#include <iostream>
#include "Front.h"

using namespace std;

void go_error(int line, string msg){
	cerr << "ERROR\nline: " << to_string(line) << "\n" <<  msg << "\n";
	exit(1);
}

void MyVisitor::compile(Program *p){
    p->accept(this);
};

// void MyVisitor::visitFnDef(FnDef *fn_def)
// {
//   /* Code For FnDef Goes Here */

//   fn_def->type_->accept(this);
//   visitIdent(fn_def->ident_);
//   fn_def->listarg_->accept(this);
//   fn_def->block_->accept(this);

// }

void MyVisitor::visitAr(Ar *ar)
{
  ar->type_->accept(this);
  visitIdent(ar->ident_);
  if(this->fun_args.count(ar->ident_) > 0){
	go_error(ar->line_number, "Multiple arguments with name \"" + ar->ident_ + "\" in function declaration");
  }
  this->fun_args.emplace(ar->ident_);
}

// void MyVisitor::visitBlk(Blk *blk)
// {
//   /* Code For Blk Goes Here */

//   blk->liststmt_->accept(this);

// }

// void MyVisitor::visitEmpty(Empty *empty)
// {
//   /* Code For Empty Goes Here */


// }

// void MyVisitor::visitBStmt(BStmt *b_stmt)
// {
//   /* Code For BStmt Goes Here */

//   b_stmt->block_->accept(this);

// }

// void MyVisitor::visitDecl(Decl *decl)
// {
//   /* Code For Decl Goes Here */

//   decl->type_->accept(this);
//   decl->listitem_->accept(this);

// }

// void MyVisitor::visitAss(Ass *ass)
// {
//   /* Code For Ass Goes Here */

//   visitIdent(ass->ident_);
//   ass->expr_->accept(this);

// }

// void MyVisitor::visitIncr(Incr *incr)
// {
//   /* Code For Incr Goes Here */

//   visitIdent(incr->ident_);

// }

// void MyVisitor::visitDecr(Decr *decr)
// {
//   /* Code For Decr Goes Here */

//   visitIdent(decr->ident_);

// }

// void MyVisitor::visitRet(Ret *ret)
// {
//   /* Code For Ret Goes Here */

//   ret->expr_->accept(this);

// }

// void MyVisitor::visitVRet(VRet *v_ret)
// {
//   /* Code For VRet Goes Here */


// }

// void MyVisitor::visitCond(Cond *cond)
// {
//   /* Code For Cond Goes Here */

//   cond->expr_->accept(this);
//   cond->stmt_->accept(this);

// }

// void MyVisitor::visitCondElse(CondElse *cond_else)
// {
//   /* Code For CondElse Goes Here */

//   cond_else->expr_->accept(this);
//   cond_else->stmt_1->accept(this);
//   cond_else->stmt_2->accept(this);

// }

// void MyVisitor::visitWhile(While *while_)
// {
//   /* Code For While Goes Here */

//   while_->expr_->accept(this);
//   while_->stmt_->accept(this);

// }

// void MyVisitor::visitSExp(SExp *s_exp)
// {
//   /* Code For SExp Goes Here */

//   s_exp->expr_->accept(this);

// }

// void MyVisitor::visitNoInit(NoInit *no_init)
// {
//   /* Code For NoInit Goes Here */

//   visitIdent(no_init->ident_);

// }

// void MyVisitor::visitInit(Init *init)
// {
//   /* Code For Init Goes Here */

//   visitIdent(init->ident_);
//   init->expr_->accept(this);

// }

// void MyVisitor::visitInt(Int *int_)
// {
//   /* Code For Int Goes Here */


// }

// void MyVisitor::visitStr(Str *str)
// {
//   /* Code For Str Goes Here */


// }

// void MyVisitor::visitBool(Bool *bool_)
// {
//   /* Code For Bool Goes Here */


// }

// void MyVisitor::visitVoid(Void *void_)
// {
//   /* Code For Void Goes Here */


// }

// void MyVisitor::visitFun(Fun *fun)
// {
//   /* Code For Fun Goes Here */

//   fun->type_->accept(this);
//   fun->listtype_->accept(this);

// }

// void MyVisitor::visitEVar(EVar *e_var)
// {
//   /* Code For EVar Goes Here */

//   visitIdent(e_var->ident_);

// }

// void MyVisitor::visitELitInt(ELitInt *e_lit_int)
// {
//   /* Code For ELitInt Goes Here */

//   visitInteger(e_lit_int->integer_);

// }

// void MyVisitor::visitELitTrue(ELitTrue *e_lit_true)
// {
//   /* Code For ELitTrue Goes Here */


// }

// void MyVisitor::visitELitFalse(ELitFalse *e_lit_false)
// {
//   /* Code For ELitFalse Goes Here */


// }

// void MyVisitor::visitEApp(EApp *e_app)
// {
//   /* Code For EApp Goes Here */

//   visitIdent(e_app->ident_);
//   e_app->listexpr_->accept(this);

// }

// void MyVisitor::visitEString(EString *e_string)
// {
//   /* Code For EString Goes Here */

//   visitString(e_string->string_);

// }

// void MyVisitor::visitNeg(Neg *neg)
// {
//   /* Code For Neg Goes Here */

//   neg->expr_->accept(this);

// }

// void MyVisitor::visitNot(Not *not_)
// {
//   /* Code For Not Goes Here */

//   not_->expr_->accept(this);

// }

// void MyVisitor::visitEMul(EMul *e_mul)
// {
//   /* Code For EMul Goes Here */

//   e_mul->expr_1->accept(this);
//   e_mul->mulop_->accept(this);
//   e_mul->expr_2->accept(this);

// }

// void MyVisitor::visitEAdd(EAdd *e_add)
// {
//   /* Code For EAdd Goes Here */

//   e_add->expr_1->accept(this);
//   e_add->addop_->accept(this);
//   e_add->expr_2->accept(this);

// }

// void MyVisitor::visitERel(ERel *e_rel)
// {
//   /* Code For ERel Goes Here */

//   e_rel->expr_1->accept(this);
//   e_rel->relop_->accept(this);
//   e_rel->expr_2->accept(this);

// }

// void MyVisitor::visitEAnd(EAnd *e_and)
// {
//   /* Code For EAnd Goes Here */

//   e_and->expr_1->accept(this);
//   e_and->expr_2->accept(this);

// }

// void MyVisitor::visitEOr(EOr *e_or)
// {
//   /* Code For EOr Goes Here */

//   e_or->expr_1->accept(this);
//   e_or->expr_2->accept(this);

// }

// void MyVisitor::visitPlus(Plus *plus)
// {
//   /* Code For Plus Goes Here */


// }

// void MyVisitor::visitMinus(Minus *minus)
// {
//   /* Code For Minus Goes Here */


// }

// void MyVisitor::visitTimes(Times *times)
// {
//   /* Code For Times Goes Here */


// }

// void MyVisitor::visitDiv(Div *div)
// {
//   /* Code For Div Goes Here */


// }

// void MyVisitor::visitMod(Mod *mod)
// {
//   /* Code For Mod Goes Here */


// }

// void MyVisitor::visitLTH(LTH *lth)
// {
//   /* Code For LTH Goes Here */


// }

// void MyVisitor::visitLE(LE *le)
// {
//   /* Code For LE Goes Here */


// }

// void MyVisitor::visitGTH(GTH *gth)
// {
//   /* Code For GTH Goes Here */


// }

// void MyVisitor::visitGE(GE *ge)
// {
//   /* Code For GE Goes Here */


// }

// void MyVisitor::visitEQU(EQU *equ)
// {
//   /* Code For EQU Goes Here */


// }

// void MyVisitor::visitNE(NE *ne)
// {
//   /* Code For NE Goes Here */


// }


// void MyVisitor::visitListTopDef(ListTopDef *list_top_def)
// {
//   for (ListTopDef::iterator i = list_top_def->begin() ; i != list_top_def->end() ; ++i)
//   {
//     (*i)->accept(this);
//   }
// }

void MyVisitor::visitListArg(ListArg *list_arg){
	this->fun_args.clear();
	for (ListArg::iterator i = list_arg->begin() ; i != list_arg->end() ; ++i){
		(*i)->accept(this);
	}
}

// void MyVisitor::visitListStmt(ListStmt *list_stmt)
// {
//   for (ListStmt::iterator i = list_stmt->begin() ; i != list_stmt->end() ; ++i)
//   {
//     (*i)->accept(this);
//   }
// }

// void MyVisitor::visitListItem(ListItem *list_item)
// {
//   for (ListItem::iterator i = list_item->begin() ; i != list_item->end() ; ++i)
//   {
//     (*i)->accept(this);
//   }
// }

// void MyVisitor::visitListType(ListType *list_type)
// {
//   for (ListType::iterator i = list_type->begin() ; i != list_type->end() ; ++i)
//   {
//     (*i)->accept(this);
//   }
// }

// void MyVisitor::visitListExpr(ListExpr *list_expr)
// {
//   for (ListExpr::iterator i = list_expr->begin() ; i != list_expr->end() ; ++i)
//   {
//     (*i)->accept(this);
//   }
// }


// void MyVisitor::visitInteger(Integer x)
// {
//   /* Code for Integer Goes Here */
// }

// void MyVisitor::visitChar(Char x)
// {
//   /* Code for Char Goes Here */
// }

// void MyVisitor::visitDouble(Double x)
// {
//   /* Code for Double Goes Here */
// }

// void MyVisitor::visitString(String x)
// {
//   /* Code for String Goes Here */
// }

// void MyVisitor::visitIdent(Ident x)
// {
//   /* Code for Ident Goes Here */
// }



