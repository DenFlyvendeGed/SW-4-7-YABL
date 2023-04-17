
#ifndef VISITOR_H
#define VISITOR_H

#include "typeChecker.h"

Repeatable* visit(Repeatable* self);

Data*visitPreamble(Preamble* self);
Data*visitRepeatable(Repeatable* self);
Data*visitExprs(Exprs* self);
Data*visitStmts(Stmts* self);
Data*visitScope(Scope* self);
Data*visitArgs(Args* self);
Data*visitFuncs(Funcs* self);
Data*visitListConstant(ListConstant* self);
Data*visitPreambles(Preambles* self);
Data*visitExpr(Expr* self);
Data*visitStmt(Nonterminals* self);
Data*visitFunc(Func* self);
Data*visitIdMutation(IdMutation* self);
Data*visitUnaryop(UnaryOperator* self);
Data*visitBinaryOp(BinaryOperator* self);
Data*visitAssign(Assign* self);
Data*visitIfStmt(IfStmt* self);
Data*visitRepeat(Repeat* self);
//Data*visitReturnStmt(self);
Data*visitInitialization(Initialization* self);
Data*visitType(Type* self);
Data*visitIdMutationDot(IdMutationDot* self);
Data*visitIdMutationCall(IdMutationCall* self);
Data*visitIdMutationIndex(IdMutationIndex* self);
Data*visitTimesLoop(TimesLoop* self);
Data*visitForLoop(ForLoop* self);
Data*visitWhileLoop(WhileLoop* self);
Data*visitRepeatLoop(RepeatLoop* self);
Data*visitTypeValue(TypeValue* self);
Data*visitId(Id* self);
Data*visitBasicType(BasicTypes* self);
Data*visitTypeDCL(Type* self);
Data*visitEvent(Event* self);
Data*visitVariable(Variable* self);
Data*visitPreambleBoard(PreambelBoard* self);
Data*visitPreambleTileItem(PreambleTileItem* self);
Data*visitPreambleTile(PreambelTile* self);
Data*visitPreamblePlayer(PreamblePlayers* self);

#endif // !VISITOR_H
