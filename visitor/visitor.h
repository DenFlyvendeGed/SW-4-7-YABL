
#include "typeChecker.h"






Data*visitPreamble(Preamble* self, int indent);
Data*visitRepeatable(Repeatable* self, int indent);
Data*visitExprs(Exprs* self, int indent);
Data*visitStmts(Stmts* self, int indent);
Data*visitScope(Scope* self, int indent);
Data*visitArgs(Args* self, int indent);
Data*visitFuncs(Funcs* self, int indent);
Data*visitListConstant(ListConstant* self, int indent);
Data*visitPreambles(Preambles* self, int indent);
Data*visitExpr(Expr* self, int indent);
Data*visitStmt(Nonterminals* self, int indent);
Data*visitFunc(Func* self, int indent);
Data*visitIdMutation(IdMutation* self, int indent);
Data*visitUnaryop(UnaryOperator* self, int indent);
Data*visitBinaryOp(BinaryOperator* self, int indent);
Data*visitAssign(Assign* self, int indent);
Data*visitIfStmt(IfStmt* self, int indent);
Data*visitRepeat(Repeat* self, int indent);
//Data*visitReturnStmt(self);
Data*visitInitialization(Initialization* self, int indent);
Data*visitType(Type* self, int indent);
Data*visitIdMutationDot(IdMutationDot* self, int indent);
Data*visitIdMutationCall(IdMutationCall* self, int indent);
Data*visitIdMutationIndex(IdMutationIndex* self, int indent);
Data*visitTimesLoop(TimesLoop* self, int indent);
Data*visitForLoop(ForLoop* self, int indent);
Data*visitWhileLoop(WhileLoop* self, int indent);
Data*visitRepeatLoop(RepeatLoop* self, int indent);
Data*visitTypeValue(TypeValue* self, int indent);
Data*visitId(Id* self, int indent);
Data*visitBasicType(BasicTypes* self, int indent);
Data*visitTypeDCL(Type* self, int indent);
Data*visitEvent(Event* self, int indent);
Data*visitVariable(Variable* self, int indent);
Data*visitPreambleBoard(PreambelBoard* self, int indent);
Data*visitPreambleTileItem(PreambleTileItem* self, int indent);
Data*visitPreambleTile(PreambelTile* self, int indent);
Data*visitPreamblePlayer(PreamblePlayers* self, int indent);