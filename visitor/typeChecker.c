int TYPE_CHECKER_ERROR_COUNT;

#include "typeChecker.h"

Data* tcPreamble(Preambles* self);

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
    // if(self->nonterminal == listConstant){ //Exprs seem to return bt_number

    // }
    if(self->exprType == et_constant)
    {
        Constant* child = self->child;
        if(child->value == NULL)   
            return createError(ECtypeExeption);
            
        switch (child->typeDcl) {
            case td_logic:            
                    return createData((BasicTypes)bt_logic);
                break;
            case td_number:
                    return createData((BasicTypes)bt_number);
                break;
            case td_text:
                    return createData((BasicTypes)bt_text); //<--- check om det er en streng?
                
                break;
            default:
                return createError(ECoutOfRange);
        }

    }
    else {
        return tcCopy(child);
    }
}

Data* tcStmt(Stmt* self, Data* child)
{
    return tcAccept();
}
Data* tcFunc(Func* self, Data* args, Data* returntype, Data* scope, Data* id){
    if(self == NULL)
        return createError(ECempty);
    if(args != NULL && args->errorCode != ECnoError)
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

    return returntype;
}

Data* tcEvent(Event* self, Data* scope){ 
    if(self == NULL){
        //free(scope); would most likely crash
        return createError(ECempty);
    }
        
    if(scope->errorCode)
        return scope;

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
Data* tcBinaryOp(BinaryOperator* self, Data* expr1, Data* expr2){
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

Data* tcTypeCast(TypeCast* self, Data* expr, Data* type){
    if(expr != NULL){

        if(expr->errorCode != ECnoError)
            return createError(expr->errorCode);
    }
    else
        return createError(ECmissingChild);
    if (type != NULL) {
        if(type->errorCode != ECnoError)
            return createError(expr->errorCode);
    }
    else{
        return createError(ECmissingChild);
    }
    return type;
}

Data* tcAssign(Assign* self, Data* id, Data* expr){
    if(self == NULL)
        return createError(ECempty);
    if(id->errorCode !=ECnoError)
        return createError(id->errorCode);
    if(expr->errorCode !=ECnoError)
        return createError(expr->errorCode);
    if(id->type != expr->type){
        prettyPrint("Attempting to assign new type to variable");
        return createError(ECtypeExeption);
    }
    Data* rval = createData(id->type);
	return rval;
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

    if(condition->type != bt_logic){
        prettyPrint("If condition must return logic");
        return createError(ECtypeExeption);
    }
    int a = 5;
	return tcAccept();
}   

//Data* tcReturnStmt(self);
Data* tcInitialization(Initialization* self, Data* id, Data* type, Data* val){ //might not be needed
    if(self == NULL){
        return createError(ECempty);
        
    }
    else if(type->errorCode != ECnoError)
        return createError(type->errorCode);

    Data* var = symbolTableGetLocal(id->value);
    if(val != NULL){
        if(val->errorCode != ECnoError)
            return createError(type->errorCode);

        if(type->type != val->type)
            return createError(ECtypeExeption);
        if(var == NULL)
            symbolTablePush(id->value, tcCopy(val));
        else
            return createError(ECnameSpaceClash);

    }
    else {
        if(var == NULL)
            symbolTablePush(id->value, tcCopy(type));
        else
            return createError(ECnameSpaceClash);
    }
    free(var);
    return tcCopy(type);
}
Data* tcType(Type* self, Data* typeVal){ //might not be needed
    if(typeVal== NULL)
        return createData(bt_NULL);
    return typeVal;
}

Data* tcCmpArgs(Data* list1, Data* list2){
    Data* rval;
    if(list1 != NULL && list2 != NULL){
        if(list1->type == list2->type){
            rval = tcCmpArgs(list1->list, list2->list);
        }
    }
    else{
        if(list1 == NULL ^ list2 == NULL){
            return createError(ECargumentExeption);
        }
        return tcAccept();
    }
    return rval;
}

Data* tcIdMutation(IdMutation* self, Data* child, Data* id){
    if(self == NULL)
        return createError(ECempty);
    if(id->errorCode != ECnoError)
        return createError(id->errorCode);

    Data* var = symbolTableGet((id->value));

    if(self->child != NULL){
        switch (*((IdMutations*)self->child)) {
             case im_call:
                // Data* args;
                
                Data* args = tcListTypeCheck(var)->list;
                
                
                if(tcCmpArgs(args, child)->errorCode != ECnoError)
                    return createError(ECargumentExeption);
                break;

            case im_none:
            case im_value:
                break;
            case im_dot:
            case im_index:
                if(var->type != bt_list)
                    return createError(ECtypeExeption);
                if(child == NULL)
                    return createError(ECmissingChild);
                var = tcCopy(child);
                break;
        }
    }
    Data* rval = var;
    // tcListTypeCheck(rval);
    
    return rval; 
}
Data* tcIdMutationDot(IdMutationDot* self, Data* idMutation) //<----
{
    if(self == NULL)
        tcAccept();
    if(idMutation == NULL){
        prettyPrint("IdMutationDot");
        createError(ECempty);
    }
    if(idMutation->errorCode != ECnoError){
        return createError(idMutation->errorCode);
    }

    return tcCopy(idMutation);
}
Data* tcIdMutationCall(IdMutationCall* self, Data* idMutation, Data* args)
{
     if(self == NULL)
        tcAccept();

    if(idMutation->errorCode != ECnoError){
        prettyPrint("IdMutationCall");
        return createError(idMutation->errorCode);
    }
    if(args != NULL && args->errorCode != ECnoError)
        return createError(args->errorCode);

    return tcCopy(args);
}
Data* tcIdMutationIndex(IdMutationIndex* self, Data* expr, Data* child, Data* type)
{
    if(self == NULL)
        tcAccept();

    if(child->errorCode){
        return createError(child->errorCode);
    }
    if(expr->errorCode != ECnoError)
        return createError(expr->errorCode);
    if(tcListTypeCheck(expr)->type != bt_number){
        prettyPrint("Index must be number");
        return createError(ECtypeExeption); 
    }

    return tcCopy(child); //expr should be able to return  datatype if needed
}

Data* tcListTypeCheck(Data* list){
    Data* rval;

    if(list->type == bt_list){
       rval = tcListTypeCheck(list->list);
    }
    else{
        return list;
    }
    return rval;
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


Data* tcId(Id self){
    pIndent();
    printf("\033[0;32m");
    printf("Id: %s\n", self);
    printf("\033[0m");

    return tcValue(self);
}; //<----
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
Data* tcPreambleBoard(PreambleBoard* self);
Data* tcPreambleTileItem(Type* self);
Data* tcPreambleTile(PreambleTile* self);
Data* tcPreamblePlayer(PreamblePlayers* self);


Data*  createData(BasicTypes dType)
{
    char* type;
    switch(dType){
        case bt_logic:
            type = "logic";
            break;
        case bt_number:
            type = "number";
            break;
        case bt_text:
            type = "text";
            break;
        case bt_NULL:
            type = "NULL";
            break;
        case bt_list:
            type = "List";
            break;
        case bt_unset:
            type = "Unset";
            break;
        case bt_custom:
            type = "custom";
            break;
        default:
            type = "Invalid type";
            createError(ECtypeExeption);
    };
    char msg[25] = "Data type: ";
    printf("\033[0;36m");
    strcat(msg ,type);
    prettyPrint(msg);
    // printf("\n");
    printf("\033[0m");
    Data*  d = malloc(sizeof(Data));
    d->type = dType;
    d->value = NULL;
    d->errorCode = ECnoError;

    return d;
};

Data* createList(Data* child){
    char* type = "List";
    char msg[20] = "data type: ";
    printf("\033[0;36m");
    strcat(msg ,type);
    prettyPrint(msg);
    printf("\n");
    printf("\033[0m");
    Data*  d = malloc(sizeof(Data));
    d->type = bt_list;
    d->list = child;
    // d->value = value;
    d->errorCode = ECnoError;

    return d;
}

Data* createError(ErrorCode error){
    TYPE_CHECKER_ERROR_COUNT++;
    char* val;
    switch(error){
    case ECnoError:
        val = "no error";
        break;
    case ECempty:
        val = "empty";
        break;
    case ECargumentExeption:
        val = "argument exception";
        break;
    case ECtypeExeption:
        val = "type exception";
        break;
    case ECmissingChild:
        val = "missing child";
        break;
    case ECoutOfRange:
        val = "out of range";
        break;
    case ECoutOfNamespace:
        val = "outOfNamespace";
        break;
    case ECnameSpaceClash:
        val = "nameSpaceClash";
        break;
    default:
        val = "Your an idiot";
    }
    char msg[30] = "error: ";

    strcat(msg, val);
    printf("\033[0;31m");
    prettyPrint(msg);
    printf("\033[0m");
    printf("\n");
    Data* d = malloc(sizeof(Data));
    d->errorCode = error;
    d->list = NULL;
    d->type = bt_unset;
	return d;
}

Data* tcAccept()
{
    Data*  d = malloc(sizeof(Data));
    d->errorCode = ECnoError;
    d->type = bt_unset;
    d->list = NULL;
    return d;
}

Data* tcCopy(Data*in){
    if(in != NULL){

        Data* d = malloc(sizeof(Data));
        d->errorCode = in->errorCode;
        // d->nonterminal = in->nonterminal;
        d->type = in->type;
        d->value = in->value;
        if(in->type == bt_list){
            d->list = tcCopy(in->list);
        }
        else{
            d->list = in->list;
        }
        return d;
    }
    return NULL;
}

Data* tcValue(void* val){
    Data*  d = malloc(sizeof(Data));
    d->errorCode = ECnoError;
    d->value = val;
    d->list = NULL;
    d->type = bt_unset;
    return d;
}

//Symbol table setup

int stringHash(char* string){
    return (int)(strlen(string) % 10);
}
int stringcompare(char* s1, char* s2){
    return strcmp(s1, s2) == 0;
}

void symbolTablePush( char* key, void* value){
     yablHashPush(SYMBOL_TABLE, key, value, &stringcompare);
}

Data* symbolTableGet(char* key){//<-- 
    void* value = ((YablHashNode*)yablHashGet(SYMBOL_TABLE, key, &stringcompare));
    // if(value == NULL)
    //     return createError(ECoutOfNamespace);
    
    YablHash* table = SYMBOL_TABLE;
    while(value == NULL){
        table = yablHashGet(table, "PARENT", &stringcompare);
        if(table == NULL)
            return createError(ECoutOfNamespace);
        table = ((YablHashNode*)table)->item;
        value = yablHashGet(table, key, &stringcompare);

    }
    value = ((YablHashNode*)value)->item;

    return tcCopy(value);
}

// Data* symbolTableCheckParent(YablHash table, )

Data* symbolTableGetLocal(char* key){//check for prexisting keys
    void* value = ((YablHashNode*)yablHashGet(SYMBOL_TABLE, key, &stringcompare));
    if(value == NULL)
        return NULL;
    value = ((YablHashNode*)value)->item;

    return tcCopy(value);
}

void createSymbolTable(){ //creates SYMBOL_TABLE and sets parent table to PARENT, and sets global pointer to new table.

    YablHash* st = malloc(sizeof(YablHash));
    *st  = yablHashCreate(HASHLISTLENGTH, &stringHash);
    YablHash* temp = SYMBOL_TABLE;
    SYMBOL_TABLE = st;
    symbolTablePush("PARENT", temp);

}

void deleteSymbolTable(){
    YablHash* parent = yablHashGet(SYMBOL_TABLE, "PARENT", &stringcompare);
    if(parent== NULL){
        prettyPrint("No parent SYMBOL_TABLE found\n");
        createError(ECoutOfRange);
        return;
    }
    YablHash* temp = ((YablHashNode*)parent)->item;
    yablHashDelete(SYMBOL_TABLE, symbolFreeFunc);
    free(SYMBOL_TABLE); //release SYMBOL_TABLE for scope
    SYMBOL_TABLE = temp; //point to parent scope table
    
}

void symbolFreeFunc(void* item){
    if(((YablHash*)item)->hashFunc != stringHash)
        free(item);
}