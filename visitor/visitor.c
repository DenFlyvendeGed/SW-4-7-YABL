#include "visitor.h"
#include "stdlib.h"
#include <stdio.h>

//Visitor function
Data* visitor(){
}

//Visit function
Data* visit(self){
    visitPreamble(self.preamble);
    visitRepeatable(self.func);
}

//--------------------------------------

//Mangler
Data* visitPreamble(Preamble* self){
    return tcAccept();
}

Data* visitRepeatable(Repeatable* self){
    
    switch (self->nonterminal)
    {
    case exprs:
        return visitExprs(self);
        break;
    case stmts:
        return visitStmts(self);
        break;
    case scope:
        return visitScope(self);
        break;
    case args:
        return visitArgs(self);
        break;
    case funcs:
        return visitFuncs(self);
        break;
    case listConstants:
        return visitListConstant(self);
        break; 
    case preambles:
        return visitPreambles(self);
        break;
    default:
        break;
    }

}

Data* visitExprs(Exprs* self){
    yabl_list_foreach(*self->children, &visitExpr);
    return tcAccept(); //<----
}

Data* visitStmts(Stmts* self){
    yabl_list_foreach(*self->children,&visitStmt);
    return tcAccept(); //<----
}

Data* visitScope(Scope* self){
    yabl_list_foreach(*self->children, &visitScope);
    return tcAccept(); //<----
}

Data* visitArgs(Args* self){
    visitExprs(self);
    return tcAccept(); //<----
}

Data* visitFuncs(Funcs* self){
    yabl_list_foreach(*self->children, &visitFunc);
    return tcAccept(); //<----
}

Data* visitListConstant(ListConstant* self){
    visitExprs(self);
    return tcAccept(); //<----
}

//Mangler
Data* visitPreambles(Preambles* self){
    return tcAccept(); //<----
}

//--------------------------------------

Data*  visitExpr(Expr* self){


    Data* child;
    Data* rval;
    switch (self->exprType)
    {
    case et_constant:
        //need a way to check type
         
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
    return rval;
}

Data* visitStmt(Nonterminals* self){
    switch (*self)
    {
    case assign:
        return visitAssign((Assign*)self);
        break;
    case ifstmt:
        return visitIfStmt((IfStmt*)self);
        break;
    case repeatstmt:
        visitRepeat((Repeat*)self);
        break;
    case initialization:
        return visitInitialization((Initialization*)self);
        break;
    case scope:
        return visitScope((Scope*)self);
        break;
    case expr:
        return visitExpr((Expr*)self);
        break;
    case returnstmt: //<-----
        return visitReturnStmt(self);
        break;
    default:
        return createError(ECoutOfRange);
        break;
    }

}

Data* visitFunc(Func* self){
    Data* rval;
    switch (self->nonterminal)
    {
    case func:
        Data* args = visitArgs(self->args);
        Data* returnType = visitType(self->returntype);
        Data* scope = visitScope(self->scope);
        Data* id = visitId(&self->name);
        rval = tcFunc(self, args, returnType, scope, id);
        free(args);
        free(returnType);
        free(scope);
        free(id);
        break;
    case event:
        rval = visitEvent(self);
        break;
    default:
        return createError(ECoutOfRange);
        break;
    }
    return rval;
}

//--------------------------------------

Data* visitIdMutation(IdMutation* self){
    Data* id = visitId(&self->name);
    Data* child;
    Data* rval;
    switch (self->idMutation)
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
    return rval;
}

Data* visitUnaryop(UnaryOperator* self){
    Data* rval;
    Data* expr = visitExpr(self->childExpr);

    rval = tcUnaryop(self, expr);
    free(expr);
    return rval;
}

Data* visitBinaryOp(BinaryOperator* self){
    Data* rval;
    Data* child1 = visitExpr(self->childExpr1);
    Data* child2 = visitExpr(self->childExpr2);
    

    rval = tcBinaryOp(self, child1, child2);
    free(child1);
    free(child2);

    return rval;
}

Data* visitAssign(Assign* self){
    Data* rval;
    Data* id = visitId(&self->variable);
    Data* expr = visitExpr(self->expression);

    rval = tcAssign(self, id, expr);
    free(id);
    free(expr);

    return rval;
}

Data* visitIfStmt(IfStmt* self){
    Data* rval;
    Data* expr = visitExprs(self->condition);
    Data* scope1 = visitScope(self->then);
    Data* scope2 = visitScope(self->elsestmt);


    rval = tcIfStmt(self, expr, scope1, scope2);
    free(expr);
    free(scope1);
    free(scope2);

    return rval;
}

Data* visitRepeat(Repeat* self){
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
    Data* scope = visitScope(self->scope);

    rval = tcRepeat(self, loopHeader, scope);
    free(loopHeader);
    free(scope);

    return rval;
}

//Mangler <-----
Data* visitReturnStmt(self){
    return tcAccept();
}

Data* visitInitialization(Initialization* self){
    Data* rval;
    Data* type = visitType(self->type);

    rval = tcInitialization(self, type);
    free(type);

    return rval;
}

Data* visitType(Type* self){
    Data* rval;
    Data* type = visitTypeValue(self->typeval);

    rval = tcType(self, type);
    free(type);
    return rval;
}

//--------------------------------------

Data*  visitIdMutationDot(IdMutationDot* self){
    Data* rval;
    
    Data* name = visitId(&self->name);
    Data* child = visitIdMutation(self->child);

    rval = tcIdMutationDot(self, name, child);
    free(name);
    free(child);

    return rval;
}

Data*  visitIdMutationCall(IdMutationCall* self){
    Data* rval;
 
    Data* child = visitIdMutation(self->child);
    Data*  argData = visitArgs(self->args);

    rval = tcIdMutationCall(self, child, argData);
    free(child);
    free(argData);
    
    return rval;
}   

Data* visitIdMutationIndex(IdMutationIndex* self){
    Data* rval;

    Data* expr = visitExpr(self->index);
    Data* child = visitIdMutation(self->child);

    rval = tcIdMutationIndex(self, expr, child);
    free(expr);
    free(child);

    return rval;
}

Data* visitTimesLoop(TimesLoop* self){
    Data* rval;
    Data* goalExpr = visitExpr(self->goal);

    rval = tcTimesLoop(self, goalExpr);
    free(goalExpr);

    return rval;
}

Data* visitForLoop(ForLoop* self){
    Data* rval;
    Data* id = visitId(&self->inputName);

    rval = tcForLoop(self, id);
    free(id);

    return rval;
}

Data* visitWhileLoop(WhileLoop* self){
    Data* rval;
    Data* expr = visitExpr(self->condition);

    rval = tcWhileLoop(self, expr);
    free(expr);

    return rval;
}

Data* visitRepeatLoop(RepeatLoop* self){ // <--

    return tcRepeatLoop(self);
}

Data* visitTypeValue(TypeValue* self){ //<----- måske der skal laves switch for at checke hvilken der er gældne
    Data* rval;
    Data* basicType = visitBasicType(&self->type);
    Data* typeDcl = visitTypeDCL(self->list);

    rval = tcTypeValue(self, basicType, typeDcl);
    free(basicType);
    free(typeDcl);

    return rval;
}

//--------------------------------------

//mangler
Data* visitId(Id* self){ //<---
    return tcAccept();
}

Data* visitBasicType(BasicTypes* self){ //det er en enum <---
    switch (*self)
    {
    case number:
        visitTypeDCL(self);
        break;
    case text:
        visitTypeDCL(self);
        break;
    case logic:
        visitTypeDCL(self);
        break;
    default:
        break;
    }
}

Data* visitTypeDCL(Type* self){
    Data* rval;
    Data* tval = visitTypeValue(self->typeval);

    rval = tcTypeDCL(self, tval);
    free(tval);

    return rval;
}

Data* visitEvent(Event* self){
    Data* rval;

    Data* scope = visitScope(self->scope);
    rval = tcEvent(self, scope);
    free(scope);

    return rval;
}

Data* visitVariable(Variable* self){
    Data* rval;
    Data* id = visitId(&self->name);
    Data* type = visitType(self->type);

    rval = tcVariable(self, type, id);
    free(id);
    free(type);

    return rval;
}

Data* visitPreambleBoard(PreambelBoard* self){
    return tcAccept();
}

Data* visitPreambleTileItem(PreambleTileItem* self){
    visitId(&self->name);
    return tcAccept();
}

Data* visitPreambleTile(PreambelTile* self){
    yabl_list_foreach(*self->tile_items, visitId);
    return tcAccept();
}

Data* visitPreamblePlayer(PreamblePlayers* self){
    yabl_list_foreach(*self->ids, visitId);
    return tcAccept();
}


