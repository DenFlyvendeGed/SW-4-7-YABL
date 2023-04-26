#include "visitor.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define PPRINTFLAG 1

#define FOREACH(T, X, E){\
	YablList _l = X->children;\
	if(_l->item != NULL) for(; _l != NULL; _l = _l->next){\
		T foreach_value = _l->item;\
		E\
	}\
}

int indent = 0;
YablHash* symbolTable;


void pIndent(){
    for(int i=1; i<=indent; i++){
        printf("    ");
    }
}
void prettyPrint(char string[]){
    pIndent();
    printf("%s\n", string);
}

//Visitor function
Data* visitor(){
	return tcAccept();
}


//Visit function
Repeatable* visit(Repeatable* self){ //Start <----
    
    //innit outersymbolTable
    YablHash* symbolTable = malloc(sizeof(YablHash));
    *symbolTable = yablHashCreate(HASHLISTLENGTH, &stringHash);

    if(PPRINTFLAG == 1)
    {
        prettyPrint("start");
    }
    indent++;
    // visitPreamble(self->preamble);
    visitRepeatable(self);
    indent--;
    printf("Error Count: %d\n", TYPE_CHECKER_ERROR_COUNT);
    return self;
}

//--------------------------------------

//Mangler
Data* visitPreamble(Preamble* self){
    if(PPRINTFLAG == 1)
    {
        prettyPrint("Preamble");
    }

    return tcAccept();
}

Data* visitRepeatable(Repeatable* self){
    if(PPRINTFLAG == 1)
    {
        prettyPrint("Repeatable");
    }
    indent++;
    Data* rtn;
    switch (self->nonterminal)
    {
        
    case exprs:
        rtn = visitExprs(self);
        break;
    case stmts:
        rtn =  visitStmts(self);
        break;
    case scope:
        rtn =  visitScope(self, NULL);
        break;
    case args:
        rtn =  visitArgs(self);
        break;
    case funcs:
        rtn =  visitFuncs(self);
        break;
    case preambles:
        rtn =  visitPreambles(self);
        break;
    default:
        rtn = createError(ECoutOfRange);
        break;
    }
    indent--;
	return rtn;
}

Data* visitExprs(Exprs* self){
    if(self == NULL){
        return tcAccept();
    }
    if(PPRINTFLAG == 1)
    {
        prettyPrint("Exprs");
    }
    indent++;
	FOREACH(Expr*, self, 
		Data* value = visitExpr(foreach_value);
		if(value->errorCode != ECnoError) return value;
		// return tcExpr(foreach_value, value);
	)
    indent--;
    return tcAccept(); //<----
}

Data* visitStmts(Stmts* self){
    if(PPRINTFLAG == 1)
    {
        prettyPrint("Stmts");
    }
    indent++;
    // yablListSimpleForeach(self->children, &visitStmt);
    FOREACH(Stmt*, self, 
		Data* value = visitStmt(foreach_value);
		if(value->errorCode != ECnoError) return value;
		// return tcStmt(foreach_value, value);
	)
    indent--;
    return tcAccept(); //<----
}

Data* visitScope(Scope* self, Data* returnType){
    if(PPRINTFLAG == 1)
    {
        prettyPrint("Scope");
    }
    indent++;
	YablList l = self->children;
	
    // yablListSipleForeach(self->children, &visitStmt, 0);
    FOREACH(Stmt*, self, 
		Data* value = visitStmt(foreach_value);
		if(*(Nonterminals*)foreach_value == returnstmt){//check returnstmt against returnType
            if(returnType != NULL ){
                if(value->type != returnType->type){ //currently dosent work against id's
                    printf("invalid return type: (scope)%i, (expected)%i", value->type, returnType->type);
                    createError(ECtypeExeption);
                }
            }
        }  
		// return tcAccept(); //<---
	)

    indent--;
    return tcAccept(); //<----
}

Data* visitArgs(Args* self){
    if(PPRINTFLAG == 1)
    {
        prettyPrint("Args");
    }
    indent++;
    visitExprs(self);
    indent--;
    return tcAccept(); //<----
}

Data* visitFuncs(Funcs* self){
    if(PPRINTFLAG == 1)
    {
        prettyPrint("Funcs");
    }
    indent++;
    // yablListSipleForeach(self->children, &visitFunc, 0);
    FOREACH(Func*, self, 
		Data* value = visitFunc(foreach_value);
		if(value->errorCode != ECnoError);
        // tcAccept();//<---
	)
    indent--;
    return tcAccept(); //<----
}

Data* visitListConstant(ListConstant* self){
    if(PPRINTFLAG == 1)
    {
        prettyPrint("Constants");
    }
    indent++;
    visitExprs(self);
    indent--;
    return tcAccept(); //<----
}

//Mangler
Data* visitPreambles(Preambles* self){
    if(PPRINTFLAG == 1)
    {
        prettyPrint("Preambles");
    }

    return tcAccept(); //<----
}

//--------------------------------------

Data*  visitExpr(Expr* self){
    if(PPRINTFLAG == 1)
    {
        prettyPrint("Expr");
    }
    indent++;

    Data* child;
    Data* rval;
    if(self == NULL)
        //createError(ECempty);
        return NULL;
    switch (self->exprType)
    {
    case et_constant:
        //need a way to check type
        child = tcAccept();//createData(bt_number);
        break;
    case et_id_mutation:
        child = visitIdMutation(self->child);
        break;
    case et_unary_operator:
        child = visitUnaryop(self->child);
        break;
    case et_binary_operator:
        child = visitBinaryOp(self->child);
        break;
    case et_expression:
        child = visitExpr(self->child);
        break;
    default:
        printf("Expr type error"); //<-------------- error
        break;
    }
    rval = tcExpr(self, child);
    free(child);
    indent--;
    return rval;
}

Data* visitStmt(Nonterminals* self){
    if(PPRINTFLAG == 1)
    {
        prettyPrint("Stmt");
    }
    indent++;
    Data* rtn;
    switch (*self)
    {
    case assign:
        rtn =  visitAssign((Assign*)self);
        break;
    case ifstmt:
        rtn =  visitIfStmt((IfStmt*)self);
        break;
    case repeatstmt:
        rtn =  visitRepeat((Repeat*)self);
        break;
    case initialization:
        rtn =  visitInitialization((Initialization*)self);
        break;
    case scope:
        rtn =  visitScope((Scope*)self, NULL);
        break;
    case expr:
        rtn =  visitExpr((Expr*)self);
        break;
     case returnstmt: //<-----
        rtn = visitReturnStmt((ReturnStmt*)self);
        break;
    default:
        return createError(ECoutOfRange);
        break;
    }
    indent--;
    return rtn;
}

Data* visitFunc(Func* self){
    if(PPRINTFLAG == 1)
    {
        prettyPrint("Func");
    }
    indent++;
    Data* rval;
    switch (self->nonterminal){
		case func:;
			Data* args = visitArgs(self->args);
			Data* returnType = visitType(self->returntype);
			Data* scope = visitScope(self->scope, returnType); //check returnstmt against returntype
			Data* id = visitId(&self->name);
			rval = tcFunc(self, args, returnType, scope, id);
			free(args);
			free(returnType);
			free(scope);
			free(id);
			break;
		case event:
			rval = visitEvent((Event*)self);
			break;
		default:
			return createError(ECoutOfRange);
			break;
    }
    indent--;
    return rval;
}

//--------------------------------------

Data* visitIdMutation(IdMutation* self){
    if(self == NULL || self->child == NULL){ //<-- might need fixes
        return tcAccept();
    }
    if(PPRINTFLAG == 1)
    {
        prettyPrint("IdMutation");
    }
    indent++;
    

    Data* id = visitId(self->name);
    Data* child;
    Data* rval;
    switch (*(IdMutations*)(self->child))
    {
    case im_none:
        break;
    case im_dot:
        child = visitIdMutationDot(self->child);
        break;
    case im_call:
        child = visitIdMutationCall(self->child);
        break;
    case im_index:
        child = visitIdMutationIndex(self->child);
        break;
    default:
        break;
    }
    rval = tcIdMutation(self, child, id);
    free(child);
    free(id);
    indent--;
    return rval;
}

Data* visitUnaryop(UnaryOperator* self){
    if(PPRINTFLAG == 1)
    {
        prettyPrint("Unaryop");
    }
    indent++;
    Data* rval;
    Data* expr = visitExpr(self->childExpr);

    rval = tcUnaryop(self, expr);
    free(expr);
    indent--;
    return rval;
}

Data* visitBinaryOp(BinaryOperator* self){
    if(PPRINTFLAG == 1)
    {
        prettyPrint("BinaryOp");
    }
    indent++;
    Data* rval;
    Data* child1 = visitExpr(self->childExpr1);
    Data* child2 = visitExpr(self->childExpr2);
    

    rval = tcBinaryOp(self, child1, child2);
    free(child1);
    free(child2);

    indent--;
    return rval;
}

Data* visitAssign(Assign* self){
    if(PPRINTFLAG == 1)
    {
        prettyPrint("Assign");
    }
    indent++;
    Data* rval;
    Data* id = visitId(self->variable);
    Data* expr = visitExpr(self->expression);

    rval = tcAssign(self, id, expr);
    free(id);
    free(expr);

    indent--;
    return rval;
}

Data* visitIfStmt(IfStmt* self){
    if(PPRINTFLAG == 1)
    {
        prettyPrint("IfStmt");
    }
    indent++;
    Data* rval;
    Data* expr = visitExpr(self->condition);
    Data* scope1 = visitScope(self->then, NULL);
    Data* scope2 = visitScope(self->elsestmt, NULL);


    rval = tcIfStmt(self, expr, scope1, scope2);
    free(expr);
    free(scope1);
    free(scope2);

    indent--;
    return rval;
}

Data* visitRepeat(Repeat* self){
    if(PPRINTFLAG == 1)
    {
        prettyPrint("Repeat");
    }
    indent++;

    Data* rval;
    Data* loopHeader;
    switch (*(LoopType*)self->loopType)
    {
    case lt_timesloop:
        loopHeader = visitTimesLoop(self->loopType);
        break;
    case lt_forloop:
        loopHeader = visitForLoop(self->loopType);
        break;
    case lt_whileloop:
        loopHeader = visitWhileLoop(self->loopType);
        break;
    case lt_repeatloop:
        loopHeader = visitRepeatLoop(self->loopType);
        
        break;
    
    default:
        loopHeader = createError(ECoutOfRange);
        break;
    }
    Data* scope = visitScope(self->scope,NULL);

    rval = tcRepeat(self, loopHeader, scope);
    free(loopHeader);
    free(scope);

    indent--;
    return rval;
}

//Mangler <-----
Data* visitReturnStmt(ReturnStmt* self){
    printf("returnstmt \n");
    Data* rtn = visitExpr(self->expr);
    return rtn;
}

Data* visitInitialization(Initialization* self){
    if(PPRINTFLAG == 1)
    {
        prettyPrint("Initialization");
    }
    
    indent++;
    Data* rval;
    Data* id = visitId(self->variable);
    Data* type = visitType(self->type);
    Data* val = visitExpr(self->initialValue);
    

    rval = tcInitialization(self, type, val);
    free(type);

    indent--;
    return rval;
}

Data* visitType(Type* self){
    if(PPRINTFLAG == 1)
    {
        prettyPrint("Type");
    }
    indent++;
    Data* rval;
    Data* type = visitTypeValue(self->typeval);

    rval = tcType(self, type); //returns type
    // free(type);

    indent--;
    return rval;
}

//--------------------------------------

Data*  visitIdMutationDot(IdMutationDot* self){
    if(PPRINTFLAG == 1)
    {
        prettyPrint("IdMutationDot");
    }
    indent++;
    Data* rval;
    
    Data* name = visitId(NULL);
    Data* child = visitIdMutation(self->child);

    rval = tcIdMutationDot(self, name, child);
    free(name);
    free(child);

    indent--;
    return rval;
}

Data*  visitIdMutationCall(IdMutationCall* self){
    if(PPRINTFLAG == 1)
    {
        prettyPrint("IdMutationCall");
    }
    indent++;

    Data* rval;
 
    Data* child = visitExprs(self->child);
    Data*  argData = visitArgs(self->args);

    rval = tcIdMutationCall(self, child, argData);
    free(child);
    free(argData);
    
    indent--;
    return rval;
}   

Data* visitIdMutationIndex(IdMutationIndex* self){
    if(PPRINTFLAG == 1)
    {
        prettyPrint("IdMutationIndex");
    }
    indent++;
    Data* rval;

    Data* expr = visitExpr(self->index);
    Data* child = visitIdMutation(self->child);

    rval = tcIdMutationIndex(self, expr, child);
    free(expr);
    free(child);

    indent--;
    return rval;
}

Data* visitTimesLoop(TimesLoop* self){
    if(PPRINTFLAG == 1)
    {
        prettyPrint("TimesLoop");
    }
    indent++;
    Data* rval;
    Data* goalExpr = visitExpr(self->goal);

    rval = tcTimesLoop(self, goalExpr);
    free(goalExpr);

    indent--;
    return rval;
}

Data* visitForLoop(ForLoop* self){
    if(PPRINTFLAG == 1)
    {
        prettyPrint("ForLoop");
    }
    indent++;
    Data* rval;
    Data* id = visitId(NULL);

    rval = tcForLoop(self, id);
    free(id);

    indent--;
    return rval;
}

Data* visitWhileLoop(WhileLoop* self){
    if(PPRINTFLAG == 1)
    {
        prettyPrint("WhileLoop");
    }
    indent++;
    Data* rval;
    Data* expr = visitExpr(self->condition);

    rval = tcWhileLoop(self, expr);
    free(expr);

    indent--;
    return rval;
}

Data* visitRepeatLoop(RepeatLoop* self){ // <--

    return tcRepeatLoop(self);
}

Data* visitTypeValue(TypeValue* self){ //<----- måske der skal laves switch for at checke hvilken der er gældne
    if(PPRINTFLAG == 1)
    {
        prettyPrint("TypeValue");
    }
    indent++;
    Data* rval;

    // Data* basicType = createData(self->type);//visitBasicType(&self->type); //gør ikke noget
    // Data* typeDcl = visitTypeDCL(self->list); //gør ikke noget

    // rval = tcTypeValue(self, basicType, typeDcl); //gør ikke noget
    // free(basicType);
    // free(typeDcl);

    indent--;
    return createData(self->type);//rval;
}

//--------------------------------------

//mangler
Data* visitId(Id self){ //<---
    if(PPRINTFLAG == 1)
    {
        prettyPrint("Id");
    }
    
    return tcId(self);
}
Data* visitBasicType(BasicTypes* self){ //det er en enum <---
    switch (*self)
    {
    case bt_number:
        visitTypeDCL(self);
        break;
    case bt_text:
        visitTypeDCL(self);
        break;
    case bt_logic:
        visitTypeDCL(self);
        break;
    default:
        break;
    }
}
Data* visitTypeDCL(Type* self){
    if(PPRINTFLAG == 1)
    {
        prettyPrint("TypeDCL");
    }
    indent++;
    // Data* rval;
    // Data* tval = visitTypeValue(self->typeval);

    // rval = tcTypeDCL(self, tval);
    // free(tval);

    indent--;
    return tcAccept();
    // return rval;
}

Data* visitEvent(Event* self){
    if(PPRINTFLAG == 1)
    {
        prettyPrint("Event");
    }
    indent++;
    Data* rval;

    Data* scope = visitScope(self->scope, NULL);
    rval = tcEvent(self, scope);
    // free(scope);

    indent--;
    return rval;
}

Data* visitVariable(Variable* self){
    if(PPRINTFLAG == 1)
    {
        prettyPrint("Variable");
    }
    indent++;
    Data* rval;
    Data* id = visitId(&self->name);
    Data* type = visitType(self->type);

    rval = tcVariable(self, type, id);
    free(id);
    free(type);

    indent--;
    return rval;
}

Data* visitPreambleBoard(PreambelBoard* self){
    return tcAccept();
}

Data* visitPreambleTileItem(PreambleTileItem* self){
    visitId(&self->name);
    return tcAccept();
}

Data* visitPreambleTile(PreambelTile* self){ //<----
    // yablListSimpleForeach(*self->tile_items, &visitId);
    
    return tcAccept();
}

Data* visitPreamblePlayer(PreamblePlayers* self){ //<--
    // yablListSimpleForeach(*self->ids, &visitId);
    return tcAccept();
}


