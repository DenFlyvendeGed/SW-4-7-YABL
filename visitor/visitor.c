#include "visitor.h"
#include "stdlib.h"
#include <stdio.h>

#define PPRINTFLAG 1


void pIndent(int indent){
    for(int i=1; i<=indent; i++){
        printf("    ");
    }
}
void prettyPrint(char string[], int indent){
    pIndent(indent);
    printf("%s\n", string);
}

//Visitor function
Data* visitor(){
}

//Visit function
Data* visit(self){ //Start <----
    int indent = 0;
    if(PPRINTFLAG == 1)
    {
        prettyPrint("start", indent);
    }
    indent++;
    visitPreamble(self.preamble, indent);
    visitRepeatable(self.func, indent);
}

//--------------------------------------

//Mangler
Data* visitPreamble(Preamble* self, int indent){
    if(PPRINTFLAG == 1)
    {
        prettyPrint("Preamble", indent);
    }
    indent++;
    return tcAccept();
}

Data* visitRepeatable(Repeatable* self, int indent){
    if(PPRINTFLAG == 1)
    {
        prettyPrint("Repeatable", indent);
    }
    indent++;
    switch (self->nonterminal)
    {
    case exprs:
        return visitExprs(self, indent);
        break;
    case stmts:
        return visitStmts(self, indent);
        break;
    case scope:
        return visitScope(self, indent);
        break;
    case args:
        return visitArgs(self, indent);
        break;
    case funcs:
        return visitFuncs(self, indent);
        break;
    case listConstants:
        return visitListConstant(self, indent);
        break; 
    case preambles:
        return visitPreambles(self, indent);
        break;
    default:
        break;
    }

}

Data* visitExprs(Exprs* self, int indent){
    if(PPRINTFLAG == 1)
    {
        prettyPrint("Exprs", indent);
    }
    indent++;
    yabl_list_foreach(*self->children, &visitExpr); //<---- fix these
    return tcAccept(); //<----
}

Data* visitStmts(Stmts* self, int indent){
    if(PPRINTFLAG == 1)
    {
        prettyPrint("Stmts", indent);
    }
    indent++;
    yabl_list_foreach(*self->children,&visitStmt);
    return tcAccept(); //<----
}

Data* visitScope(Scope* self, int indent){
    if(PPRINTFLAG == 1)
    {
        prettyPrint("Scope", indent);
    }
    indent++;
    yabl_list_foreach(*self->children, &visitScope);
    return tcAccept(); //<----
}

Data* visitArgs(Args* self, int indent){
    if(PPRINTFLAG == 1)
    {
        prettyPrint("Args", indent);
    }
    indent++;
    visitExprs(self, indent);
    return tcAccept(); //<----
}

Data* visitFuncs(Funcs* self, int indent){
    if(PPRINTFLAG == 1)
    {
        prettyPrint("Funcs", indent);
    }
    indent++;
    yabl_list_foreach(*self->children, &visitFunc);
    return tcAccept(); //<----
}

Data* visitListConstant(ListConstant* self, int indent){
    if(PPRINTFLAG == 1)
    {
        prettyPrint("Constants", indent);
    }
    indent++;
    visitExprs(self, indent);
    return tcAccept(); //<----
}

//Mangler
Data* visitPreambles(Preambles* self, int indent){
    if(PPRINTFLAG == 1)
    {
        prettyPrint("Preambles", indent);
    }
    indent++;
    return tcAccept(); //<----
}

//--------------------------------------

Data*  visitExpr(Expr* self, int indent){
    if(PPRINTFLAG == 1)
    {
        prettyPrint("Expr", indent);
    }
    indent++;

    Data* child;
    Data* rval;
    switch (self->exprType)
    {
    case et_constant:
        //need a way to check type
         
        break;
    case et_id_mutation:
        child = visitIdMutation(self->child, indent);
        break;
    case et_unary_operator:
        child = visitUnaryop(self->child, indent);
        break;
    case et_binary_operator:
        child = visitBinaryOp(self->child, indent);
        break;
    case et_expression:
        child = visitExpr(self->child, indent);
        break;
    default:
        printf("Expr type error"); //<-------------- error
        break;
    }
    rval = tcExpr(self, child);
    free(child);
    return rval;
}

Data* visitStmt(Nonterminals* self, int indent){
    if(PPRINTFLAG == 1)
    {
        prettyPrint("Stmt", indent);
    }
    indent++;
    switch (*self)
    {
    case assign:
        return visitAssign((Assign*)self, indent);
        break;
    case ifstmt:
        return visitIfStmt((IfStmt*)self, indent);
        break;
    case repeatstmt:
        visitRepeat((Repeat*)self, indent);
        break;
    case initialization:
        return visitInitialization((Initialization*)self, indent);
        break;
    case scope:
        return visitScope((Scope*)self, indent);
        break;
    case expr:
        return visitExpr((Expr*)self, indent);
        break;
    case returnstmt: //<-----
        return visitReturnStmt(self, indent);
        break;
    default:
        return createError(ECoutOfRange);
        break;
    }

}

Data* visitFunc(Func* self, int indent){
    if(PPRINTFLAG == 1)
    {
        prettyPrint("Func", indent);
    }
    indent++;
    Data* rval;
    switch (self->nonterminal)
    {
    case func:
        Data* args = visitArgs(self->args, indent);
        Data* returnType = visitType(self->returntype, indent);
        Data* scope = visitScope(self->scope, indent);
        Data* id = visitId(&self->name, indent);
        rval = tcFunc(self, args, returnType, scope, id);
        free(args);
        free(returnType);
        free(scope);
        free(id);
        break;
    case event:
        rval = visitEvent(self, indent);
        break;
    default:
        return createError(ECoutOfRange);
        break;
    }
    return rval;
}

//--------------------------------------

Data* visitIdMutation(IdMutation* self, int indent){
    if(PPRINTFLAG == 1)
    {
        prettyPrint("IdMutation", indent);
    }
    indent++;

    Data* id = visitId(&self->name, indent);
    Data* child;
    Data* rval;
    switch (self->idMutation)
    {
    case im_none:
        break;
    case im_dot:
        child = visitIdMutationDot(self->child, indent);
        break;
    case im_call:
        child = visitIdMutationCall(self->child, indent);
        break;
    case im_index:
        child = visitIdMutationIndex(self->child, indent);
        break;
    default:
        break;
    }
    rval = tcIdMutation(self, child, id);
    free(child);
    free(id);
    return rval;
}

Data* visitUnaryop(UnaryOperator* self, int indent){
    if(PPRINTFLAG == 1)
    {
        prettyPrint("Unaryop", indent);
    }
    indent++;
    Data* rval;
    Data* expr = visitExpr(self->childExpr, indent);

    rval = tcUnaryop(self, expr);
    free(expr);
    return rval;
}

Data* visitBinaryOp(BinaryOperator* self, int indent){
    if(PPRINTFLAG == 1)
    {
        prettyPrint("BinaryOp", indent);
    }
    indent++;
    Data* rval;
    Data* child1 = visitExpr(self->childExpr1, indent);
    Data* child2 = visitExpr(self->childExpr2, indent);
    

    rval = tcBinaryOp(self, child1, child2);
    free(child1);
    free(child2);

    return rval;
}

Data* visitAssign(Assign* self, int indent){
    if(PPRINTFLAG == 1)
    {
        prettyPrint("Assign", indent);
    }
    indent++;
    Data* rval;
    Data* id = visitId(&self->variable, indent);
    Data* expr = visitExpr(self->expression, indent);

    rval = tcAssign(self, id, expr);
    free(id);
    free(expr);

    return rval;
}

Data* visitIfStmt(IfStmt* self, int indent){
    if(PPRINTFLAG == 1)
    {
        prettyPrint("IfStmt", indent);
    }
    indent++;
    Data* rval;
    Data* expr = visitExprs(self->condition, indent);
    Data* scope1 = visitScope(self->then, indent);
    Data* scope2 = visitScope(self->elsestmt, indent);


    rval = tcIfStmt(self, expr, scope1, scope2);
    free(expr);
    free(scope1);
    free(scope2);

    return rval;
}

Data* visitRepeat(Repeat* self, int indent){
    if(PPRINTFLAG == 1)
    {
        prettyPrint("Repeat", indent);
    }
    indent++;

    Data* rval;
    Data* loopHeader;
    switch (*(LoopType*)self->loopType)
    {
    case lt_timesloop:
        loopHeader = visitTimesLoop(self->loopType, indent);
        break;
    case lt_forloop:
        loopHeader = visitForLoop(self->loopType, indent);
        break;
    case lt_whileloop:
        loopHeader = visitWhileLoop(self->loopType, indent);
        break;
    case lt_repeatloop:
        loopHeader = visitRepeatLoop(self->loopType, indent);
        
        break;
    
    default:
        loopHeader = createError(ECoutOfRange);
        break;
    }
    Data* scope = visitScope(self->scope, indent);

    rval = tcRepeat(self, loopHeader, scope);
    free(loopHeader);
    free(scope);

    return rval;
}

//Mangler <-----
Data* visitReturnStmt(self, indent){

    return tcAccept();
}

Data* visitInitialization(Initialization* self, int indent){
    if(PPRINTFLAG == 1)
    {
        prettyPrint("Initialization", indent);
    }
    indent++;
    Data* rval;
    Data* type = visitType(self->type, indent);

    rval = tcInitialization(self, type);
    free(type);

    return rval;
}

Data* visitType(Type* self, int indent){
    if(PPRINTFLAG == 1)
    {
        prettyPrint("Type", indent);
    }
    indent++;
    Data* rval;
    Data* type = visitTypeValue(self->typeval, indent);

    rval = tcType(self, type);
    free(type);
    return rval;
}

//--------------------------------------

Data*  visitIdMutationDot(IdMutationDot* self, int indent){
    if(PPRINTFLAG == 1)
    {
        prettyPrint("IdMutationDot", indent);
    }
    indent++;
    Data* rval;
    
    Data* name = visitId(&self->name, indent);
    Data* child = visitIdMutation(self->child, indent);

    rval = tcIdMutationDot(self, name, child);
    free(name);
    free(child);

    return rval;
}

Data*  visitIdMutationCall(IdMutationCall* self, int indent){
    if(PPRINTFLAG == 1)
    {
        prettyPrint("IdMutationCall", indent);
    }
    indent++;

    Data* rval;
 
    Data* child = visitIdMutation(self->child, indent);
    Data*  argData = visitArgs(self->args, indent);

    rval = tcIdMutationCall(self, child, argData);
    free(child);
    free(argData);
    
    return rval;
}   

Data* visitIdMutationIndex(IdMutationIndex* self, int indent){
    if(PPRINTFLAG == 1)
    {
        prettyPrint("IdMutationIndex", indent);
    }
    indent++;
    Data* rval;

    Data* expr = visitExpr(self->index, indent);
    Data* child = visitIdMutation(self->child, indent);

    rval = tcIdMutationIndex(self, expr, child);
    free(expr);
    free(child);

    return rval;
}

Data* visitTimesLoop(TimesLoop* self, int indent){
    if(PPRINTFLAG == 1)
    {
        prettyPrint("TimesLoop", indent);
    }
    indent++;
    Data* rval;
    Data* goalExpr = visitExpr(self->goal, indent);

    rval = tcTimesLoop(self, goalExpr);
    free(goalExpr);

    return rval;
}

Data* visitForLoop(ForLoop* self, int indent){
    if(PPRINTFLAG == 1)
    {
        prettyPrint("ForLoop", indent);
    }
    indent++;
    Data* rval;
    Data* id = visitId(&self->inputName, indent);

    rval = tcForLoop(self, id);
    free(id);

    return rval;
}

Data* visitWhileLoop(WhileLoop* self, int indent){
    if(PPRINTFLAG == 1)
    {
        prettyPrint("WhileLoop", indent);
    }
    indent++;
    Data* rval;
    Data* expr = visitExpr(self->condition, indent);

    rval = tcWhileLoop(self, expr);
    free(expr);

    return rval;
}

Data* visitRepeatLoop(RepeatLoop* self, int indent){ // <--

    return tcRepeatLoop(self);
}

Data* visitTypeValue(TypeValue* self, int indent){ //<----- måske der skal laves switch for at checke hvilken der er gældne
    if(PPRINTFLAG == 1)
    {
        prettyPrint("TypeValue", indent);
    }
    indent++;
    Data* rval;
    Data* basicType = visitBasicType(&self->type, indent);
    Data* typeDcl = visitTypeDCL(self->list, indent);

    rval = tcTypeValue(self, basicType, typeDcl);
    free(basicType);
    free(typeDcl);

    return rval;
}

//--------------------------------------

//mangler
Data* visitId(Id* self, int indent){ //<---
    if(PPRINTFLAG == 1)
    {
        prettyPrint("Id", indent);
    }
    indent++;
    return tcAccept();
}

Data* visitBasicType(BasicTypes* self, int indent){ //det er en enum <---
    switch (*self)
    {
    case number:
        visitTypeDCL(self, indent);
        break;
    case text:
        visitTypeDCL(self, indent);
        break;
    case logic:
        visitTypeDCL(self, indent);
        break;
    default:
        break;
    }
}

Data* visitTypeDCL(Type* self, int indent){
    if(PPRINTFLAG == 1)
    {
        prettyPrint("TypeDCL", indent);
    }
    indent++;
    Data* rval;
    Data* tval = visitTypeValue(self->typeval, indent);

    rval = tcTypeDCL(self, tval);
    free(tval);

    return rval;
}

Data* visitEvent(Event* self, int indent){
    if(PPRINTFLAG == 1)
    {
        prettyPrint("Event", indent);
    }
    indent++;
    Data* rval;

    Data* scope = visitScope(self->scope, indent);
    rval = tcEvent(self, scope);
    free(scope);

    return rval;
}

Data* visitVariable(Variable* self, int indent){
    if(PPRINTFLAG == 1)
    {
        prettyPrint("Variable", indent);
    }
    indent++;
    Data* rval;
    Data* id = visitId(&self->name, indent);
    Data* type = visitType(self->type, indent);

    rval = tcVariable(self, type, id);
    free(id);
    free(type);

    return rval;
}

Data* visitPreambleBoard(PreambelBoard* self, int indent){
    return tcAccept();
}

Data* visitPreambleTileItem(PreambleTileItem* self, int indent){
    visitId(&self->name, indent);
    return tcAccept();
}

Data* visitPreambleTile(PreambelTile* self, int indent){
    yabl_list_foreach(*self->tile_items, visitId);
    return tcAccept();
}

Data* visitPreamblePlayer(PreamblePlayers* self, int indent){
    yabl_list_foreach(*self->ids, visitId);
    return tcAccept();
}


