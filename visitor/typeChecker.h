#ifndef TYPE_CHECKER_H
#define TYPE_CHECKER_H

#include "../data-structures/list.h"
#include "../data-structures/hashtable.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../cfg.h"

//Jeg kan ikke huske borders :)
#define NUMBERMAX 255
#define NUMBERMIN -255
#define HASHLISTLENGTH 10

extern int TYPE_CHECKER_ERROR_COUNT;
extern YablHash* SYMBOL_TABLE;
extern int indent;
extern void pIndent();
extern void prettyPrint(char string[]);



typedef enum  {ECnoError, ECempty, ECargumentExeption, ECtypeExeption, ECmissingChild, ECoutOfRange, ECoutOfNamespace} ErrorCode;

typedef struct {
    // Nonterminals nonterminal;
    BasicTypes type;
    ErrorCode errorCode;
    void* list; //Data*
    void* value; 
} Data;

//Symbol table setup

int stringHash(char* string);

int stringcompare(char* s1, char* s2);

void symbolTablePush( char* key, void* value);

Data* symbolTableGet(char* key);

void createSymbolTable();

void deleteSymbolTable();





// YablHash global = yablHashCreate(10, &stringHash);
// int* i = malloc(sizeof(int));
// *i = 10;
// yablHashPush(&global, "test", i, &stringcompare);
// int* res = ((YablHashNode*)yablHashGet(&global, "test", &stringcompare))->item;

// printf("res= %i\n", *res);



//Data constructers
Data* createData(BasicTypes dType);
Data* createList(Data* child);
Data* createError(ErrorCode error);
Data* tcAccept();
Data* tcCopy(Data*in);
Data* tcValue(void* val);


Data* tcPreamble(Preambles* self);

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
Data* tcInitialization(Initialization* self, Data* id, Data* type, Data* val);
Data* tcType(Type* self, Data* typeVal);

Data* tcIdMutation(IdMutation* self, Data* child, Data* id);
Data* tcIdMutationDot(IdMutationDot* self, Data* idMutation);
Data* tcIdMutationCall(IdMutationCall* self, Data* idMutation, Data* args);
Data* tcIdMutationIndex(IdMutationIndex* self, Data* expr, Data* idMutation);

Data* tcRepeat(Repeat* self, Data* loopHeader, Data* scope);
Data* tcTimesLoop(TimesLoop* self, Data* goalExpr);
Data* tcForLoop(ForLoop* self, Data* inputName);
Data* tcWhileLoop(WhileLoop* self, Data* cond);
Data* tcRepeatLoop(RepeatLoop* self);

Data* tcTypeValue(TypeValue* self, Data* list, Data* typedcl); //<---- union, der kan kun være real data i en af dem
Data* tcId(Id self);
Data* tcBasicType(BasicTypes* self); //<----- enum
Data* tcTypeDCL(Type* self, Data* typeval);


Data* tcVariable(Variable* self, Data* type, Data* id);

//work in progress
Data* tcPreambleBoard(PreambleBoard* self);
Data* tcPreambleTileItem(Type* self);
Data* tcPreambleTile(PreambleTile* self);
Data* tcPreamblePlayer(PreamblePlayers* self);

#endif // !TYPE_CHECKER_H

