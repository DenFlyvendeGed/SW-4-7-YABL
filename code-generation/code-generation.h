#ifndef YABL_CODE_GENERATION
#define YABL_CODE_GENERATION
#include <stdlib.h>
#include <stdio.h>

void cgScope(Scope* self, FILE* writer);
void cgStmt(Stmt* self, FILE* writer);
void cgAssign(Assign* self, FILE* writer);
void cgIdMutation(IdMutation* self, FILE* writer);
void cgExpr(Expr* self, FILE* writer);
void cgStart(Repeatable* tree, FILE* writer);
void cgFuncs(Funcs* self, FILE* writer);
void cgEvent(Event* self, FILE* writer);
void cgIfStmt(IfStmt* self, FILE* writer);
void cgRepeatStmt(Repeat* self, FILE* writer);
void cgTimesLoop(TimesLoop* self, FILE* writer);
void cgForLoop(ForLoop* self, FILE* writer);
void cgWhileLoop(WhileLoop* self, FILE* writer);
void cgRepeatLoop(RepeatLoop* self, FILE* writer);
void cgInitialization(Initialization* self, FILE* writer);
void cgType(Type* self, FILE* writer);
void cgId(Id* self, FILE* writer);
void cgReturnStmt(ReturnStmt* self, FILE* writer);
void cgBreakStmt(Break* self, FILE* writer);
void cgBinaryOperator(BinaryOperator* self, FILE* writer);
void cgUnaryOperator(UnaryOperator* self, FILE* writer);
void cgConstant(Constant* self, FILE* writer);
void cgTypeCast(TypeCast* self, FILE* writer);
void cgCall(IdMutationCall* self, FILE* writer);
void cgExprs(Exprs* self, FILE* writer);
void cgIdMutationChild(IdMutations* self, FILE* writer);

#endif
