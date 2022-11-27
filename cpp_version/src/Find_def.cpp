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
  this->arg_types.push_back(this->last_type);
  visitIdent(ar->ident_);
  if(this->fun_args.count(ar->ident_) > 0){
	  go_error(ar->line_number, "Multiple arguments with name \"" + ar->ident_ + "\" in function declaration");
  }
  this->fun_args.emplace(ar->ident_);
}
