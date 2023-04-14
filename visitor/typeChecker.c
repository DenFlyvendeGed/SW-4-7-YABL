#include "typeChecker.h"

Data* tcPreamble(Preamble* self);

//List
// Data* tcRepeatable(Repeatable* self)  //return child
// {

// }
Data* tcExprs(Exprs* self);
Data* tcStmts(Stmts* self);
Data* tcScope(Scope* self);
Data* tcArgs(Args* self);
Data* tcFuncs(Funcs* self);
Data* tcListConstant(ListConstant* self);
Data* tcPreambles(Preambles* self);


Data* tcExpr(Expr* self, Data* child)
{
    if(self == NULL)
        return createError(ECempty);
    if(child->errorCode == ECempty) {
        return createError(ECmissingChild); 
    } 
    if(self->exprType == et_constant)
    {
        //Logic
        if(*(int*)self->child <= 1 && *(int*)self->child >= 0){
            //true
            return createData((BasicTypes)logic);
        }
        else{
            return createError(ECtypeExeption);
            //false
        }

        //Number
        if(*(int*)self->child <= NUMBERMAX && *(int*)self->child >= NUMBERMIN){
            //true
            return createData((BasicTypes)number);
        }
        else{
            return createError(ECtypeExeption);
            //false
        }

        //Text
        return createData((BasicTypes)text);
        //idk what to check <-----

    }
    else {
        return child;
    }
}

//Data* tcStmt(Nonterminals* self); //return visit data
Data* tcFunc(Func* self, Data* args, Data* returntype, Data* scope, Data* id){
    if(self == NULL)
        return createError(ECempty);
    if(args->errorCode >= 0)
        return createError(args->errorCode);
    if(returntype->errorCode >= 0)
        return createError(returntype->errorCode);
    if(scope->errorCode >= 0)
        return createError(scope->errorCode);
    if(id->errorCode >= 0)
        return createError(id->errorCode);
    
    //check scope and args id's match? <--------

    //check if returntype and scope return data match
    if(returntype->type != scope->type)
    {
        return createError(ECtypeExeption);
    }

    return scope;
}

Data* tcEvent(Event* self, Data* scope){ //<---
    if(self == NULL)
        return createError(ECempty);
    if(scope->errorCode)
        return scope;
    //<---- return eventType?
    return scope;
}

Data* tcUnaryop(UnaryOperator* self, Data* expr)
{
    if(self == NULL)
        return createError(ECempty);
     if(self->uo != 0){
        return createError(ECoutOfRange);
    }

    return expr; //error or not expr contains relavent data
}
Data* tcBinaryOp(BinaryOperator* self, Data* expr1, Data* expr2){  //<---- skal samenligningen retuneres?
    if(self == NULL)
        return createError(ECempty);
    if(self->bo < 0 || self->bo > 13)
    {
        return createError(ECoutOfRange);
    }

    //2 exprs
    if(self->childExpr1 != NULL && self->childExpr2 != NULL)
    {
        //logic + logic
        if(expr1->type == logic && expr2->type == logic){
            switch (self->bo) {
                //returns logic
                case bo_not: //maybe not <----  
                case bo_eq:
                case bo_neq:
                case bo_gt:
                case bo_gteq: 
                case bo_lt:
                case bo_lteq:
                case bo_and:
                case bo_or:
                    return createData(logic);
                    //return createData(logic, void *value);
                    break;

                default:
                    return createError(ECtypeExeption);
                    //error
                    break;
            }
        }
        //number + number
        if(expr1->type == number && expr2->type == number){
            switch (self->bo) {
                //returns number
                case bo_plus:
                case bo_minus:
                case bo_divsion:
                case bo_mul:
                case bo_modulus:
                    return createData(number);
                    break;

                //returns logic
                case bo_eq:
                case bo_neq:
                case bo_gt:
                case bo_gteq:
                case bo_lt:
                case bo_lteq:
                    return createData(logic);
                    break;

                default:
                    return createError(ECoutOfRange);
                    //error
                    break;
            }
        }
        //text + text
        if(expr1->type == text && expr2->type == text){
            switch (self->bo) {
                //returns text
                case bo_plus:
                    return createData(text);
                    break;

                //returns logic
                case bo_eq:
                case bo_neq:
                    return createData(logic);
                    break;

                //returns logic based on length ? <-----
                case bo_gt:
                case bo_lt:
                case bo_gteq:
                case bo_lteq:

                default:
                    return createError(ECoutOfRange);
                    break;
            }
        }
    }
    else
    {
        return createError(ECargumentExeption);
    }
    //1 expr <-----
    // else if(self->childExpr1 != NULL)
    // {
    //     Data* child = visitExpr(self->childExpr1);

        
    // }
}

Data* tcAssign(Assign* self, Data* id, Data* expr){
    if(self == NULL)
        return createError(ECempty);
    if(id->errorCode >=0)
        return id;
    if(expr->errorCode >=0)
        return expr;
    
    //<--- need return data type that handles assign
}
Data* tcIfStmt(IfStmt* self, Data* condition, Data* thenScope, Data* elseScope){
    if(self == NULL)
        return createError(ECempty);
    if(condition->errorCode >= 0)
        return createError(condition->errorCode);
    if(thenScope->errorCode >= 0)
        return createError(thenScope->errorCode);
    if(elseScope->errorCode >= 0)
        return createError(elseScope->errorCode);

    if(condition->type != logic)
        return createError(ECtypeExeption);

    //create a fitting return type <-----
}   

//Data* tcReturnStmt(self);
Data* tcInitialization(Initialization* self, Data* type){ //might not be needed
    if(self == NULL)
        return createError(ECempty);
    if(type->errorCode >= 0)
        return createError(type->errorCode);

    return type;
}
Data* tcType(Type* self, Data* typeVal){ //might not be needed
    return typeVal;
}

Data* tcIdMutation(IdMutation* self, Data* child, Data* id){
    if(self == NULL)
        return createError(ECempty);
    if(id->errorCode >= 0)
        return createError(id->errorCode);
    if(child != NULL && child->errorCode >= 0)
        return createError(child->errorCode);

    switch (self->idMutation) {
        case im_none:
            break;
        case im_dot:
        case im_call:
        case im_index:
            if(child == NULL)
                return createError(ECmissingChild);
            break;
    }
    return tcAccept();
}
Data* tcIdMutationDot(IdMutationDot* self, Data* name, Data* idMutation) //<----
{
    return tcIdMutation((IdMutation*)self, idMutation , name);
}
Data* tcIdMutationCall(IdMutationCall* self, Data* idMutation, Data* args)
{
    return tcIdMutation((IdMutation*)self, idMutation , args); //args only used to pass error code
}
Data* tcIdMutationIndex(IdMutationIndex* self, Data* expr, Data* idMutation)
{
    return tcIdMutation((IdMutation*)self, idMutation , expr); //expr should be able to return  datatype if needed
}

Data* tcRepeat(Repeat* self, Data* loopHeader, Data* scope)
{
    if(self == NULL)
        return createError(ECempty);
    if(loopHeader->errorCode >= 0)
        return createError(loopHeader->errorCode);
    if(scope->errorCode >= 0)
        return createError(scope->errorCode);
    
    return tcAccept();
}
Data* tcTimesLoop(TimesLoop* self, Data* goalExpr)
{
    if(self == NULL)
        return createError(ECempty);
    if(goalExpr->errorCode >= 0)
        return createError(goalExpr->errorCode);
    
    return tcAccept();
}
Data* tcForLoop(ForLoop* self, Data* inputName)
{
    if(self == NULL)
        return createError(ECempty);
    if(inputName->errorCode >= 0)
        return createError(inputName->errorCode);
    
    return tcAccept();
}
Data* tcWhileLoop(WhileLoop* self, Data* cond){
    if(self == NULL)
        return createError(ECempty);
    if(cond->errorCode >= 0)
        return createError(cond->errorCode);

    return tcAccept();
}

Data* tcRepeatLoop(RepeatLoop* self){
    if(self == NULL)
        return createError(ECempty);
    return tcAccept();
}

Data* tcTypeValue(TypeValue* self, Data* list, Data* typedcl); //<---- union, der kan kun være real data i en af dem
Data* tcId(Id* self); //<----
Data* tcBasicType(BasicTypes* self); //<----- enum
Data* tcTypeDCL(Type* self, Data* typeval){
    if(self == NULL)
        return createError(ECempty);
    if(typeval->errorCode >= 0)
        return createError(typeval->errorCode);
    
    return tcAccept();
}


Data* tcVariable(Variable* self, Data* type, Data* id){
    if(self == NULL)
        return createError(ECempty);
    if(type->errorCode >= 0)
        return  createError(type->errorCode);
    if(id->errorCode >= 0)
        return createError(id->errorCode);

    return tcAccept();
}

//work in progress
Data* tcPreambleBoard(PreambelBoard* self);
Data* tcPreambleTileItem(PreambleTileItem* self);
Data* tcPreambleTile(PreambelTile* self);
Data* tcPreamblePlayer(PreamblePlayers* self);


Data*  createData(BasicTypes dType)
{
    Data*  d = malloc(sizeof(Data));
    d->type = dType;
    // d->value = value;
    d->errorCode = -1;

    return d;
};

Data* createError(ErrorCode error){
    Data* d = malloc(sizeof(Data));
    d->errorCode = error;

}

Data* tcAccept()
{
    Data*  d = malloc(sizeof(Data));
    d->errorCode = -1;

    return d;
}