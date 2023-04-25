int TYPE_CHECKER_ERROR_COUNT;

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
        Constant* child = self->child;
        switch (child->typeDcl) {
            case td_logic:
                 //Logic
                if(*(int*)child->value <= 1 && *(int*)child->value >= 0){    
                    return createData((BasicTypes)bt_logic);
                }
                else {
                    return createError(ECtypeExeption);
                }
                break;
            case td_number:
                if(*(int*)self->child <= NUMBERMAX && *(int*)self->child >= NUMBERMIN){
                    return createData((BasicTypes)bt_number);
                }   
                else {
                    return createError(ECtypeExeption);
                }
                break;
            case td_text:
                return createData((BasicTypes)bt_text); //<--- check om det er en streng?
                break;
            default:
                return createError(ECoutOfRange);
        }

    }
    else {
        return createData(child->type);
    }
}

Data* tcStmt(Stmt* self, Data* child)
{
    return tcAccept();
}
Data* tcFunc(Func* self, Data* args, Data* returntype, Data* scope, Data* id){
    if(self == NULL)
        return createError(ECempty);
    if(args->errorCode != ECnoError)
        return createError(args->errorCode);
    if(returntype->errorCode != ECnoError)
        return createError(returntype->errorCode);
    if(scope->errorCode != ECnoError)
        return createError(scope->errorCode);
    if(id->errorCode != ECnoError)
        return createError(id->errorCode);
    
    //check scope and args id's match? <--------

    //return type is checked in the scope
    // if(returntype->type != scope->type)
    // {
    //     return createError(ECtypeExeption);
    // }

    return scope;
}

Data* tcEvent(Event* self, Data* scope){ //<---
    if(self == NULL){
        free(scope);
        return createError(ECempty);
    }
        
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
        if(expr1->type == bt_logic && expr2->type == bt_logic){
            switch (self->bo) {
                //returns logic
                case bo_eq:
                case bo_neq:
                // case bo_gt:
                // case bo_gteq: 
                // case bo_lt:
                // case bo_lteq:
                case bo_and:
                case bo_or:
                    return createData(bt_logic);
                    //return createData(logic, void *value);
                    break;

                default:
                    return createError(ECtypeExeption);
                    //error
                    break;
            }
        }
        //number + number
        if(expr1->type == bt_number && expr2->type == bt_number){
            switch (self->bo) {
                //returns number
                case bo_plus:
                case bo_minus:
                case bo_division:
                case bo_mul:
                case bo_modulus:
                    return createData(bt_number);
                    break;

                //returns logic
                case bo_eq:
                case bo_neq:
                case bo_gt:
                case bo_gteq:
                case bo_lt:
                case bo_lteq:
                    return createData(bt_logic);
                    break;

                default:
                    return createError(ECoutOfRange);
                    //error
                    break;
            }
        }
        //text + text
        if(expr1->type == bt_text && expr2->type == bt_text){
            switch (self->bo) {
                //returns text
                case bo_plus:
                    return createData(bt_text);
                    break;

                //returns logic
                case bo_eq:
                case bo_neq:
                    return createData(bt_logic);
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
        else {
            return createError(ECtypeExeption);
        }
    }
    else
    {
        return createError(ECargumentExeption);
    }

	return tcAccept();
}

Data* tcAssign(Assign* self, Data* id, Data* expr){
    if(self == NULL)
        return createError(ECempty);
    if(id->errorCode >=0)
        return id;
    if(expr->errorCode >=0)
        return expr;
    
	return tcAccept();
}
Data* tcIfStmt(IfStmt* self, Data* condition, Data* thenScope, Data* elseScope){
    if(self == NULL)
        return createError(ECempty);
    if(condition->errorCode != ECnoError)
        return createError(condition->errorCode);
    if(thenScope->errorCode != ECnoError)
        return createError(thenScope->errorCode);
    if(elseScope->errorCode != ECnoError)
        return createError(elseScope->errorCode);

    if(condition->type != bt_logic)
        return createError(ECtypeExeption);

	return tcAccept();
}   

//Data* tcReturnStmt(self);
Data* tcInitialization(Initialization* self, Data* type, Data* val){ //might not be needed
    if(self == NULL){
        return createError(ECempty);
    }
    else if(type->errorCode != ECnoError)
        return createError(type->errorCode);
    else if(val->errorCode != ECnoError)
        return createError(type->errorCode);

    if(type->type != val->type)
        return createError(ECtypeExeption);

    return type;
}
Data* tcType(Type* self, Data* typeVal){ //might not be needed
    return typeVal;
}

Data* tcIdMutation(IdMutation* self, Data* child, Data* id){
    if(self == NULL)
        return createError(ECempty);
    if(id->errorCode != ECnoError)
        return createError(id->errorCode);
    if(child != NULL && child->errorCode != ECnoError)
        return createError(child->errorCode);

    switch (*((IdMutations*)(self->child))) {
        case im_none:
		case im_value:
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
    if(loopHeader->errorCode != ECnoError)
        return createError(loopHeader->errorCode);
    if(scope->errorCode != ECnoError)
        return createError(scope->errorCode);
    
    return tcAccept();
}
Data* tcTimesLoop(TimesLoop* self, Data* goalExpr)
{
    if(self == NULL)
        return createError(ECempty);
    if(goalExpr->errorCode != ECnoError)
        return createError(goalExpr->errorCode);
    
    return tcAccept();
}
Data* tcForLoop(ForLoop* self, Data* inputName)
{
    if(self == NULL)
        return createError(ECempty);
    if(inputName->errorCode != ECnoError)
        return createError(inputName->errorCode);
    
    return tcAccept();
}
Data* tcWhileLoop(WhileLoop* self, Data* cond){
    if(self == NULL)
        return createError(ECempty);
    if(cond->errorCode != ECnoError)
        return createError(cond->errorCode);

    return tcAccept();
}

Data* tcRepeatLoop(RepeatLoop* self){
    if(self == NULL)
        return createError(ECempty);
    return tcAccept();
}

Data* tcTypeValue(TypeValue* self, Data* list, Data* typedcl){
	return tcAccept();
}
Data* tcId(Id* self); //<----
Data* tcBasicType(BasicTypes* self); //<----- enum
Data* tcTypeDCL(Type* self, Data* typeval){
    if(self == NULL)
        return createError(ECempty);
    if(typeval->errorCode != ECnoError)
        return createError(typeval->errorCode);
    
    return tcAccept();
}


Data* tcVariable(Variable* self, Data* type, Data* id){
    if(self == NULL)
        return createError(ECempty);
    if(type->errorCode != ECnoError)
        return  createError(type->errorCode);
    if(id->errorCode != ECnoError)
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
    printf("\n dataType: %d\n", dType);
    Data*  d = malloc(sizeof(Data));
    d->type = dType;
    // d->value = value;
    d->errorCode = ECnoError;

    return d;
};

Data* createError(ErrorCode error){
    TYPE_CHECKER_ERROR_COUNT++;
    printf("\nerror %i\n", error);
    Data* d = malloc(sizeof(Data));
    d->errorCode = error;
	return d;
}

Data* tcAccept()
{
    Data*  d = malloc(sizeof(Data));
    d->errorCode = ECnoError;

    return d;
}
