/****************************************************/
/* File: code.h                                     */
/* Code emitting utilities for the TINY compiler    */
/* and interface to the TM machine                  */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#ifndef _CODE_H_
#define _CODE_H_

/* pc = program counter  */
#define pc 7

/* mp = "memory pointer" points
 * to top of memory (for temp storage)
 */
#define mp 6

/* gp = "global pointer" points
 * to bottom of memory for (global)
 * variable storage
 */
#define gp 5

/* accumulator */
#define ac 0

/* 2nd accumulator */
#define ac1 1

/* code emitting utilities */

/* Procedure emitComment prints a comment line
 * with comment c in the code file
 */
void emitComment(char *c);

void emitCommentWithLine(char *c, int line);

void emitTimeOfCompilation();

char *getOpChar(TreeNode *tree);

char *getOpOpositeChar(TreeNode *tree);
/* Procedure emitRO emits a register-only
 * TM instruction
 * op = the opcode
 * r = target register
 * s = 1st source register
 * t = 2nd source register
 * c = a comment to be printed if TraceCode is TRUE
 */
void emitRO(char *op, int r, int s, int t, char *c);

/* Procedure emitRM emits a register-to-memory
 * TM instruction
 * op = the opcode
 * r = target register
 * d = the offset
 * s = the base register
 * c = a comment to be printed if TraceCode is TRUE
 */
void emitRM(char *op, int r, int s, char *c);

/* Function emitSkip skips "howMany" code
 * locations for later backpatch. It also
 * returns the current code position
 */
int emitSkip(int howMany);

/* Procedure emitBackup backs up to
 * loc = a previously skipped location
 */
void emitBackup(int loc);

/* Procedure emitRestore restores the current
 * code position to the highest previously
 * unemitted position
 */
void emitRestore(void);

void emitOpAssign(char *op);

int getRegisterNumber(void);

void emitAssign(void);

void increaseSubroutineLevel();

void decreaseSubroutineLevel();

void printSubRoutine();

void emitCommentNodeKind(TreeNode *tree);

int printNumParams(TreeNode *tree);

void emitDeviationAssign();

void emitIf();

void emitElse();

void emitDeviation();

void emitWhileDeviation();

void emitWhile();

void emitEndWhile();

void emitArrayAssign(TreeNode *tree);

void emitArrayAtribution(TreeNode *tree);

#endif
