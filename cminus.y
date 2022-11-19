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

%token IF ELSE INT RETURN VOID WHILE
%token ID NUM
%token EQ LT GT GEQ LEQ EQEQ INEQ PLUS COMMA MINUS TIMES OVER
LPAREN RPAREN LBRACKETS RBRACKETS LCBRACES RCBRACES SEMI
%token ERROR 

%%

program     : decl_list   {  };

decl_list   : decl_list decl {   };
            | decl {  };

decl        : var_decl {  };
            | func_decl {  };

var_decl    : type_spec ID SEMI {  };
            | type_spec ID LBRACKETS NUM RBRACKETS SEMI {  };

type_spec   : INT {  };
            | VOID {  };

func_decl   : type_spec ID LPAREN params RPAREN comp_decl {  };

params      : param_list {  };
            | VOID {  };

param_list  : param_list COMMA param {  };
            | param {  };

param       : type_spec ID {  };
            | type_spec ID LBRACKETS RBRACKETS {  };

comp_decl   : LCBRACES local_decl stat_list RCBRACES {  };

local_decl  : local_decl var_decl {  };
            | %empty {  };

stat_list   : stat_list stat {  };
            | %empty {  };

stat        : exp_decl {  };
            | comp_decl {  };
            | selec_decl {  };
            | iter_decl {  };
            | ret_decl {  };            

exp_decl    : exp SEMI {  };
            | SEMI {  };

selec_decl  : IF LPAREN exp RPAREN stat {  };
            | IF LPAREN exp RPAREN stat ELSE stat {  };

iter_decl   : WHILE LPAREN exp RPAREN stat { };

ret_decl    : RETURN SEMI {  };
            | RETURN exp SEMI {  };

exp         : var EQ exp {  };
            | simple_exp {  };

var         : ID {  };
            | ID LBRACKETS exp RBRACKETS {  };

simple_exp  : soma_exp relac soma_exp {  };
            | soma_exp {  };

relac       : LEQ   {  };
            | LT    {  };
            | GT    {  };           
            | GEQ   {  };
            | EQEQ  {  };
            | INEQ  {  };

soma_exp    : soma_exp soma term {  };
            | term {  };

soma        : PLUS {  };
            | MINUS {  };

term        : term mult fator { };
            | fator { };

mult        : TIMES {  };
            | OVER {  };

fator       : LPAREN exp RPAREN {  }; 
            | var {  };
            | ativ {  };
            | NUM {  };

ativ        : ID LPAREN args RPAREN {  }; 

args        : arg_list {  };
            | %empty {  };

arg_list    : arg_list COMMA exp {  };
            | exp  {  };

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

