#ifndef PARSER_HEADER_FILE
#define PARSER_HEADER_FILE

#include<vector>
#include<string>


class Program;
class TopDef;
class ListTopDef;
class Arg;
class ListArg;
class Ext;
class ClsDecl;
class ListClsDecl;
class Block;
class ListStmt;
class Stmt;
class Item;
class ListItem;
class Type;
class ListType;
class Expr;
class ListExpr;
class AddOp;
class MulOp;
class RelOp;
typedef union
{
  int    _int;
  char   _char;
  double _double;
  char*  _string;
  Program* program_;
  TopDef* topdef_;
  ListTopDef* listtopdef_;
  Arg* arg_;
  ListArg* listarg_;
  Ext* ext_;
  ClsDecl* clsdecl_;
  ListClsDecl* listclsdecl_;
  Block* block_;
  ListStmt* liststmt_;
  Stmt* stmt_;
  Item* item_;
  ListItem* listitem_;
  Type* type_;
  ListType* listtype_;
  Expr* expr_;
  ListExpr* listexpr_;
  AddOp* addop_;
  MulOp* mulop_;
  RelOp* relop_;
} YYSTYPE;

Program* pProgram(FILE *inp);
Program* pProgram(const char *str);


#define _ERROR_ 258
#define _SYMB_11 259
#define _SYMB_23 260
#define _SYMB_17 261
#define _SYMB_12 262
#define _SYMB_0 263
#define _SYMB_1 264
#define _SYMB_15 265
#define _SYMB_14 266
#define _SYMB_7 267
#define _SYMB_4 268
#define _SYMB_10 269
#define _SYMB_8 270
#define _SYMB_9 271
#define _SYMB_16 272
#define _SYMB_5 273
#define _SYMB_18 274
#define _SYMB_19 275
#define _SYMB_6 276
#define _SYMB_22 277
#define _SYMB_20 278
#define _SYMB_21 279
#define _SYMB_24 280
#define _SYMB_25 281
#define _SYMB_26 282
#define _SYMB_27 283
#define _SYMB_28 284
#define _SYMB_29 285
#define _SYMB_30 286
#define _SYMB_31 287
#define _SYMB_32 288
#define _SYMB_33 289
#define _SYMB_34 290
#define _SYMB_35 291
#define _SYMB_36 292
#define _SYMB_37 293
#define _SYMB_2 294
#define _SYMB_13 295
#define _SYMB_3 296
#define _STRING_ 297
#define _INTEGER_ 298
#define _IDENT_ 299

extern YYSTYPE yylval;

#endif
