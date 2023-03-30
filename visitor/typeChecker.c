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
    if(self->child == NULL) {
        return createError(ECmissingChild); 
    } 
    if(self->exprType == et_constant)
    {
        //Logic
        if(*(int*)self->child <= 1 && *(int*)self->child >= 0){
            //true
            return createData((BasicTypes)logic, self->child);
        }
        else{
            //false
        }

        //Number
        if(*(int*)self->child <= NUMBERMAX && *(int*)self->child >= NUMBERMIN){
            //true
            return createData((BasicTypes)number, self->child);
        }
        else{
            //false
        }

        //Text
        return createData((BasicTypes)text, self->child);
        //idk what to check <-----

    }
    else {
        return child;
    }
}

//Data* tcStmt(Nonterminals* self); //return visit data
Data* tcFunc(Func* self, Data* args, Data* returntype, Data* scope, Data* id){
    //check for error further down
    if(args->errorCode >= 0)
        return args;
    if(returntype->errorCode >= 0)
        return returntype;
    if(scope->errorCode >= 0)
        return scope;
    if(id->errorCode >= 0)
        return id;
    
    //check scope and args id's match? <--------

    //check if returntype and scope return data match
    if(returntype->type != scope->type)
    {
        return createError(ECtypeExeption);
    }

    return scope;
}

Data* tcEvent(Event* self, Data* scope){ //<---
    if(scope->errorCode)
        return scope;
    //<---- return eventType?
    return scope;
}

Data* tcUnaryop(UnaryOperator* self, Data* expr)
{
     if(self->uo != 0){
        return createError(ECoutOfRange);
    }

    return expr; //error or not expr contains relavent data
}
Data* tcBinaryOp(BinaryOperator* self, Data* expr1, Data* expr2){  //<---- skal samenligningen retuneres?

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
                    //return createData(logic, void *value);
                    break;

                default:
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
                    break;

                //returns logic
                case bo_eq:
                case bo_neq:
                case bo_gt:
                case bo_gteq:
                case bo_lt:
                case bo_lteq:
                
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
    if(id->errorCode >=0)
        return id;
    if(expr->errorCode >=0)
        return expr;
    
    //<--- need return data type that handles assign
}
Data* tcIfStmt(IfStmt* self, Data* condition, Data* thenScope, Data* elseScope){
    if(condition->errorCode >= 0)
        return condition;
    if(thenScope->errorCode >= 0)
        return thenScope;
    if(elseScope->errorCode >= 0)
        return elseScope;

    if(condition->type != logic)
        return createError(ECtypeExeption);

    //create a fitting return type <-----
}   

//Data* tcReturnStmt(self);
Data* tcInitialization(Initialization* self, Data* type){ //might not be needed
    if(type->errorCode >= 0)
        return type;

    return type;
}
Data* tcType(Type* self, Data* typeVal){ //might not be needed
    return typeVal;
}

Data* tcIdMutation(IdMutation* self, Data* child);
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


Data*  createData(BasicTypes dType, void* value)
{
    Data*  d = malloc(sizeof(Data));
    d->type = dType;
    d->value = value;
    d->errorCode = -1;

    return d;
};

Data* createError(ErrorCode error){
    Data* d = malloc(sizeof(Data));
    d->errorCode = error;

}