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
  for (ListItem::iterator i = list_item->begin() ; i != list_item->end() ; ++i)
  {
    (*i)->accept(this);
    this->vars.begin()->emplace(make_pair(this->last_ident, this->last_type));
  }
}

void TypeChecker::visitNoInit(NoInit *no_init)
{
  visitIdent(no_init->ident_);
  this->last_ident = no_init->ident_;
}

void TypeChecker::visitInit(Init *init)
{
  visitIdent(init->ident_);
  this->last_ident = init->ident_;
  init->expr_->accept(this);
}