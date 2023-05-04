#ifndef YABL_CFG_FUNCS_H
#define YABL_CFG_FUNCS_H

#include "cfg-structs.h"

char* copystringalloc(char*);

Repeatable* createRepeatable(Nonterminals nonterminal);
Repeatable* repeatablePushChild(Repeatable* self, void* child);

Exprs* createExprs();
void destroyExprs(Exprs* exprs);

Constant* createConstant(TypeDcls type, char* value);
void destroyConstant(Constant* p);

Func* createFunc(char* name, Args* funcArgs, Type* returnType, Scope* funcScope);
void destroyFunc(Func* p);

TypeValue* createTypeValue(BasicTypes varType, Type* listChild);
void destroyTypeValue(TypeValue* p);

Type* createType(TypeValue* value);
void destroyType(Type* p);

Repeatable* creatRepeatable(Nonterminals nonterminal, YablList* list);
void destroyRepeatable(Repeatable* p);

Expr* createExpr(ExprType ExprType, void* child);
Exprs* exprsAddExpr(Exprs* self, Expr* expr);
void destroyExpr(Expr* p);

BinaryOperator* createBinaryOperator(BinaryOperators bOp, Expr* childExpr1, Expr* childExpr2);
void destroyBinaryOperator(BinaryOperator* p);

UnaryOperator* createUnaryOperator(UnaryOperators uOp, Expr* childExpr);
void destroyUnaryOperator(UnaryOperator* p);

TypeCast* createTypeCast(Type* type, Expr* cast);
void destroyTypeCast(TypeCast* self);

IdMutationDot* createIdMutationDot(IdMutation* child);
void destroyIdMutationDot(IdMutationDot* p);

IdMutationCall* createIdMutationCall(void* child, Exprs* mutationArgs);
void destroyIdMutationCall(IdMutationCall* p);

IdMutationIndex* createIdMutationIndex(void* child, Expr* index);
void destroyIdMutationIndex(IdMutationIndex* p);

IdMutation* createIdMutation(Id name, void* child);
void destroyIdMutation(IdMutation* p);

IfStmt* createIfStmt(Expr* cond, Scope* condTrue, Scope* condFalse);
void destroyIfStmt(IfStmt* p);

Repeat* createRepeat(void* loop, Scope* repeatScope);
void destroyRepeat(Repeat* self);

TimesLoop* createTimesLoop(Expr* goal);
void destroyTimesLoop(TimesLoop* p);

ForLoop* createForLoop(Id varName, IdMutation* inputName);
void destroyForLoop(ForLoop *p);

WhileLoop* createWhileLoop(Expr* cond);
void destroyWhileloop(WhileLoop* p);

RepeatLoop* createRepeatLoop();
void destroyRepeatLoop(RepeatLoop* p);

Break* createBreak();
void destroyBreak(Break* self);

Initialization* createInitialization(Id var, Type* varType, Expr* initialValue);
void destroyInitialization(Initialization* p);

Assign* createAssign(IdMutation* var, Expr* exp);
void destroyAssign(Assign* p);

Event* createEvent(Scope* eventScope, Events eventType);
void destroyEvent(Event* p);

Variable createVariable(Id name, Type* type);
void destroyVariable(Variable* p);

Stmts* createStmts();
void destroyStmts(Stmts* p);
Stmts* stmtsAddStmt(Stmts* self, Stmt* stmt);

Scope* createScope(Stmts* stmts);
Scope* scopeAddStmt(Scope* self, Stmt* stmt);
void destroyScope(Scope* p);

Args* createArgs();
Args* argsAddInitialization(Args* self, Initialization* initialization);
void destroyArgs(Args* p);

Funcs* createFuncs();
Funcs* funcsAddFunc(Funcs* self, Func* func);
void  destroyFuncs(Funcs* p);

ListConstant* createListConstant(Exprs* exprs);
void destroyListConstant(ListConstant* p);

ReturnStmt* createReturnStmt(Expr* rtn);
void destroyReturnStmt(ReturnStmt* self);

Preambles* createPreambles();
Preambles* preamblesPushPreamble(Preambles* self, void* child);
void  destroyPreambles(Preambles* self);

PreambleTile* createPreambleTile();
PreambleTile* preambleTileAddInitialiation(PreambleTile* self, Initialization* variable);
void destroyPreambleTile(PreambleTile* self);

PreamblePlayers* createPreamblePlayers();
PreamblePlayers* preamblePlayersAddPlayer(PreamblePlayers* self, char* player);
void destroyPreamblePlayers(PreamblePlayers* self);

PreambleBoard* createPreambleBoard(char* size);
void destroyPreambleBoard(PreambleBoard* self);

#endif // !YABL_CFG_FUNCS_H

