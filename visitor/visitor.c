#include "visitor.h"
#include "stdlib.h"

//Jeg kan ikke huske borders :)
#define NUMBERMAX 255
#define NUMBERMIN -255
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
    Data*  d;

    if(self->child == NULL) {
        //throw error 
    } 
    switch (self->exprType)
    {
    case et_constant:
        //need a way to check type
         
        //Logic
        if(*(int*)self->child <= 1 && *(int*)self->child >= 0){
            //true
            d = createData((BasicTypes)logic, self->child);
        }
        else{
            //false
        }

        //Number
        if(*(int*)self->child <= NUMBERMAX && *(int*)self->child >= NUMBERMIN){
            //true
            d = createData((BasicTypes)number, self->child);
        }
        else{
            //false
        }

        //Text
        d = createData((BasicTypes)text, self->child);
        //idk what to check <-----

        break;
    case et_id_mutation:
        d = visitIdMutation(self->child);
        break;
    case et_unary_operator:
        d = visitUnaryop(self->child);
        break;
    case et_binary_operator:
        d = visitBinaryOp(self->child);
        break;
    case et_expression:
        d = visitExpr(self->child);
        break;
    default:
        printf("Expr type error"); //<-------------- error
        break;
    }
    return d;
}

Data* visitStmt(Nonterminals* self){
    switch (*self)
    {
    case assign:
        visitAssign((Assign*)self);
        break;
    case ifstmt:
        visitIfStmt((IfStmt*)self);
        break;
    case repeatstmt:
        visitRepeat((Repeat*)self);
        break;
    case initialization:
        visitInitialization((Initialization*)self);
        break;
    case scope:
        visitScope((Scope*)self);
        break;
    case expr:
        visitExpr((Expr*)self);
        break;
    case returnstmt: //<-----
        visitReturnStmt(self);
        break;
    default:
        break;
    }
}

Data* visitFunc(Func* self){
    switch (self->nonterminal)
    {
    case func:
        visitArgs(self->args);
        visitType(self->returntype);
        visitScope(self->scope);
        visitId(&self->name);
        break;
    case event:
        visitEvent(self);
        break;
    default:
        break;
    }
}

//--------------------------------------

Data* visitIdMutation(IdMutation* self){

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

Data* visitUnaryop(UnaryOperator* self){
    if(self->uo != 0){
        //error
    }
    visitExpr(self->childExpr);
}

Data* visitBinaryOp(BinaryOperator* self){
    
    if(self->bo < 0 || self->bo > 13)
    {
        //error
    }

    //2 exprs
    if(self->childExpr1 == NULL && self->childExpr2 == NULL)
    {

        Data* child1 = visitExpr(self->childExpr1);
        Data* child2 = visitExpr(self->childExpr2);

        //logic + logic
        if(child1->type == logic && child2->type == logic){
            switch (self->bo) {
                //returns logic
                case bo_not: //maybe not <----
                    break;   
                case bo_eq:
                    break;
                case bo_neq:
                    break;
                case bo_gt:
                    break;
                case bo_gteq:
                    break;
                case bo_lt:
                case bo_lteq:
                case bo_and:
                case bo_or:
                break;

                default:
                    //error
                    break;
            }
        }
        //number + number
        if(child1->type == number && child2->type == number){
            switch (self->bo) {
                //returns number
                case bo_plus:
                    break;
                case bo_minus:
                    break;
                case bo_divsion:
                    break;
                case bo_mul:
                    break;
                case bo_modulus:
                    break;

                //returns logic
                case bo_eq:
                    break;
                case bo_neq:
                    break;
                case bo_gt:
                    break;
                case bo_gteq:
                    break;
                case bo_lt:
                case bo_lteq:
                
                    break;
                default:
                    //error
                    break;
            }
        }
        //text + text
        if(child1->type == text && child2->type == text){
            switch (self->bo) {
                //returns text
                case bo_plus:
                    break;

                //returns logic
                case bo_eq:
                case bo_neq:
                    break;

                //returns logic based on length ? <-----
                case bo_gt:
                case bo_lt:
                case bo_gteq:
                case bo_lteq:

                default:
                    //error
                    break;
            }
        }
    }

    //1 expr
    // else if(self->childExpr1 != NULL)
    // {
    //     Data* child = visitExpr(self->childExpr1);

        
    // }
    else
    {
        //error
    }





}

Data* visitAssign(Assign* self){
    visitExpr(self->expression);
}

Data* visitIfStmt(IfStmt* self){
    visitExprs(self->condition);
    visitScope(self->then);
    visitScope(self->elsestmt);
}

Data* visitRepeat(Repeat* self){
    switch (self->nonterminal)
    {
    case lt_timesloop:
        visitTimesLoop(self->loopType);
        break;
    case lt_forloop:
        visitForLoop(self->loopType);
        break;
    case lt_whileloop:
        visitWhileLoop(self->loopType);
        break;
    case lt_repeatloop:
        visitRepeatLoop(self->loopType);
        
        break;
    
    default:
        break;
    }
    visitScope(self->scope);
}

//Mangler
Data* visitReturnStmt(self){

}

Data* visitInitialization(Initialization* self){
    visitType(self->type);
}

Data* visitType(Type* self){
    visitTypeValue(self->typeval);
}

//--------------------------------------

Data*  visitIdMutationDot(IdMutationDot* self){
    if(self->name == NULL || strcmp(self->name, "")){
        //Error <---
    }
    switch (self->childType) {
        case im_dot: 
        case im_call: 
        case im_index:
            self->child != NULL ? 1: 0;//error 
            break;
        case im_none:
            self->child != NULL ? 0: 1;//error?
            break;
        default:
            //errror
            break;
    }
    Data* d = visitIdMutation(self->child);
    //check if d has error
    return d;
}

Data*  visitIdMutationCall(IdMutationCall* self){
    
    switch (self->childType) { 
        case im_dot: 
        case im_call: 
        case im_index:
            self->child != NULL ? 1: 0;//error 
            break;
        case im_none:
            self->child != NULL ? 0: 1;//error?
            break;
        default:
            //errror
            break;
    }

    Data* d = visitIdMutation(self->child);


    //check if function require args ?
    if(self->args == NULL){
        //depends on child;
    }
    else{
        //again should depend on if child needs
        Data*  argData = visitArgs(self->args);
    }
    
}

Data* evisitIdMutationIndex(IdMutationIndex* self){


    visitExpr(self->index);
    visitIdMutation(self->child);
}

Data* visitTimesLoop(TimesLoop* self){
    visitExpr(self->goal);
}

Data* visitForLoop(ForLoop* self){
    visitId(&self->input_name);
}

Data* visitWhileLoop(WhileLoop* self){
    visitExpr(self->condition);
}

Data* visitRepeatLoop(RepeatLoop* self){
}

Data* visitTypeValue(TypeValue* self){
    //visitBasicType(self->type); enum
    visitTypeDCL(self->list);
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
    visitTypeValue(self->typeval);
}

Data* visitEvent(Event* self){
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
    visitScope(self->scope);
}

Data* visitVariable(Variable* self){
    visitId(&self->name);
    visitType(self->type);
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


Data*  createData(BasicTypes dType, void* value)
{
    Data*  d = malloc(sizeof(Data));
    d->type = dType;
    d->value = value;

    return d;
};