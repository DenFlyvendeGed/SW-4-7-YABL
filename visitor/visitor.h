
#include "../cfg.h"
#include "../data-structures/list.h"


void visitPreamble(Preamble* self);
void visitRepeatable(Repeatable* self);
void visitExprs(Exprs* self);
void visitStmts(Stmts* self);
void visitScope(Scope* self);
void visitArgs(Args* self);
void visitFuncs(Funcs* self);
void visitListConstant(ListConstant* self);
void visitPreambles(Preambles* self);
void visitExpr(Expr* self);
void visitStmt(Nonterminals* self);
void visitFunc(Func* self);
void visitIdMutation(IdMutation* self);
void visitUnaryop(UnaryOperator* self);
void visitBinaryOp(BinaryOperator* self);
void visitAssign(Assign* self);
void visitIfStmt(IfStmt* self);
void visitRepeat(Repeat* self);
//void visitReturnStmt(self);
void visitInitialization(Initialization* self);
void visitType(Type* self);
void visitIdMutationDot(IdMutationDot* self);
void visitIdMutationCall(IdMutationCall* self);
void visitIdMutationIndex(IdMutationIndex* self);
void visitTimesLoop(TimesLoop* self);
void visitForLoop(ForLoop* self);
void visitWhileLoop(WhileLoop* self);
void visitRepeatLoop(RepeatLoop* self);
void visitTypeValue(TypeValue* self);
void visitId(Id* self);
void visitBasicType(BasicTypes* self);
void visitTypeDCL(Type* self);
void visitEvent(Event* self);
void visitVariable(Variable* self);
void visitPreambleBoard(PreambelBoard* self);
void visitPreambleTileItem(PreambleTileItem* self);
void visitPreambleTile(PreambelTile* self);
void visitPreamblePlayer(PreamblePlayers* self);