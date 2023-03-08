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

/* Procedure emitComment prints a comment line
 * with comment c in the code file
 */
void printSubRoutine() {
  for (int i = 0; i < subRoutineLevel; i++) {
    fprintf(code, "\t");
  }
}

void emitComment(char *c)
{
  if (TraceCode) {
    printSubRoutine();
    fprintf(code, "* %s\n", c);
  }
    
}

void emitCommentWithLine(char *c, int line)
{
  if (TraceCode) {
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

void increaseSubroutineLevel() {
  subRoutineLevel++;  
}

void decreaseSubroutineLevel() {
  subRoutineLevel--;
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
  case EQ:
    return "==";
    break;
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
  fprintf(code, "%3d:  %5s  %d,%d,%d ", emitLoc++, op, r, s, t);
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
  fprintf(code, "%3d:  %5s  %d(%d) ", emitLoc++, op, r, s);
  if (TraceCode)
    fprintf(code, "\t%s", c);
  fprintf(code, "\n");
  if (highEmitLoc < emitLoc)
    highEmitLoc = emitLoc;
} /* emitRM */

void emitOpAssign(char *op)
{
  printSubRoutine();
  fprintf(code, "%3d:  t%d = ", emitLoc++, registerNumber);

  if (highEmitLoc < emitLoc)
    highEmitLoc = emitLoc;
  registerNumber++;
}

void emitAssign()
{
  printSubRoutine();
  fprintf(code, "%3d: ", emitLoc++);
  if (highEmitLoc < emitLoc)
    highEmitLoc = emitLoc;
}

int getRegisterNumber(){
  return registerNumber;
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
void emitRM_Abs(char *op, int r, int a, char *c)
{
  fprintf(code, "%3d:  %5s  %d,%d(%d) ",
          emitLoc, op, r, a - (emitLoc + 1), pc);
  ++emitLoc;
  if (TraceCode)
    fprintf(code, "\t%s", c);
  fprintf(code, "\n");
  if (highEmitLoc < emitLoc)
    highEmitLoc = emitLoc;
} /* emitRM_Abs */
