/*** BNFC-Generated Visitor Design Pattern Skeleton. ***/
/* This implements the common visitor design pattern.
   Note that this method uses Visitor-traversal of lists, so
   List->accept() does NOT traverse the list. This allows different
   algorithms to use context information differently. */

#include "Skeleton.H"



void Skeleton::visitProgram(Program *t) {} //abstract class
void Skeleton::visitTopDef(TopDef *t) {} //abstract class
void Skeleton::visitArg(Arg *t) {} //abstract class
void Skeleton::visitBlock(Block *t) {} //abstract class
void Skeleton::visitStmt(Stmt *t) {} //abstract class
void Skeleton::visitItem(Item *t) {} //abstract class
void Skeleton::visitType(Type *t) {} //abstract class
void Skeleton::visitExpr(Expr *t) {} //abstract class
void Skeleton::visitAddOp(AddOp *t) {} //abstract class
void Skeleton::visitMulOp(MulOp *t) {} //abstract class
void Skeleton::visitRelOp(RelOp *t) {} //abstract class

void Skeleton::run(Program *p){
    p->accept(this);
}

void Skeleton::visitProg(Prog *prog)
{
    prog->listtopdef_->accept(this);

}

void Skeleton::visitFnDef(FnDef *fn_def)
{
    fn_def->type_->accept(this);
  visitIdent(fn_def->ident_);
  fn_def->listarg_->accept(this);
  fn_def->block_->accept(this);

}

void Skeleton::visitAr(Ar *ar)
{
    ar->type_->accept(this);
  visitIdent(ar->ident_);

}

void Skeleton::visitBlk(Blk *blk)
{
    blk->liststmt_->accept(this);

}

void Skeleton::visitEmpty(Empty *empty)
{
  
}

void Skeleton::visitBStmt(BStmt *b_stmt)
{
    b_stmt->block_->accept(this);

}

void Skeleton::visitDecl(Decl *decl)
{
    decl->type_->accept(this);
  decl->listitem_->accept(this);

}

void Skeleton::visitAss(Ass *ass)
{
    visitIdent(ass->ident_);
  ass->expr_->accept(this);

}

void Skeleton::visitIncr(Incr *incr)
{
    visitIdent(incr->ident_);

}

void Skeleton::visitDecr(Decr *decr)
{
    visitIdent(decr->ident_);

}

void Skeleton::visitRet(Ret *ret)
{
    ret->expr_->accept(this);

}

void Skeleton::visitVRet(VRet *v_ret)
{
  
}

void Skeleton::visitCond(Cond *cond)
{
    cond->expr_->accept(this);
  cond->stmt_->accept(this);

}

void Skeleton::visitCondElse(CondElse *cond_else)
{
    cond_else->expr_->accept(this);
  cond_else->stmt_1->accept(this);
  cond_else->stmt_2->accept(this);

}

void Skeleton::visitWhile(While *while_)
{
    while_->expr_->accept(this);
  while_->stmt_->accept(this);

}

void Skeleton::visitSExp(SExp *s_exp)
{
    s_exp->expr_->accept(this);

}

void Skeleton::visitNoInit(NoInit *no_init)
{
    visitIdent(no_init->ident_);

}

void Skeleton::visitInit(Init *init)
{
    visitIdent(init->ident_);
  init->expr_->accept(this);

}

void Skeleton::visitInt(__attribute__((unused)) Int *int_)
{
  this->last_type = "int";
}

void Skeleton::visitStr(__attribute__((unused)) Str *str)
{
  this->last_type = "string";
}

void Skeleton::visitBool(__attribute__((unused)) Bool *bool_)
{
  this->last_type = "boolean";
}

void Skeleton::visitVoid(__attribute__((unused)) Void *void_)
{
  this->last_type = "void";
}

void Skeleton::visitFun(Fun *fun)
{
  fun->type_->accept(this);
  std::string ret_type = this->last_type;
  fun->listtype_->accept(this);
  this->last_type = ret_type + " " + this->last_type;
}

void Skeleton::visitListType(ListType *list_type)
{
    std::string all = "(";
  for (ListType::iterator i = list_type->begin() ; i != list_type->end() ; ++i)
  {
    (*i)->accept(this);
    all += this->last_type + ",";
  }
  all.pop_back();
  this->last_type = all + ")";
}

void Skeleton::visitEVar(EVar *e_var)
{
    visitIdent(e_var->ident_);

}

void Skeleton::visitELitInt(ELitInt *e_lit_int)
{
    visitInteger(e_lit_int->integer_);

}

void Skeleton::visitELitTrue(ELitTrue *e_lit_true)
{
  
}

void Skeleton::visitELitFalse(ELitFalse *e_lit_false)
{
  
}

void Skeleton::visitEApp(EApp *e_app)
{
    visitIdent(e_app->ident_);
  e_app->listexpr_->accept(this);

}

void Skeleton::visitEString(EString *e_string)
{
    visitString(e_string->string_);

}

void Skeleton::visitNeg(Neg *neg)
{
    neg->expr_->accept(this);

}

void Skeleton::visitNot(Not *not_)
{
    not_->expr_->accept(this);

}

void Skeleton::visitEMul(EMul *e_mul)
{
    e_mul->expr_1->accept(this);
  e_mul->mulop_->accept(this);
  e_mul->expr_2->accept(this);

}

void Skeleton::visitEAdd(EAdd *e_add)
{
    e_add->expr_1->accept(this);
  e_add->addop_->accept(this);
  e_add->expr_2->accept(this);

}

void Skeleton::visitERel(ERel *e_rel)
{
    e_rel->expr_1->accept(this);
  e_rel->relop_->accept(this);
  e_rel->expr_2->accept(this);

}

void Skeleton::visitEAnd(EAnd *e_and)
{
    e_and->expr_1->accept(this);
  e_and->expr_2->accept(this);

}

void Skeleton::visitEOr(EOr *e_or)
{
    e_or->expr_1->accept(this);
  e_or->expr_2->accept(this);

}

void Skeleton::visitPlus(Plus *plus)
{
  
}

void Skeleton::visitMinus(Minus *minus)
{
  
}

void Skeleton::visitTimes(Times *times)
{
  
}

void Skeleton::visitDiv(Div *div)
{
  
}

void Skeleton::visitMod(Mod *mod)
{
  
}

void Skeleton::visitLTH(LTH *lth)
{
  
}

void Skeleton::visitLE(LE *le)
{
  
}

void Skeleton::visitGTH(GTH *gth)
{
  
}

void Skeleton::visitGE(GE *ge)
{
  
}

void Skeleton::visitEQU(EQU *equ)
{
  
}

void Skeleton::visitNE(NE *ne)
{
  
}


void Skeleton::visitListTopDef(ListTopDef *list_top_def)
{
  for (ListTopDef::iterator i = list_top_def->begin() ; i != list_top_def->end() ; ++i)
  {
    (*i)->accept(this);
  }
}

void Skeleton::visitListArg(ListArg *list_arg)
{
  for (ListArg::iterator i = list_arg->begin() ; i != list_arg->end() ; ++i)
  {
    (*i)->accept(this);
  }
}

void Skeleton::visitListStmt(ListStmt *list_stmt)
{
  for (ListStmt::iterator i = list_stmt->begin() ; i != list_stmt->end() ; ++i)
  {
    (*i)->accept(this);
  }
}

void Skeleton::visitListItem(ListItem *list_item)
{
  for (ListItem::iterator i = list_item->begin() ; i != list_item->end() ; ++i)
  {
    (*i)->accept(this);
  }
}

void Skeleton::visitListExpr(ListExpr *list_expr)
{
  for (ListExpr::iterator i = list_expr->begin() ; i != list_expr->end() ; ++i)
  {
    (*i)->accept(this);
  }
}


void Skeleton::visitInteger(Integer x)
{
  }

void Skeleton::visitChar(Char x)
{
  }

void Skeleton::visitDouble(Double x)
{
  }

void Skeleton::visitString(String x)
{
  }

void Skeleton::visitIdent(Ident x)
{
  }



