/****************************************************/
/* File: cgen.c                                     */
/* The code generator implementation                */
/* for the TINY compiler                            */
/* (generates code for the TM machine)              */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#include "globals.h"
#include "symtab.h"
#include "code.h"
#include "cgen.h"

/* tmpOffset is the memory offset for temps
   It is decremented each time a temp is
   stored, and incremeted when loaded again
*/
static int tmpOffset = 0;

int regnum = 1;

/* prototype for internal recursive code generator */
static void cGen (TreeNode * tree);
TreeNode * p1;
TreeNode * p2;

/*******************************/


/* Variable indentno is used by printTreeGen to
 * store current number of spaces to indent
 */
static int indentno = 0;

/* macros to increase/decrease indentation */
#define INDENT indentno+=2
#define UNINDENT indentno-=2

/* printSpaces indents by printing spaces */
static void printSpaces(void)
{ int i;
  for (i=0;i<indentno;i++)
    fprintf(listing," ");
}


void printTokenGen( TokenType token, const char* tokenString )
{ switch (token)
  { case IF:
    case ELSE:
    case INT:
    case RETURN:
    case VOID:
    case EMPTY:
    case WHILE: break;
    case EQ: fprintf(listing,"=\n"); break;
    case LT: fprintf(listing,"<\n"); break;
    case GT: fprintf(listing,">\n"); break;
    case GEQ: fprintf(listing,">=\n"); break;
    case LEQ: fprintf(listing,"<=\n"); break;
    case EQEQ: fprintf(listing,"==\n"); break;
    case INEQ: fprintf(listing,"!=\n"); break;
    case PLUS: fprintf(listing,"+\n"); break;
    case COMMA: fprintf(listing,",\n"); break;
    case MINUS: fprintf(listing,"-\n"); break;
    case TIMES: fprintf(listing,"*\n"); break;
    case OVER: fprintf(listing,"/\n"); break;
    case LPAREN: fprintf(listing,"(\n"); break;
    case RPAREN: fprintf(listing,")\n"); break;
    case LBRACKETS: fprintf(listing,"[\n"); break;
    case RBRACKETS: fprintf(listing,"]\n"); break;
    case LCBRACES: fprintf(listing,"{\n"); break;
    case RCBRACES: fprintf(listing,"}\n"); break;
    case SEMI: fprintf(listing,";\n"); break;
    case ENDFILE: fprintf(listing,"EOF\n"); break;
    case NUM:
      fprintf(listing,
          "NUM, val= %s\n",tokenString);
      break;
    case ID:
      fprintf(listing,
          "ID, name= %s\n",tokenString);
      break;
    case ERROR:
      fprintf(listing,
          "ERROR: %s\n",tokenString);
      break;
    default: /* should never happen */
      fprintf(listing,"Unknown token: %d\n",token);
  }
}

void printTreeGen( TreeNode * tree )
{ int i;
  INDENT;
  while (tree != NULL) {
    printSpaces();
    if (tree->nodekind==StmtK)
    { switch (tree->kind.stmt) {
        case IfK:
          fprintf(listing,"If\n");
          break;
        case RepeatK:
          fprintf(listing,"Repeat\n");
          break;
        case AssignK:

          
          p1 = tree->child[0];
          p2 = tree->child[1];

          if(p2->nodekind==ExpK && p2->kind.exp==OpK){
            printTreeGen(p2);
         }
          else fprintf(listing,"%s = %d\n",tree->child[0]->attr.name, p1->attr.val);
        
/*           p1 = tree->child[1];
          p2 = p1->child[0];
          fprintf(listing,"Assign to: %s = %s\n",tree->attr.name, tree->attr.name);
          printTreeGen(p1); */
          break;
        case WriteK:
          fprintf(listing,"Write\n");
          break;
        case ActivK:
          fprintf(listing,"Function: %s\n", tree->attr.name);
          break;
        case RetK:
          fprintf(listing,"Return\n");
          break;
        case TypeK:
          fprintf(listing,"Type: %s\n", tree->attr.name);
          break;
        case FuncDeclK:
          fprintf(listing,"FuncDecl: %s\n", tree->attr.name);
          break;
        case VarDeclK:
          fprintf(listing,"VarDecl: %s\n", tree->attr.name);
          break;
        case ArrDeclK:
          fprintf(listing,"ArrDecl: %s\n", tree->attr.name);
          break;
        default:
          fprintf(listing,"Unknown ExpNode kind\n");
          break;
      }
    }
    else if (tree->nodekind==ExpK)
    { switch (tree->kind.exp) {
        case OpK:
          /*if(tree->child[0]->kind.exp==OpK && tree->child[1]->kind.exp==OpK){
            fprintf(listing,"t%d = ", regnum);
            printTreeGen(tree->child[0]);
            fprintf(listing,"t%d = ", regnum+1);
            printTreeGen(tree->child[1]);
            regnum += 2;
            fprintf(listing,"t%d + t%d", regnum-2, regnum-1);
          }
          else*/ if(tree->child[0]->kind.exp==OpK){
            //fprintf(listing,"###t%d = ", regnum);
            printTreeGen(tree->child[0]);
            regnum += 1;
            fprintf(listing,"###t%d = t%d + %d", regnum, regnum-1, tree->child[1]->attr.val);
          }
          /*else if(tree->child[1]->kind.exp==OpK){
            fprintf(listing,"t%d = ", regnum);
            int v = tree->child[0]->attr.val;
            printTreeGen(tree->child[1]);
            regnum += 1;
            fprintf(listing,"%d + t%d", v, regnum-1);
          }*/
          else
            fprintf(listing,"t%d =%d + %d", regnum, tree->child[0]->attr.val, tree->child[1]->attr.val);


          //fprintf(listing,"Op: ");
          //printTokenGen(tree->attr.op,"\0");
          break;
        case ConstK:
          //fprintf(listing,"Const: %d\n",tree->attr.val);
          break;
        case IdK:
          fprintf(listing,"Id: %s\n",tree->attr.name);
          break;
        default:
          fprintf(listing,"Unknown ExpNode kind\n");
          break;
      }
    }
    else fprintf(listing,"Unknown node kind\n");
    for (i=0;i<MAXCHILDREN;i++)
         printTreeGen(tree->child[i]);
    tree = tree->sibling;
  }
  UNINDENT;
}


/****************/

/* Procedure genStmt generates code at a statement node */
static void genStmt( TreeNode * tree)
{ 
  emitComment("STMTTTTTTTTTTTTTTT"); 
  TreeNode * p1, * p2, * p3;
  int savedLoc1,savedLoc2,currentLoc;
  int loc;
  switch (tree->kind.stmt) {

      case IfK :
         emitComment("IF"); 
         if (TraceCode) emitComment("-> if") ;
         p1 = tree->child[0] ;
         p2 = tree->child[1] ;
         p3 = tree->child[2] ;
         /* generate code for test expression */
         cGen(p1);
         savedLoc1 = emitSkip(1) ;
         emitComment("if: jump to else belongs here");
         /* recurse on then part */
         cGen(p2);
         savedLoc2 = emitSkip(1) ;
         emitComment("if: jump to end belongs here");
         currentLoc = emitSkip(0) ;
         emitBackup(savedLoc1) ;
         emitRM_Abs("JEQ",ac,currentLoc,"if: jmp to else");
         emitRestore() ;
         /* recurse on else part */
         cGen(p3);
         currentLoc = emitSkip(0) ;
         emitBackup(savedLoc2) ;
         emitRM_Abs("LDA",pc,currentLoc,"jmp to end") ;
         emitRestore() ;
         if (TraceCode)  emitComment("<- if") ;
         break; /* if_k */

      case RepeatK:
         emitComment("REPEAT");
         if (TraceCode) emitComment("-> repeat") ;
         p1 = tree->child[0] ;
         p2 = tree->child[1] ;
         savedLoc1 = emitSkip(0);
         emitComment("repeat: jump after body comes back here");
         /* generate code for body */
         cGen(p1);
         /* generate code for test */
         cGen(p2);
         emitRM_Abs("JEQ",ac,savedLoc1,"repeat: jmp back to body");
         if (TraceCode)  emitComment("<- repeat") ;
         break; /* repeat */

      case AssignK:
         fprintf(listing,"#####Assign to: %s\n",tree->attr.name);
         emitComment("ASSIGN");
         if (TraceCode) emitComment("-> assign") ;
         /* generate code for rhs */
         cGen(tree->child[0]);
         /* now store value */
         loc = st_lookup(tree->attr.name);
         emitRM("ST",ac,loc,gp,"assign: store value");
         if (TraceCode)  emitComment("<- assign") ;
         break; /* assign_k */
      case WriteK:
         emitComment("WRITE");
         /* generate code for expression to write */
         cGen(tree->child[0]);
         /* now output it */
         emitRO("OUT",ac,0,0,"write ac");
         break;
      case ActivK :
         emitComment("Activ"); 
         p1 = tree->child[0] ;
         cGen(p1);
         break;
      case DeclK :
         emitComment("Decl"); 
         p1 = tree->child[0] ;
         cGen(p1);
         break;
      case RetK:
         emitComment("RET");
         emitRO("IN",ac,0,0,"read integer value");
         loc = st_lookup(tree->attr.name);
         emitRM("ST",ac,loc,gp,"read: store value");
         break;
      case TypeK :
         emitComment("TYPE");
         p1 = tree->child[0] ;
         cGen(p1);
         break;
      case VarDeclK :
         emitComment("VarDecl");
         fprintf(listing,"#####VarDecl to: %s\n",tree->attr.name);
         p1 = tree->child[0] ;
         cGen(p1);
         break;
      case FuncDeclK :
         emitComment("FuncDecl");
         fprintf(listing,"#####FuncDecl to: %s\n",tree->attr.name);
         for (int i=0;i<MAXCHILDREN;i++){
               printf("AAAAAA");
               cGen(tree->child[i]);
         }
         break;
      case ArrDeclK :
         emitComment("ArrDecl"); 
         p1 = tree->child[0] ;
         cGen(p1);
         break;
      default:
         break;
    }
} /* genStmt */

/* Procedure genExp generates code at an expression node */
static void genExp( TreeNode * tree)
{ 
  emitComment("EXPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP"); 
  int loc;
  TreeNode * p1, * p2;
  switch (tree->kind.exp) {

    case ConstK :
      emitComment("Const"); 
      if (TraceCode) emitComment("-> Const") ;
      /* gen code to load integer constant using LDC */
      emitRM("LDC",ac,tree->attr.val,0,"load const");
      if (TraceCode)  emitComment("<- Const") ;
      break; /* ConstK */
    
    case IdK :
      emitComment("Id");
      if (TraceCode) emitComment("-> Id") ;
      loc = st_lookup(tree->attr.name);
      emitRM("LD",ac,loc,gp,"load id value");
      if (TraceCode)  emitComment("<- Id") ;
      break; /* IdK */

    case OpK :
         emitComment("Op");
         if (TraceCode) emitComment("-> Op") ;
         p1 = tree->child[0];
         p2 = tree->child[1];
         /* gen code for ac = left arg */
         cGen(p1);
         /* gen code to push left operand */
         emitRM("ST",ac,tmpOffset--,mp,"op: push left");
         /* gen code for ac = right operand */
         cGen(p2);
         /* now load left operand */
         emitRM("LD",ac1,++tmpOffset,mp,"op: load left");
         switch (tree->attr.op) {
            case PLUS :
               emitRO("ADD",ac,ac1,ac,"op +");
               break;
            case MINUS :
               emitRO("SUB",ac,ac1,ac,"op -");
               break;
            case TIMES :
               emitRO("MUL",ac,ac1,ac,"op *");
               break;
            case OVER :
               emitRO("DIV",ac,ac1,ac,"op /");
               break;
            case LT :
               emitRO("SUB",ac,ac1,ac,"op <") ;
               emitRM("JLT",ac,2,pc,"br if true") ;
               emitRM("LDC",ac,0,ac,"false case") ;
               emitRM("LDA",pc,1,pc,"unconditional jmp") ;
               emitRM("LDC",ac,1,ac,"true case") ;
               break;
            case EQ :
               emitRO("SUB",ac,ac1,ac,"op ==") ;
               emitRM("JEQ",ac,2,pc,"br if true");
               emitRM("LDC",ac,0,ac,"false case") ;
               emitRM("LDA",pc,1,pc,"unconditional jmp") ;
               emitRM("LDC",ac,1,ac,"true case") ;
               break;
            default:
               emitComment("BUG: Unknown operator");
               break;
         } /* case op */
         if (TraceCode)  emitComment("<- Op") ;
         break; /* OpK */

    default:
      break;
  }
} /* genExp */

/* Procedure cGen recursively generates code by
 * tree traversal
 */
static void cGen( TreeNode * tree)
{ if (tree != NULL)
  { switch (tree->nodekind) {
      case StmtK:
        genStmt(tree);
        break;
      case ExpK:
        genExp(tree);
        break;
      default:
        break;
    }
    cGen(tree->sibling);
  }
}

/**********************************************/
/* the primary function of the code generator */
/**********************************************/
/* Procedure codeGen generates code to a code
 * file by traversal of the syntax tree. The
 * second parameter (codefile) is the file name
 * of the code file, and is used to print the
 * file name as a comment in the code file
 */
void codeGen(TreeNode * syntaxTree, char * codefile)
{  char * s = malloc(strlen(codefile)+7);
   strcpy(s,"File: ");
   strcat(s,codefile);
   emitComment("TINY Compilation to TM Code");
   emitComment(s);
   /* generate standard prelude */
   emitComment("Standard prelude:");
   emitRM("LD",mp,0,ac,"load maxaddress from location 0");
   emitRM("ST",ac,0,ac,"clear location 0");
   emitComment("End of standard prelude.");
   /* generate code for TINY program */
   printTreeGen(syntaxTree);
   /* finish */
   emitComment("End of execution.");
   emitRO("HALT",0,0,0,"");
}



