/****************************************************/
/* File: analyze.c                                  */
/* Semantic analyzer implementation                 */
/* for the TINY compiler                            */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#include "globals.h"
#include "symtab.h"
#include "analyze.h"

/* counter for variable memory locations */
static int location = 0;
char *currentScope = "global";

/* Procedure traverse is a generic recursive 
 * syntax tree traversal routine:
 * it applies preProc in preorder and postProc 
 * in postorder to tree pointed to by t
 */
static void traverse( TreeNode * t,
               void (* preProc) (TreeNode *),
               void (* postProc) (TreeNode *) )
{ if (t != NULL)
  { preProc(t);
    { int i;
      for (i=0; i < MAXCHILDREN; i++)
        traverse(t->child[i],preProc,postProc);
    }
    postProc(t);
    traverse(t->sibling,preProc,postProc);
  }
}

/* nullProc is a do-nothing procedure to 
 * generate preorder-only or postorder-only
 * traversals from traverse
 */
static void nullProc(TreeNode * t)
{ if (t==NULL) return;
  else return;
}

/* Procedure insertNode inserts 
 * identifiers stored in t into 
 * the symbol table 
 */
static void insertDecl( TreeNode * t)
{ switch (t->nodekind)
  { case StmtK:
      switch (t->kind.stmt){
        case VarDeclK:
          t->decl = 1;
          break;
        case FuncDeclK:
          t->decl = 2;
          break;
        case ArrDeclK:
          t->decl = 3;
          break;
        default:
          break;
      }
      break;
    case ExpK:
      switch (t->kind.exp)
      { 
        default:
          break;
      }
      break;
    default:
      break;
  }
}

static void semanticError(TreeNode * t, char * message)
{ fprintf(listing,"Erro semantico na linha %d: %s\n",t->lineno,message);
  Error = TRUE;
}

static void insertType( TreeNode * t)
{ switch (t->nodekind)
  { case StmtK:
      switch (t->kind.stmt){
        case TypeK:
          if (strcmp(t->attr.name, "void") == 0) {
            t->child[0]->type = Void;
          } else if (strcmp(t->attr.name, "int") == 0) {
            t->child[0]->type = Integer;
          }
          break;
      }
      break;
    case ExpK:
      switch (t->kind.exp)
      { 
        default:
          break;
      }
      break;
    default:
      break;
  }
}

static void insertNode( TreeNode * t)
{
 switch (t->nodekind)
  { case StmtK:
      switch (t->kind.stmt)
      { 
        int varIsNotGlobal, varIsNotOnCurrentScope;
        case AssignK:
          varIsNotGlobal = st_lookup(t->attr.name, "global");
          varIsNotOnCurrentScope = st_lookup(t->attr.name, currentScope);

          if(t->child[1]->kind.stmt == ActivK && !(strcmp(t->child[1]->attr.name, "input") == 0 || strcmp(t->child[1]->attr.name, "output") == 0)) { 
            if (t->child[1]->type != t->type) {
              semanticError(t, "atribuição inválida");
            }
          }

          if (varIsNotOnCurrentScope == -1 && varIsNotGlobal == -1) {
            semanticError(t, "variável não declarada");
          }
          if (varIsNotGlobal != -1) {
            st_insert(t->attr.name,t->lineno,location++,t->decl,t->type, "global");
          }
          else {
            st_insert(t->attr.name,t->lineno,location++,t->decl,t->type, currentScope);
          }
          break;
        case VarDeclK:
          varIsNotGlobal = st_lookup(t->attr.name, "global");
          varIsNotOnCurrentScope = st_lookup(t->attr.name, currentScope);
          if (varIsNotOnCurrentScope == -1 && varIsNotGlobal == -1) {
            /* not yet in table, so treat as new definition */
            st_insert(t->attr.name,t->lineno,location++,t->decl,t->type, currentScope);
          }
          else {
            int var_decl_current_scope = st_lookup_decl(t->attr.name, currentScope);
            int var_decl_global_scope = st_lookup_decl(t->attr.name, "global");
            /* already in table, so ignore location, 
             add line number of use only */ 
            if ((varIsNotGlobal != -1 && (var_decl_global_scope == t->decl)) || 
              (varIsNotOnCurrentScope != -1 && (var_decl_current_scope == t->decl))) {
              semanticError(t, "variavel ja declarada anteriormente");
              st_insert(t->attr.name,t->lineno,0,t->decl,t->type, currentScope);
            } else {
              semanticError(t, "declaracao inválida");
              st_insert(t->attr.name,t->lineno,0,t->decl,t->type, currentScope);
            }
          }
          break;
        case FuncDeclK:
          if (st_lookup(t->attr.name, currentScope) == -1){
            /* not yet in table, so treat as new definition */
            st_insert(t->attr.name,t->lineno,location++,t->decl,t->type, "global");
            currentScope = t->attr.name;
          }
          else {
            /* already in table, so ignore location, 
             add line number of use only */ 
            semanticError(t, "funcao ja declarada anteriormente");
            st_insert(t->attr.name,t->lineno,0,t->decl, t->type, "global");
          }

          break;
        case ArrDeclK:
          if (st_lookup(t->attr.name, currentScope) == -1){
            /* not yet in table, so treat as new definition */
            st_insert(t->attr.name,t->lineno,location++,t->decl,t->type, currentScope);
          }
          else{
            /* already in table, so ignore location, 
             add line number of use only */ 
            semanticError(t, "variavel ja declarada anteriormente");
            st_insert(t->attr.name,t->lineno,0,t->decl,t->type, currentScope);
          }
          break;
        case IfK:
            // if
            // st_insert("IfK",t->lineno,location++,t->decl,t->type, currentScope);
          break;
        case RepeatK:
            // final do while
            // st_insert("RepeatK",t->lineno,location++,t->decl,t->type, currentScope);
          break;
        case WriteK:
            // nao sei o que eh
            // st_insert("WriteK",t->lineno,location++,t->decl,t->type, currentScope);
          break;
        case ActivK:
            // chamada de função
            // st_insert("ActivK",t->lineno,location++,t->decl,t->type, currentScope);
          break;
        case DeclK:
            // so eh declarado mas não usa em lugar nenhum
            // st_insert("DeclK",t->lineno,location++,t->decl,t->type, currentScope);
          break;
        case RetK:
            // retorno de funcao
            // st_insert("RetK",t->lineno,location++,t->decl,t->type, currentScope);
          break;
        case TypeK:
            // atribuicao de tipo, desnecessario, ja é colocado o tipo na declaração da variável
            // st_insert("TypeK",t->lineno,location++,t->decl,t->type, currentScope);
          break;
        default:
          break;
      }
      break;
    case ExpK:
      switch (t->kind.exp)
      { case OpK:
          // operacao qualquer de variavel
          // st_insert("OpK",t->lineno,location++,t->decl,t->type, currentScope);
          break;
        case ConstK:
          // comparacao ou atribuicao de variavel Ex: i=0, i == 0
          // st_insert("ConstK",t->lineno,location++,t->decl,t->type, currentScope);
          break;
        case IdK:
          // variavel usada sem ser a declaracao
          // fprintf(listing, "\n nome: %s linha: %d decl: %s type: %s scope:%s",t->attr.name, t->lineno,convertDeclToMessage(t->decl),convertTypeToMessage(t->type), currentScope);
          // st_insert("IdK",t->lineno,location++,t->decl,t->type, currentScope);
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
}

/* Function buildSymtab constructs the symbol 
 * table by preorder traversal of the syntax tree
 */
void buildSymtab(TreeNode * syntaxTree)
{ 
  traverse(syntaxTree,insertDecl,nullProc);
  traverse(syntaxTree,insertType,nullProc);
  traverse(syntaxTree,insertNode,nullProc);
  //traverse(syntaxTree,insertScope,nullProc);
  if (TraceAnalyze)
  { 
    printErrors(listing);
    fprintf(listing,"\nSymbol table:\n\n");
    printSymTab(listing);
  }
}

static void typeError(TreeNode * t, char * message)
{ fprintf(listing,"Type error at line %d: %s\n",t->lineno,message);
  Error = TRUE;
}

/* Procedure checkNode performs
 * type checking at a single tree node
 */
static void checkNode(TreeNode * t)
{ switch (t->nodekind)
  { case ExpK:
      switch (t->kind.exp)
      { case OpK:
          if ((t->child[0]->type != Integer) ||
              (t->child[1]->type != Integer))
            typeError(t,"Op applied to non-integer");
          if ((t->attr.op == EQ) || (t->attr.op == LT))
            t->type = Boolean;
          else
            t->type = Integer;
          break;
        case ConstK:
        case IdK:
          t->type = Integer;
          break;
        default:
          break;
      }
      break;
    case StmtK:
      switch (t->kind.stmt)
      { 
        // case IfK:
        //   if (t->child[0]->type == Integer)
        //     typeError(t->child[0],"if test is not Boolean");
        //   break;
        case AssignK:
          // fprintf(listing, "\n nome: %s linha: %d decl: %s type: %s scope:%s",
          // t->attr.name, t->lineno,convertDeclToMessage(t->decl),convertTypeToMessage(t->type), currentScope);
          // fprintf(listing, "\n filho nome: %s linha: %d decl: %s type: %s scope:%s",
          // t->child[0]->attr.name, t->child[0]->lineno,convertDeclToMessage(t->child[0]->decl),convertTypeToMessage(t->child[0]->type), currentScope);
          if (t->child[0]->type != Integer)
            typeError(t->child[0],"assignment of non-integer value");
          break;
        case WriteK:
          if (t->child[0]->type != Integer)
            typeError(t->child[0],"write of non-integer value");
          break;
        case RepeatK:
          if (t->child[1]->type == Integer)
            typeError(t->child[1],"repeat test is not Boolean");
          break;
        case ActivK:
          if (st_lookup(t->attr.name, currentScope) == -1 && st_lookup(t->attr.name, "global") == -1) {
            if (strcmp(t->attr.name, "input") == 0 || strcmp(t->attr.name, "output") == 0) {
              break;
            }
            typeError(t, "chamada de função não declarada");
          }
          break;
        case TypeK:
          if (strcmp(t->attr.name, "void") == 0 && t->child[0]->decl == 1) {
            typeError(t, "declaracao invalida de variavel");
          }
          break;

        default:
          break;
      }
      break;
    default:
      break;

  }
}

/* Procedure typeCheck performs type checking 
 * by a postorder syntax tree traversal
 */
void typeCheck(TreeNode * syntaxTree)
{ traverse(syntaxTree,nullProc,checkNode);
}
