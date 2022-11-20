/****************************************************/
/* File: tiny.y                                     */
/* The TINY Yacc/Bison specification file           */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/
%{
#define YYPARSER /* distinguishes Yacc output from other code files */

#include "globals.h"
#include "util.h"
#include "scan.h"
#include "parse.h"

#define YYSTYPE TreeNode *
static char * savedName; /* for use in assignments */
static int savedLineNo;  /* ditto */
static TreeNode * savedTree; /* stores syntax tree for later return */
static int yylex(void);
int yyerror(char *s);

%}

%token IF ELSE INT RETURN VOID WHILE EMPTY
%token ID NUM
%token EQ LT GT GEQ LEQ EQEQ INEQ PLUS COMMA MINUS TIMES OVER
LPAREN RPAREN LBRACKETS RBRACKETS LCBRACES RCBRACES SEMI
%token ERROR

%% /* Grammar for Cminus */

program     : decl_list
                 { savedTree = $1; }
            ;
decl_list    : decl_list stmt
                 {
                 }
            | decl  { $$ = $1; }
            ;
decl        : var_decl {/* $$ = $1;*/ }
            | fun_decl { /*$$ = NULL;*/ }
            ;
var_decl    : type_spec ID SEMI { /*$$ = NULL*/ }
            | type_spec ID LBRACKETS NUM RBRACKETS SEMI  { /*$$ = NULL*/ }
            ;
type_spec   : INT { /*$$ = NULL*/ }
            | VOID {/* $$ = NULL*/ }
            ;
fun_decl    : type_spec ID LPAREN params RPAREN comp_decl { /*$$ = NULL*/ }
            ;
params      : param_list { /*$$ = NULL*/ }
            | VOID { /*$$ = NULL*/ }
            ;
param_list  : param_list COMMA param { /*$$ = NULL*/ }
            | param { /*$$ = NULL*/ }
            ;
param       : type_spec ID { /*$$ = NULL*/ }
            | type_spec ID LBRACKETS RBRACKETS { /*$$ = NULL*/ }
            ;
comp_decl   : LCBRACES loc_decl stmt_list RCBRACES { /*$$ = NULL*/ }
            ;
loc_decl    : loc_decl var_decl { $$ = NULL; }
            | EMPTY { $$ = NULL; }
            ;
stmt_list   : stmt_list stmt {
                $$ = newStmtNode($2->kind);

                $$->sibling = $1;
            }
            | EMPTY { $$ = NULL; }
            ;
stmt        : exp_decl { $$ = $1; }
            | comp_decl { $$ = $1; }
            | sel_decl { $$ = $1;}
            | iter_decl { $$ = $1; }
            | rtrn_decl { $$ = $1;}
            ;
exp_decl    : exp SEMI { $$ = $1; }
            | SEMI { $$ = NULL; }
            ;
sel_decl    : IF LPAREN exp RPAREN { $$ = NULL; }
            | IF LPAREN exp RPAREN stmt ELSE stmt { $$ = NULL; }
            ;
iter_decl   : WHILE LPAREN exp RPAREN stmt { $$ = NULL ;}
            ;
rtrn_decl   : RETURN SEMI {
                $$ = newExpNode(IdK);
                $$->attr.name = copyString($1);
            }
            | RETURN exp SEMI {
              $$ = newExpNode(IdK);
              $$->child[0] = $2;
              $$->attr.name = copyString($1);
            }
            ;
exp         : var EQ exp {
                $$ = newStmtNode(AssignK);
                $$->child[0] = $1;
                $$->child[1] = $3;
              }
            | simple_exp { $$ = $1; }
            ;
var         : ID {
                  $$ = newExpNode(IdK);
                  $$->attr.name =
                        copyString(tokenString);}
            | ID LBRACKETS exp RBRACKETS {
              $$ = newExpNode(IdK);
              $$->child[0] = $3;
              $$->attr.name =
                        copyString($1->attr.name);
            }
            ;
simple_exp  : sum_exp relational sum_exp {
              $$ = newExpNode(OpK);
              $$->child[0] = $1;
              $$->child[1] = $3;
              $$->attr.op = $2->attr.op
            }
            | sum_exp { $$ = $1;}
            ;
relational  : LEQ {
                $$ = newExpNode(OpK);
                $$->attr.op = LEQ;
            }
            | LT {
                $$ = newExpNode(OpK);
                $$->attr.op = LT;
            }
            | GT {
                $$ = newExpNode(OpK);
                $$->attr.op = GT;
            }
            | GEQ {
                $$ = newExpNode(OpK);
                $$->attr.op = GEQ;
            }
            | EQEQ {
                $$ = newExpNode(OpK);
                $$->attr.op = EQEQ;
            }
            | INEQ {
                $$ = newExpNode(OpK);
                $$->attr.op = INEQ;
            }
            ;
sum_exp     : sum_exp sum term {
              $$ = newExpNode(OpK);
              $$->child[0] = $1;
              $$->child[1] = $2;
              $$->attr.op = $2->attr.op;
            }
            | term { $$ = $1; }
            ;
sum         : PLUS {
                $$ = newExpNode(OpK);
                $$->attr.op = PLUS;
            }
            | MINUS {
                $$ = newExpNode(OpK);
                $$->attr.op = MINUS;
             }
            ;
term        : term mult factor {
                $$ = newExpNode(OpK);
                $$->child[0] = $1;
                $$->child[1] = $2;
                $$->attr.op = $2->attr.op;
            }
            | factor { $$ = $1; }
            ;
mult        : TIMES {
                $$ = newExpNode(OpK);
                $$->attr.op = TIMES;
            }
            | OVER {
               $$ = newExpNode(OpK);
               $$->attr.op = OVER;
            }
            ;
factor      : LPAREN exp RPAREN { $$ = $2; }
            | var { $$ = $2; }
            | activ { $$ = $2; }
            | NUM {
              $$ = newExpNode(ConstK);
              $$->attr.val = atoi(tokenString);
            }
            ;
activ       : ID LPAREN args RPAREN {
                $$ = newStmtNode(activK);
                $$->child[0] = $3;
                $$->attr.name = copyString($1->attr.name);
            }
            ;
args        : arg_list {
                $$ = $1;
              }
            | EMPTY { $$ = NULL; }
            ;
arg_list    : arg_list COMMA exp { $$ = NULL; }
            | exp { $$ = $1; }
            ;
/*
if_stmt     : IF LPAREN exp RPAREN LCBRACES stmt_seq RCBRACES
                 { $$ = newStmtNode(IfK);
                   $$->child[0] = $2;
                   $$->child[1] = $4;
                 }
            | IF LPAREN exp RPAREN LCBRACES stmt_seq RCBRACES ELSE LCBRACES stmt_seq RCBRACES
                 { $$ = newStmtNode(IfK);
                   $$->child[0] = $2;
                   $$->child[1] = $4;
                   $$->child[2] = $6;
                 }
            ;
exp         : simple_exp LT simple_exp
                 { $$ = newExpNode(OpK);
                   $$->child[0] = $1;
                   $$->child[1] = $3;
                   $$->attr.op = LT;
                 }
            | simple_exp EQ simple_exp
                 { $$ = newExpNode(OpK);
                   $$->child[0] = $1;
                   $$->child[1] = $3;
                   $$->attr.op = EQ;
                 }
            | simple_exp { $$ = $1; }
            ;
simple_exp  : simple_exp PLUS term
                 { $$ = newExpNode(OpK);
                   $$->child[0] = $1;
                   $$->child[1] = $3;
                   $$->attr.op = PLUS;
                 }
            | simple_exp MINUS term
                 { $$ = newExpNode(OpK);
                   $$->child[0] = $1;
                   $$->child[1] = $3;
                   $$->attr.op = MINUS;
                 }
            | term { $$ = $1; }
            ;
term        : term TIMES factor
                 { $$ = newExpNode(OpK);
                   $$->child[0] = $1;
                   $$->child[1] = $3;
                   $$->attr.op = TIMES;
                 }
            | term OVER factor
                 { $$ = newExpNode(OpK);
                   $$->child[0] = $1;
                   $$->child[1] = $3;
                   $$->attr.op = OVER;
                 }
            | factor { $$ = $1; }
            ;
factor      : LPAREN exp RPAREN
                 { $$ = $2; }
            | NUM
                 { $$ = newExpNode(ConstK);
                   $$->attr.val = atoi(tokenString);
                 }
            | ID { $$ = newExpNode(IdK);
                   $$->attr.name =
                         copyString(tokenString);
                 }
            | error { $$ = NULL; }
            ; */

%%

int yyerror(char * message)
{ fprintf(listing,"Syntax error at line %d: %s\n",lineno,message);
  fprintf(listing,"Current token: ");
  printToken(yychar,tokenString);
  Error = TRUE;
  return 0;
}

/* yylex calls getToken to make Yacc/Bison output
 * compatible with ealier versions of the TINY scanner
 */
static int yylex(void)
{ return getToken(); }

TreeNode * parse(void)
{ yyparse();
  return savedTree;
}

