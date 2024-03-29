/****************************************************/
/* File: code.c                                     */
/* TM Code emitting utilities                       */
/* implementation for the TINY compiler             */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#include "globals.h"
#include <time.h>
#include "code.h"

/* TM location number for current instruction emission */
static int emitLoc = 0;

/* Highest TM location emitted so far
   For use in conjunction with emitSkip,
   emitBackup, and emitRestore */
static int highEmitLoc = 0;

static int highRegisterNumber = 0;

static int registerNumber = 0;

static int subRoutineLevel = 0;

static int deviationLevel = 0;

/* Procedure emitComment prints a comment line
 * with comment c in the code file
 */
void printSubRoutine()
{
  for (int i = 0; i < subRoutineLevel; i++)
  {
    fprintf(code, "\t");
  }
}

void emitComment(char *c)
{
  if (TraceCode)
  {
    printSubRoutine();
    fprintf(code, "* %s\n", c);
  }
}

void emitCommentNodeKind(TreeNode *tree)
{
  switch (tree->nodekind)
  {
  case StmtK:

    switch (tree->kind.stmt)
    {

    case IfK:
      if (TraceCode)
        emitCommentWithLine("-> if", tree->lineno);
      break; /* if_k */

    case RepeatK:
      if (TraceCode)
        emitCommentWithLine("-> repeat", tree->lineno);
      break; /* repeat */

    case AssignK:
      if (TraceCode)
        emitCommentWithLine("-> assign", tree->lineno);
      break; /* assign_k */
    case DeclK:
      if (TraceCode)
        emitCommentWithLine("-> decl", tree->lineno);
      break;
    case ActivK:
      if (TraceCode)
        emitCommentWithLine("-> activ", tree->lineno);
      break;
    case RetK:
      if (TraceCode)
        emitCommentWithLine("-> ret", tree->lineno);
      break;
    case TypeK:
      if (TraceCode)
        emitCommentWithLine("-> type", tree->lineno);

      break;
    case VarDeclK:
      if (TraceCode)
      {
        emitCommentWithLine("-> vardecl", tree->lineno);
      }

      break;
    case FuncDeclK:
      if (TraceCode)
      {
        emitCommentWithLine("-> funcDecl", tree->lineno);
        emitComment(tree->attr.name);
      }
      break;
    case ArrDeclK:
      if (TraceCode)
        emitCommentWithLine("-> arrDecl", tree->lineno);
      break;
    case WriteK:
      if (TraceCode)
        emitCommentWithLine("-> write", tree->lineno);
      break;
    default:
      if (TraceCode)
        emitCommentWithLine("-> break", tree->lineno);
      break;
    }
    break;
  case ExpK:

    switch (tree->kind.exp)

    {
    case ConstK:
      if (TraceCode)
        emitCommentWithLine("-> Const", tree->lineno);
      /* gen code to load integer constant using LDC */
      break; /* ConstK */

    case IdK:
      if (TraceCode)
        emitCommentWithLine("-> Id", tree->lineno);
      break; /* IdK */

    case OpK:
      if (TraceCode)
        emitCommentWithLine("-> Op", tree->lineno);
      break; /* OpK */

    default:
      break;
    }
    break;
  default:
    break;
  }
}

void emitCommentWithLine(char *c, int line)
{
  if (TraceCode)
  {
    printSubRoutine();
    fprintf(code, "* %s line: %d\n", c, line);
  }
}

void emitTimeOfCompilation()
{
  if (TraceCode)
  {
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    fprintf(code, "* TIME OF COMPILATION: %s\n", asctime(timeinfo));
  }
}

void increaseSubroutineLevel()
{
  subRoutineLevel++;
}

void decreaseSubroutineLevel()
{
  subRoutineLevel--;
}

void printDeviation()
{
  for (int i = 0; i < deviationLevel; i++)
  {
    fprintf(code, "\t");
  }
}

void emitDeviationAssign()
{
  printSubRoutine();
  fprintf(code, " t%d = ", registerNumber);

  if (highEmitLoc < emitLoc)
    highEmitLoc = emitLoc;
  registerNumber++;
  deviationLevel++;
}

void emitIf()
{
  printSubRoutine();
  fprintf(code, "if_true t%d goto L%d \n", registerNumber - 1, deviationLevel);
  if (highEmitLoc < emitLoc)
    highEmitLoc = emitLoc;
}

void emitElse()
{
  printSubRoutine();
  fprintf(code, "goto L%d \n", deviationLevel + 1);

  if (highEmitLoc < emitLoc)
    highEmitLoc = emitLoc;
}

void emitDeviation()
{
  printSubRoutine();
  fprintf(code, " L%d: \n", deviationLevel);
  deviationLevel++;
  if (highEmitLoc < emitLoc)
    highEmitLoc = emitLoc;
}

void emitWhileDeviation()
{
  printSubRoutine();
  fprintf(code, "L%d: t%d = ", deviationLevel + 1, registerNumber);

  if (highEmitLoc < emitLoc)
    highEmitLoc = emitLoc;
  registerNumber++;
  deviationLevel++;
}

void emitWhile()
{
  printSubRoutine();
  fprintf(code, "if_true t%d goto L%d \n", registerNumber - 1, deviationLevel + 1);
  if (highEmitLoc < emitLoc)
    highEmitLoc = emitLoc;
  deviationLevel++;
}

void emitEndWhile()
{
  printSubRoutine();
  fprintf(code, "goto L%d \n", deviationLevel - 1);

  printSubRoutine();
  fprintf(code, "L%d:\n", deviationLevel);

  if (highEmitLoc < emitLoc)
    highEmitLoc = emitLoc;
}

char *getOpChar(TreeNode *tree)
{
  switch (tree->attr.op)
  {
  case PLUS:
    return "+";
    break;
  case MINUS:
    return "-";
    break;
  case TIMES:
    return "*";
    break;
  case OVER:
    return "/";
    break;
  case LT:
    return "<";
    break;
  case GT:
    return ">";
    break;
  case GEQ:
    return ">=";
    break;
  case LEQ:
    return "<=";
    break;
  case EQEQ:
    return "==";
    break;
  case INEQ:
    return "!=";
    break;
  default:
    return "BUG: Unknown operator";
    break;
  }
}

char *getOpOpositeChar(TreeNode *tree)
{
  switch (tree->attr.op)
  {
  case LT:
    return ">=";
    break;
  case GT:
    return "<=";
    break;
  case GEQ:
    return "<";
    break;
  case LEQ:
    return ">";
    break;
  case EQEQ:
    return "==";
    break;
  case INEQ:
    return "==";

  default:
    return "BUG: Unknown operator";
    break;
  }
}

/* Procedure emitRO emits a register-only
 * TM instruction
 * op = the opcode
 * r = target register
 * s = 1st source register
 * t = 2nd source register
 * c = a comment to be printed if TraceCode is TRUE
 */
void emitRO(char *op, int r, int s, int t, char *c)
{
  printSubRoutine();
  fprintf(code, " %5s  %d,%d,%d ", op, r, s, t);
  if (TraceCode)
    fprintf(code, "\t%s", c);
  fprintf(code, "\n");
  if (highEmitLoc < emitLoc)
    highEmitLoc = emitLoc;
} /* emitRO */

/* Procedure emitRM emits a register-to-memory
 * TM instruction
 * op = the opcode
 * r = target register
 * d = the offset
 * s = the base register
 * c = a comment to be printed if TraceCode is TRUE
 */
void emitRM(char *op, int r, int s, char *c)
{
  printSubRoutine();
  fprintf(code, " %5s  %d(%d) ", op, r, s);
  if (TraceCode)
    fprintf(code, "\t%s", c);
  fprintf(code, "\n");
  if (highEmitLoc < emitLoc)
    highEmitLoc = emitLoc;
} /* emitRM */

void emitOpAssign(char *op)
{
  printSubRoutine();
  fprintf(code, "t%d = ", registerNumber);

  if (highEmitLoc < emitLoc)
    highEmitLoc = emitLoc;
  registerNumber++;
}

void emitAssign()
{
  printSubRoutine();
  if (highEmitLoc < emitLoc)
    highEmitLoc = emitLoc;
}

void emitArrayAssign(TreeNode *tree)
{
  fprintf(code, "t%d = ", registerNumber);

  if (tree->child[0]->kind.exp == IdK)
  {
    fprintf(code, "%s", tree->child[0]->attr.name);
  }
  else if (tree->child[0]->kind.exp == ConstK)
  {
    fprintf(code, "%d", tree->child[0]->attr.val);
  }

  fprintf(code, " * 4 \n");
  printSubRoutine();
  fprintf(code, "%s[t%d] = ", tree->attr.name, registerNumber);
  registerNumber++;
  if (highEmitLoc < emitLoc)
    highEmitLoc = emitLoc;
}

void emitArrayAtribution(TreeNode *tree)
{
  printSubRoutine();
  fprintf(code, "t%d = ", registerNumber);

  if (tree->child[0]->kind.exp == IdK)
  {
    fprintf(code, "%s", tree->child[0]->attr.name);
  }
  else if (tree->child[0]->kind.exp == ConstK)
  {
    fprintf(code, "%d", tree->child[0]->attr.val);
  }

  fprintf(code, " * 4 \n");
  registerNumber++;
  if (highEmitLoc < emitLoc)
    highEmitLoc = emitLoc;
}


int getRegisterNumber()
{
  return registerNumber;
}

int printNumParams(TreeNode *tree)
{
  TreeNode *p;
  int numParams = 0;
  p = tree->child[0];
  while (p != NULL && (p->kind.stmt == IdK || p->kind.stmt == ActivK || p->kind.exp == ConstK))
  {
    printSubRoutine();
    if (p->kind.exp == ConstK) {
      fprintf(code, "param %d\n", p->attr.val);
    }
    else {
      fprintf(code, "param %s\n", p->attr.name);
    }
    

    p = p->sibling;
    numParams++;
  }
  return numParams;
}

/* Function emitSkip skips "howMany" code
 * locations for later backpatch. It also
 * returns the current code position
 */
int emitSkip(int howMany)
{
  int i = emitLoc;
  emitLoc += howMany;
  if (highEmitLoc < emitLoc)
    highEmitLoc = emitLoc;
  return i;
} /* emitSkip */

/* Procedure emitBackup backs up to
 * loc = a previously skipped location
 */
void emitBackup(int loc)
{
  if (loc > highEmitLoc)
    emitComment("BUG in emitBackup");
  emitLoc = loc;
} /* emitBackup */

/* Procedure emitRestore restores the current
 * code position to the highest previously
 * unemitted position
 */
void emitRestore(void)
{
  emitLoc = highEmitLoc;
}

/* Procedure emitRM_Abs converts an absolute reference
 * to a pc-relative reference when emitting a
 * register-to-memory TM instruction
 * op = the opcode
 * r = target register
 * a = the absolute location in memory
 * c = a comment to be printed if TraceCode is TRUE
 */