#include "visitor.h"
#include "stdlib.h"

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
}

Data* visitStmts(Stmts* self){
    yabl_list_foreach(*self->children,&visitStmt);
}

Data* visitScope(Scope* self){
    yabl_list_foreach(*self->children, &visitScope);
}

Data* visitArgs(Args* self){
    visitExprs(self);
}

Data* visitFuncs(Funcs* self){
    yabl_list_foreach(*self->children, &visitFunc);
}

Data* visitListConstant(ListConstant* self){
    visitExprs(self);
}

//Mangler
Data* visitPreambles(Preambles* self){

}

//--------------------------------------

Data*  visitExpr(Expr* self){


    Data* child;
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

    return tcExpr(self, child);
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
    switch (self->nonterminal)
    {
    case func:
        Data* args = visitArgs(self->args);
        Data* returnType = visitType(self->returntype);
        Data* scope = visitScope(self->scope);
        Data* id = visitId(&self->name);
        return tcFunc(self, args, returnType, scope, id);
        break;
    case event:
        return visitEvent(self);
        break;
    default:
        return createError(ECoutOfRange);
        break;
    }
}

//--------------------------------------

Data* visitIdMutation(IdMutation* self){
    Data* id = visitId(&self->name);
    Data* child;
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
    return tcIdMutation(self, child, id);
}

Data* visitUnaryop(UnaryOperator* self){
   
    visitExpr(self->childExpr);
}

Data* visitBinaryOp(BinaryOperator* self){
    Data* child1 = visitExpr(self->childExpr1);
    Data* child2 = visitExpr(self->childExpr2);
    





}

Data* visitAssign(Assign* self){
    Data* id = visitId(&self->variable);
    Data* expr = visitExpr(self->expression);

    return tcAssign(self, id, expr)
}

Data* visitIfStmt(IfStmt* self){
    Data* expr = visitExprs(self->condition);
    Data* scope1 = visitScope(self->then);
    Data* scope2 = visitScope(self->elsestmt);

    return tcIfStmt(self, expr, scope1, scope2);
}

Data* visitRepeat(Repeat* self){
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

    return tcRepeat(self, loopHeader, scope);

}

//Mangler
Data* visitReturnStmt(self){

}

Data* visitInitialization(Initialization* self){
    Data* type = visitType(self->type);

    return tcInitialization(self, type);
}

Data* visitType(Type* self){
    Data* type = visitTypeValue(self->typeval);

    return tcType(self, type);
}

//--------------------------------------

Data*  visitIdMutationDot(IdMutationDot* self){
    
    Data* name = visitId(&self->name);
    Data* child = visitIdMutation(self->child);

    return tcIdMutationDot(self, name, child);
}

Data*  visitIdMutationCall(IdMutationCall* self){
    
 
    Data* child = visitIdMutation(self->child);

    Data*  argData = visitArgs(self->args);

    return tcIdMutationCall(self, child, argData);
}

Data* visitIdMutationIndex(IdMutationIndex* self){


    Data* expr = visitExpr(self->index);
    Data* child = visitIdMutation(self->child);

    return tcIdMutationIndex(self, expr, child);
}

Data* visitTimesLoop(TimesLoop* self){
    Data* goalExpr = visitExpr(self->goal);

    return tcTimesLoop(self, goalExpr);
}

Data* visitForLoop(ForLoop* self){
    Data* id = visitId(&self->inputName);

    return tcForLoop(self, id);
}

Data* visitWhileLoop(WhileLoop* self){
    Data* expr = visitExpr(self->condition);

    return tcWhileLoop(self, expr);
}

Data* visitRepeatLoop(RepeatLoop* self){ // <--

    return tcRepeatLoop(self);
}

Data* visitTypeValue(TypeValue* self){ //<----- måske der skal laves switch for at checke hvilken der er gældne
    Data* basicType = visitBasicType(&self->type);
    Data* typeDcl = visitTypeDCL(self->list);

    return tcTypeValue(self, basicType, typeDcl)
}

//--------------------------------------

//mangler
Data* visitId(Id* self){
    
}

Data* visitBasicType(BasicTypes* self){ //det er en enum
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
    Data* tval = visitTypeValue(self->typeval);

    return tcTypeDCL(self, tval);
}

Data* visitEvent(Event* self){
    // switch (self->nonterminal)
    // {
    // case event_setup:
    //     visitFunc(self->eventType);  //eventType er enum
    //     break;
    // case event_turn:
    //     visitFunc(self->eventType);
    //     break;
    // case event_close:
    //     visitFunc(self->eventType);
    //     break;
    // default:
    //     break;
    // }
    Data* scope = visitScope(self->scope);
    return tcEvent(self, scope);
}

Data* visitVariable(Variable* self){
    Data* id = visitId(&self->name);
    Data* type = visitType(self->type);

    return tcVariable(self, type, id);
}

Data* visitPreambleBoard(PreambelBoard* self){

}

Data* visitPreambleTileItem(PreambleTileItem* self){
    visitId(&self->name);
}

Data* visitPreambleTile(PreambelTile* self){
    yabl_list_foreach(*self->tile_items, visitId);
}

Data* visitPreamblePlayer(PreamblePlayers* self){
    yabl_list_foreach(*self->ids, visitId);
}


