#include "visitor.h"

//Visitor function
void visitor(){
}

//Visit function
void visit(self){
    visitPreamble(self.preamble);
    visitRepeatable(self.func);
}

//--------------------------------------

//Mangler
void visitPreamble(Preamble* self){
}

void visitRepeatable(Repeatable* self){
    switch (self->nonterminal)
    {
    case exprs:
        visitExprs(self);
        break;
    case stmts:
        visitStmts(self);
        break;
    case scope:
        visitScope(self);
        break;
    case args:
        visitArgs(self);
        break;
    case funcs:
        visitFuncs(self);
        break;
    case listConstants:
        visitListConstant(self);
        break; 
    case preambles:
        visitPreambles(self);
        break;
    default:
        break;
    }
}

void visitExprs(Exprs* self){
    yabl_list_foreach(self->children, &visitExpr);
}

void visitStmts(Stmts* self){
    yabl_list_foreach(self->children,&visitStmt);
}

void visitScope(Scope* self){
    yabl_list_foreach(self->children, &visitScope);
}

void visitArgs(Args* self){
    visitExprs(self);
}

void visitFuncs(Funcs* self){
    yabl_list_foreach(self->children, &visitFunc);
}

void visitListConstant(ListConstant* self){
    visitExprs(self);
}

//Mangler
void visitPreambles(Preambles* self){

}

//--------------------------------------

void visitExpr(Expr* self){
    switch (self->exprType)
    {
    case et_constant:
        break;
    case et_id_mutation:
        visitIdMutation(self->child);
        break;
    case et_unary_operator:
        visitUnaryop(self->child);
        break;
    case et_binary_operator:
        visitBinaryOp(self->child);
        break;
    case et_expressoin:
        visitExpr(self->child);
        break;
    default:
        break;
    }
}

void visitStmt(Nonterminals* self){
    switch (*self)
    {
    case assign:
        visitAssign(self);
        break;
    case ifstmt:
        visitIfStmt(self);
        break;
    case repeatstmt:
        visitRepeat(self);
        break;
    case initialization:
        visitInitialization(self);
        break;
    case scope:
        visitScope(self);
        break;
    case expr:
        visitExpr(self);
        break;
    case returnstmt:
        visitReturnStmt(self);
        break;
    default:
        break;
    }
}

void visitFunc(Func* self){
    switch (self->nonterminal)
    {
    case func:
        visitArgs(&self->args);
        visitType(&self->returntype);
        visitScope(&self->scope);
        visitIdMutation(&self->name);
        break;
    case event:
        visitEvent(self);
        break;
    default:
        break;
    }
}

//--------------------------------------

void visitIdMutation(IdMutation* self){
    switch (self->idMutation)
    {
    case im_none:
        break;
    case im_dot:
        visitIdMutationDot(self->child);
        break;
    case im_call:
        visitIdMutationCall(self->child);
        break;
    case im_index:
        visitIdMutationIndex(self->child);
        break;
    default:
        break;
    }
}

void visitUnaryop(UnaryOperator* self){
    visitExpr(&self->child_expr);
}

void visitBinaryOp(BinaryOperator* self){
    visitExpr(&self->child_expr1);
    visitExpr(&self->child_expr2);
}

void visitAssign(Assign* self){
    visitExpr(&self->expression);
}

void visitIfStmt(IfStmt* self){
    visitExpr(&self->condition);
    visitScope(&self->then);
    visitScope(&self->elsestmt);
}

void visitRepeat(Repeat* self){
    switch (self->nonterminal)
    {
    case lt_timesloop:
        visitTimesLoop(self->loop_type);
        break;
    case lt_forloop:
        visitForLoop(self->loop_type);
        break;
    case lt_whileloop:
        visitWhileLoop(self->loop_type);
        break;
    case lt_repeatloop:
        visitRepeatLoop(self->loop_type);
        
        break;
    
    default:
        break;
    }
    visitScope(&self->scope);
}

//Mangler
void visitReturnStmt(self){

}

void visitInitialization(Initialization* self){
    visitType(&self->type);
}

void visitType(Type* self){
    visitTypeValue(&self->typeval);
}

//--------------------------------------

void visitIdMutationDot(IdMutationDot* self){
    visitIdMutation(self->child_type);
}

void visitIdMutationCall(IdMutationCall* self){
    visitArgs(&self->args);
    visitIdMutation(self->child_type);
}

void visitIdMutationIndex(IdMutationIndex* self){
    visitExpr(&self->index);
    visitIdMutation(self->child_type);
}

void visitTimesLoop(TimesLoop* self){
    visitExpr(&self->goal);
}

void visitForLoop(ForLoop* self){
    visitId(self->input_name);
}

void visitWhileLoop(WhileLoop* self){
    visitExpr(&self->condition);
}

void visitRepeatLoop(RepeatLoop* self){
}

void visitTypeValue(TypeValue* self){
    visitBasicType(self->type);
    visitTypeDCL(self->list);
}

//--------------------------------------

//mangler
void visitId(Id* self){
    
}

void visitBasicType(BasicTypes* self){
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

void visitTypeDCL(Type* self){
    visitTypeValue(&self->typeval);
}

void visitEvent(Event* self){
    switch (self->nonterminal)
    {
    case event_setup:
        visitFunc(self->eventType);
        break;
    case event_turn:
        visitFunc(self->eventType);
        break;
    case event_close:
        visitFunc(self->eventType);
        break;
    default:
        break;
    }
    visitScope(&self->scope);
}

void visitVariable(Variable* self){
    visitId(self->name);
    visitType(&self->type);
}

void visitPreambleBoard(PreambelBoard* self){

}

void visitPreambleTileItem(PreambleTileItem* self){
    visitId(self->name);
}

void visitPreambleTile(PreambelTile* self){
    yabl_list_foreach(self->tile_items, &visitId);
}

void visitPreamblePlayer(PreamblePlayers* self){
    yabl_list_foreach(self->ids, &visitId);
}

