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

/* prototype for internal recursive code generator */
static void cGen(TreeNode *tree);

// static void iterateFunction(TreeNode *tree)
// {
//   if (tree != NULL)
//   {
//     emitComment("iterateFunctionNotNull");
//     switch (tree->kind.stmt)
//     {
//     // case TypeK:
//     //   if (TraceCode)
//     //     emitComment("-> typeK função");
//     //   iterateFunction(tree->sibling);
//     //   break;
//     default:
//       if (TraceCode)
//         emitComment("-> break");
//       break;
//     }

//   }
// }

static int cGenAssign(TreeNode *tree)
{
  TreeNode *p1, *p2, *p3;
  int firstRegister, secondRegister;
  if (tree != NULL)
  {
    switch (tree->nodekind)
    {
    case StmtK:
      switch (tree->kind.stmt)
      {
      default:
        if (TraceCode)
          emitComment("-> break cgenStmtk");
        break;
      }
      break;
    case ExpK:
      switch (tree->kind.exp)
      {
      case OpK:
        emitComment("Opk");
        emitComment(getOpChar(tree));
        p1 = tree->child[0];
        p2 = tree->child[1];
        firstRegister = cGenAssign(p1);
        secondRegister = cGenAssign(p2);
        emitOpAssign(getOpChar(tree));
        if (p1->kind.exp == IdK)
        {
          fprintf(code, "%s", p1->attr.name);
        }
        else if (p1->kind.exp == ConstK)
        {
          fprintf(code, "%d", p1->attr.val);
        }
        else
        {
          fprintf(code, "t%d", firstRegister - 1);
        }

        fprintf(code, " %s ", getOpChar(tree));

        if (p2->kind.exp == IdK)
        {
          fprintf(code, "%s", p2->attr.name);
        }
        else if (p2->kind.exp == ConstK)
        {
          fprintf(code, "%d", p2->attr.val);
        }
        else
        {
          fprintf(code, "t%d", secondRegister - 1);
        }
        fprintf(code, "\n");
        break;
      case IdK:
        emitCommentWithLine("Idk", tree->lineno);
        emitCommentWithLine(tree->attr.name, tree->lineno);
        p1 = tree->child[0];
        p2 = tree->child[1];
        firstRegister = cGenAssign(p1);
        secondRegister = cGenAssign(p2);
        break;
      case ConstK:
        emitCommentWithLine("ConstK", tree->lineno);
        break;
      default:
        if (TraceCode)
          emitComment("-> break cgenExpk");
        break;
      }
      break;
    default:
      if (TraceCode)
        emitComment("-> break cGen");
      break;
    }
  }
  return getRegisterNumber();
}

/* Procedure genStmt generates code at a statement node */
static void genStmt(TreeNode *tree)
{
  TreeNode *p1, *p2, *p3;
  int savedLoc1, savedLoc2, currentLoc;
  int loc;
  switch (tree->kind.stmt)
  {

  case IfK:
    if (TraceCode)
      emitComment("-> if");
    // p1 = tree->child[0];
    // p2 = tree->child[1];
    // p3 = tree->child[2];
    // /* generate code for test expression */
    // cGen(p1);
    // savedLoc1 = emitSkip(1);
    // emitComment("if: jump to else belongs here");
    // /* recurse on then part */
    // cGen(p2);
    // savedLoc2 = emitSkip(1);
    // emitComment("if: jump to end belongs here");
    // currentLoc = emitSkip(0);
    // emitBackup(savedLoc1);
    // emitRM_Abs("JEQ", ac, currentLoc, "if: jmp to else");
    // emitRestore();
    // /* recurse on else part */
    // cGen(p3);
    // currentLoc = emitSkip(0);
    // emitBackup(savedLoc2);
    // emitRM_Abs("LDA", pc, currentLoc, "jmp to end");
    // emitRestore();
    // if (TraceCode)
    //   emitComment("<- if");
    break; /* if_k */

  case RepeatK:
    if (TraceCode)
      emitComment("-> repeat");
    p1 = tree->child[0];
    p2 = tree->child[1];
    savedLoc1 = emitSkip(0);
    emitComment("repeat: jump after body comes back here");
    /* generate code for body */
    cGen(p1);
    /* generate code for test */
    cGen(p2);
    emitRM_Abs("JEQ", ac, savedLoc1, "repeat: jmp back to body");
    if (TraceCode)
      emitComment("<- repeat");
    break; /* repeat */

  case AssignK:
    if (TraceCode)
      emitCommentWithLine("-> assign", tree->lineno);
    // /* generate code for rhs */
    int first, second;
    p1 = tree->child[0];
    p2 = tree->child[1];
    first = cGenAssign(p1);
    second = cGenAssign(p2);
    // /* now store value */
    // emitAssign("assign value");
    if (TraceCode)
      emitCommentWithLine("<- assign", tree->lineno);
    break; /* assign_k */
  case DeclK:
    if (TraceCode)
      emitComment("-> decl");
    break;
  case ActivK:
    if (TraceCode)
      emitComment("-> activ");
    break;
  case RetK:
    if (TraceCode)
      emitComment("-> ret");
    break;
  case TypeK:
    if (TraceCode)
      emitComment("-> type");
    p1 = tree->child[0];
    if (p1->kind.stmt == FuncDeclK)
    {
      cGen(p1);
    }

    break;
  case VarDeclK:
    if (TraceCode)
    {
      emitComment("-> vardecl");
      emitComment(tree->attr.name);
    }

    break;
  case FuncDeclK:
    if (TraceCode)
    {
      emitComment("-> funcDecl");
      emitComment(tree->attr.name);
    }
    p1 = tree->child[0];
    // check case foi void main(void)
    if (tree->child[0] == NULL)
    {
      p1 = tree->child[1];
    }
    cGen(p1);
    break;
  case ArrDeclK:
    if (TraceCode)
      emitComment("-> arrDecl");
    break;
  case WriteK:
    if (TraceCode)
      emitComment("-> write");
    break;
    // case ReadK:
    //    emitRO("IN",ac,0,0,"read integer value");
    //    loc = st_lookup(tree->attr.name);
    //    emitRM("ST",ac,loc,gp,"read: store value");
    //    break;
    // case WriteK:
    //    /* generate code for expression to write */
    //    cGen(tree->child[0]);
    //    /* now output it */
    //    emitRO("OUT",ac,0,0,"write ac");
    //    break;

  default:
    if (TraceCode)
      emitComment("-> break");
    break;
  }
} /* genStmt */

/* Procedure genExp generates code at an expression node */
static void genExp(TreeNode *tree)
{
  int loc;
  TreeNode *p1, *p2;
  switch (tree->kind.exp)

  {
  case ConstK:
    if (TraceCode)
      emitComment("-> Const");
    /* gen code to load integer constant using LDC */
    emitRM("LDC", ac, tree->attr.val, "load const");
    if (TraceCode)
      emitComment("<- Const");
    break; /* ConstK */

  case IdK:
    if (TraceCode)
      emitComment("-> Id");
    emitRM("LD", ac, gp, "load id value");
    if (TraceCode)
      emitComment("<- Id");
    break; /* IdK */

  case OpK:
    if (TraceCode)
      emitComment("-> Op");
    p1 = tree->child[0];
    p2 = tree->child[1];
    /* gen code for ac = left arg */
    cGen(p1);
    /* gen code to push left operand */
    emitRM("ST", ac, mp, "op: push left");
    /* gen code for ac = right operand */
    cGen(p2);
    /* now load left operand */
    emitRM("LD", ac1, mp, "op: load left");
    switch (tree->attr.op)
    {
    case PLUS:
      emitRO("ADD", ac, ac1, ac, "op +");
      break;
    case MINUS:
      emitRO("SUB", ac, ac1, ac, "op -");
      break;
    case TIMES:
      emitRO("MUL", ac, ac1, ac, "op *");
      break;
    case OVER:
      emitRO("DIV", ac, ac1, ac, "op /");
      break;
    case LT:
      emitRO("SUB", ac, ac1, ac, "op <");
      emitRM("JLT", ac, pc, "br if true");
      emitRM("LDC", ac, ac, "false case");
      emitRM("LDA", pc, pc, "unconditional jmp");
      emitRM("LDC", ac, ac, "true case");
      break;
    case EQ:
      emitRO("SUB", ac, ac1, ac, "op ==");
      emitRM("JEQ", ac, pc, "br if true");
      emitRM("LDC", ac, ac, "false case");
      emitRM("LDA", pc, pc, "unconditional jmp");
      emitRM("LDC", ac, ac, "true case");
      break;
    default:
      emitComment("BUG: Unknown operator");
      break;
    } /* case op */
    if (TraceCode)
      emitComment("<- Op");
    break; /* OpK */

  default:
    break;
  }
} /* genExp */

/* Procedure cGen recursively generates code by
 * tree traversal
 */
static void cGen(TreeNode *tree)
{
  if (tree != NULL)
  {
    switch (tree->nodekind)
    {
    case StmtK:
      emitComment("Stmk");
      genStmt(tree);
      break;
    case ExpK:
      emitComment("Expk");
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
void codeGen(TreeNode *syntaxTree, char *codefile)
{
  char *s = malloc(strlen(codefile) + 7);
  strcpy(s, "File: ");
  strcat(s, codefile);
  emitComment("CMINUS COMPILATION");
  emitComment(s);
  emitTimeOfCompilation();
  /* generate standard prelude */
  emitComment("Standard prelude:");
  emitRM("LD", mp, ac, "load maxaddress from location 0");
  emitRM("ST", ac, ac, "clear location 0");
  emitComment("End of standard prelude.");
  /* generate code for TINY program */
  cGen(syntaxTree);
  /* finish */
  emitComment("End of execution.");
  emitRO("HALT", 0, 0, 0, "");
}
