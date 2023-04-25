#ifndef TYPE_CHECKER_H
#define TYPE_CHECKER_H

#include "../data-structures/list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../cfg.h"

//Jeg kan ikke huske borders :)
#define NUMBERMAX 255
#define NUMBERMIN -255

extern int TYPE_CHECKER_ERROR_COUNT;


typedef enum  {ECnoError, ECempty, ECargumentExeption, ECtypeExeption, ECmissingChild, ECoutOfRange} ErrorCode;

typedef struct {
    Nonterminals nonterminal;
    BasicTypes type;
    ErrorCode errorCode;
    void* value; 
} Data;

//Data constructers
Data* createData(BasicTypes dType);
Data* createError(ErrorCode error);
Data* tcAccept();


Data* tcPreamble(Preamble* self);

//List
Data* tcRepeatable(Repeatable* self);
Data* tcExprs(Exprs* self);
Data* tcStmts(Stmts* self);
Data* tcScope(Scope* self);
Data* tcArgs(Args* self);
Data* tcFuncs(Funcs* self);
Data* tcListConstant(ListConstant* self);
Data* tcPreambles(Preambles* self);


Data* tcExpr(Expr* self, Data* child);
Data* tcStmt(Stmt* self, Data* child); //return visit data
Data* tcFunc(Func* self, Data* args, Data* returntype, Data* scope, Data* id);
Data* tcEvent(Event* self, Data* scope);

Data* tcUnaryop(UnaryOperator* self, Data* expr);
Data* tcBinaryOp(BinaryOperator* self, Data* expr1, Data* expr2);

Data* tcAssign(Assign* self, Data* id, Data* expr);
Data* tcIfStmt(IfStmt* self, Data* condition, Data* thenScope, Data* elseScope);

//Data* tcReturnStmt(self);
Data* tcInitialization(Initialization* self, Data* type, Data* val);
Data* tcType(Type* self, Data* typeVal);

Data* tcIdMutation(IdMutation* self, Data* child, Data* id);
Data* tcIdMutationDot(IdMutationDot* self, Data* name, Data* idMutation);
Data* tcIdMutationCall(IdMutationCall* self, Data* idMutation, Data* args);
Data* tcIdMutationIndex(IdMutationIndex* self, Data* expr, Data* idMutation);

Data* tcRepeat(Repeat* self, Data* loopHeader, Data* scope);
Data* tcTimesLoop(TimesLoop* self, Data* goalExpr);
Data* tcForLoop(ForLoop* self, Data* inputName);
Data* tcWhileLoop(WhileLoop* self, Data* cond);
Data* tcRepeatLoop(RepeatLoop* self);

Data* tcTypeValue(TypeValue* self, Data* list, Data* typedcl); //<---- union, der kan kun være real data i en af dem
Data* tcId(Id* self);
Data* tcBasicType(BasicTypes* self); //<----- enum
Data* tcTypeDCL(Type* self, Data* typeval);


Data* tcVariable(Variable* self, Data* type, Data* id);

//work in progress
Data* tcPreambleBoard(PreambelBoard* self);
Data* tcPreambleTileItem(PreambleTileItem* self);
Data* tcPreambleTile(PreambelTile* self);
Data* tcPreamblePlayer(PreamblePlayers* self);

#endif // !TYPE_CHECKER_H

