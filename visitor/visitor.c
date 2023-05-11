#include "visitor.h"
#include "typeChecker.h"
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
YablHash* SYMBOL_TABLE ;


void pIndent(){
    for(int i=1; i<=indent; i++){
        printf("|   ");
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

void symbolTableAddKeywords(){
    prettyPrint("stdlip keywords: -----------------\n");
    symbolTablePush("input", createData(bt_text));


    Data* rtnType = createData(bt_NULL);
    Data* args = createData(bt_text);
    rtnType->list = args;
    symbolTablePush("print", rtnType);
    symbolTablePush("quit", createData(bt_NULL));



    //Reserved input
    Data* indputList = createData(bt_text);
    symbolTablePush("input", indputList);

    //Reserved endgame
    Data* endgameList = createData(bt_text);
    symbolTablePush("endgame", endgameList);

    //Reserved board
    

    //Reserved tile
    Data* tileList = createData(bt_list);
    tileList->list = createData(bt_text);
    symbolTablePush("tile", tileList);

    Data* boardList = createData(bt_list);
    boardList->list = createData(bt_list);
    Data* tileType = createData(bt_custom);
    tileType->value = "tile";
    ((Data*)boardList->list)->list = tileType; //<---
    symbolTablePush("board", boardList );//<--- list list tile
    
    //Reserved player
    Data* playerList = createData(bt_list);
    playerList->list = createData(bt_text);
    symbolTablePush("player", playerList);

    //Reserved curretplayer
    symbolTablePush("currentPlayer", createData(bt_text));




    prettyPrint("------------------------------------\n");
}

Data* prototypeArgs(Args* self)
{
    //get args
    Data* temp = malloc(sizeof(Data));
    Data* temp2; // = createData(bt_unset);
    Data* rval = temp;

    FOREACH(Initialization*, self, 
        Data* value = visitType(foreach_value->type);
        temp2 = tcCopy(value);
        free(value);
        temp->list = temp2;
        temp = temp2;
    )
    rval = tcCopy(rval->list);

    return rval;
}

void symbolTablePrototypes(Repeatable* self){ //put prototypes in symbolTable
    
    prettyPrint("Prototypes: -------------------\n");
    FOREACH(Repeatable*, self, 
        if(foreach_value->nonterminal == funcs){
            FOREACH(Func*, foreach_value,
                if(foreach_value->nonterminal==func){
                    char* key = visitId(foreach_value->name)->value;
                    Data* type = visitType(foreach_value->returntype);
                    //Data* args = visitArgs(foreach_value->args); //saves next arg in Data->list
                    Data* args = prototypeArgs(foreach_value->args);
                   

                    type->list = args;
                    if(symbolTableGetLocal(key) == NULL)
                        symbolTablePush(key, type); //might need to push args + return type
                    else{
                        createError(ECnameSpaceClash);
                    }
                }
            )
        }
    )
    prettyPrint("--------------------------------\n");
}

//Visit function
int visit(Repeatable* self){ //Start <----
    if(self == NULL){

        printf("No AST\n");
        return 1;
    }
    //innit outersymbolTable
    SYMBOL_TABLE = malloc(sizeof(YablHash));
    *SYMBOL_TABLE = yablHashCreate(HASHLISTLENGTH, &stringHash);
    symbolTableAddKeywords();
    symbolTablePrototypes(self);
    // if(PPRINTFLAG == 1)
    // {
    //     prettyPrint("start");
    // }
    // indent++;
    // visitPreamble(self->preamble);
    visitRepeatable(self);
    // indent--;
    printf("Error Count: %d\n", TYPE_CHECKER_ERROR_COUNT);
    return TYPE_CHECKER_ERROR_COUNT;
}

Data* visitStart(Repeatable* self){
    if(PPRINTFLAG == 1)
    {
        prettyPrint("start");
    }
    indent++;

    FOREACH(Repeatable*, self, 
		Data* value = visitRepeatable(foreach_value);
		// if(value->errorCode != ECnoError) return value;
	)
    //visitRepeatable(self->children);
    indent--;
    //printf("Error Count: %d\n", TYPE_CHECKER_ERROR_COUNT);
    return self;
}

//--------------------------------------

//Mangler
Data* visitPreamble(Preambles* self){
    if(PPRINTFLAG == 1)
    {
        prettyPrint("Preamble");
    }
    indent++;
    switch(self->nonterminal)
    {
        case preambleTile:
            visitPreambleTile(self);
            break;
        case preambleBoard:
            visitPreambleBoard(self);
            break;
        case preamblePlayers:
            visitPreamblePlayer(self);
            break;
        default:
            return createError(ECoutOfRange);

    }
    indent--;
    
    return tcAccept();
}

Data* visitRepeatable(Repeatable* self){
    // if(PPRINTFLAG == 1)
    // {
    //     prettyPrint("Repeatable");
    // }
    // indent++;
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
    case start:
        rtn = visitStart(self);
        break;
    default:
        rtn = createError(ECoutOfRange);
        break;
    }
    // indent--;
	return rtn;
}

Data* visitExprs(Exprs* self){
    
    if(PPRINTFLAG == 1)
    {
        prettyPrint("Exprs");
    }
    if(self == NULL){
        return tcAccept();
    }
    indent++;
    Data* temp = malloc(sizeof(Data));
    Data* temp2; // = createData(bt_unset);
    Data* rval = temp;
	FOREACH(Expr*, self, 
		Data* value = visitExpr(foreach_value);
		if(value->errorCode != ECnoError) return value;
        temp2 = tcCopy(value);
        free(value);
        temp->list = temp2;
        temp = temp2;
		// return tcExpr(foreach_value, value);
	)
    rval = tcCopy(rval->list);
    indent--;
    return rval; 
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
    return tcAccept(); 
}

Data* visitScope(Scope* self, Data* returnType){
    if(PPRINTFLAG == 1)
    {
        prettyPrint("Scope");
    }
    indent++;
    
	// YablList l = self->children;
	
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
		
	)
    
    indent--;
    return tcAccept(); 
}

Data* visitArgs(Args* self){
    if(PPRINTFLAG == 1)
    {
        prettyPrint("Args");
    }
    indent++;

    Data* temp = malloc(sizeof(Data));
    Data* temp2; // = createData(bt_unset);
    Data* rval = temp;

    FOREACH(Initialization*, self, 
		Data* value = visitInitialization(foreach_value);
        temp2 = tcCopy(value);
        free(value);
        temp->list = temp2;
        temp = temp2;
	)
    rval = tcCopy(rval->list);
    // free(temp);
    //visitExprs(self);
    indent--;
    return rval; 
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
	)
    indent--;
    return tcAccept(); 
}

Data* visitListConstant(ListConstant* self){
    if(PPRINTFLAG == 1)
    {
        prettyPrint("List");
    }
    indent++;
    Data* type = createData(bt_unset);
    // visitExprs(self);
    FOREACH(Expr*, self->exprs, 
		Data* value = visitExpr(foreach_value);
        if(value->errorCode != ECnoError) return createError(value->errorCode);
		if(value->type == type->type){
        }
        else if(type->type == bt_unset)
        {
            type->type = value->type;
        }
	)

    indent--;
    return createList(type);
}

//Mangler
Data* visitPreambles(Preambles* self){
    if(PPRINTFLAG == 1)
    {
        prettyPrint("Preambles");
    }
    indent++;
    FOREACH(Repeatable*, self, 
        visitPreamble(foreach_value);
    )
    indent--;
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
    if(self == NULL){
        indent--;
        return NULL;
    }
    switch (self->exprType)
    {
    case et_constant:
        child = tcAccept();       
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
    case et_list:
        child = visitListConstant((ListConstant*)self->child);
        break;
    case et_typecast:
        child = visitTypeCast(self->child);
        break;
    default:
        return createError(ECoutOfRange);
        break;
    }
    rval = tcExpr(self, child);
    self->extension = tcCopy(rval);
    free(child);
    indent--;
    return rval;
}

Data* visitTypeCast(TypeCast* self){
    if(PPRINTFLAG == 1)
    {
        prettyPrint("TypeCast");
    }
    indent++;
    Data* expr = visitExpr(self->cast);
    Data* type = visitType(self->type);

    Data* rval = tcTypeCast(self, expr, type);
    indent--;
    free(expr);
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
     case returnstmt: 
        rtn = visitReturnStmt((ReturnStmt*)self);
        break;
	case breakstmt:
		rtn = tcAccept();
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
    createSymbolTable();
    indent++;
    Data* rval;
    switch (self->nonterminal){
		case func:;
            Data* id = visitId(self->name);
			Data* args = visitArgs(self->args);
			Data* returnType = visitType(self->returntype);
			Data* scope = visitScope(self->scope, returnType); //check returnstmt against returntype
			
			rval = tcFunc(self, args, returnType, scope, id);
			free(args);
			//free(returnType);
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
    deleteSymbolTable();
    return rval;
}

//--------------------------------------

Data* visitIdMutation(IdMutation* self){
    if(self == NULL){ 
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
    if(self->child!= NULL){
        switch (*(IdMutations*)(self->child))
        {
        case im_none:
            break;
        case im_dot:
            child = visitIdMutationDot(self->child, id->value); // <--
            break;
        case im_call:
            child = visitIdMutationCall(self->child);
            break;
        case im_index:
            char temp[40] = "index";
            strcat(temp, id->value);
            symbolTablePush(temp, symbolTableGet(id->value)); //save local instance 
            child = visitIdMutationIndex(self->child, temp);
            break;
        default:
            createError(ECoutOfRange);
            break;

        }
    }
    else{
        child = tcAccept();
    }
    
    rval = tcIdMutation(self, child, id);
    if(PPRINTFLAG == 1)
    {
        free(createData(rval->type));
    }
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
    Data* id = visitIdMutation(self->variable);
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
    createSymbolTable();
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

    deleteSymbolTable();
    indent--;
    return rval;
}


Data* visitReturnStmt(ReturnStmt* self){
    if(PPRINTFLAG == 1)
    {
    prettyPrint("returnstmt");
    }
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
    

    rval = tcInitialization(self,id, type, val);
    free(type);
    free(val);
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
    Data* type = NULL;
    if(self != NULL){
        type = visitTypeValue(self->typeval);
    }

    rval = tcType(self, type); //returns type
    // free(type);

    indent--;
    return rval;
}

//--------------------------------------

Data*  visitIdMutationDot(IdMutationDot* self, Id id){
    if(PPRINTFLAG == 1)
    {
        prettyPrint("IdMutationDot");
    }
    indent++;
    Data* rval;
    Data* type;
    if(strstr(id, "index") != NULL){
        type = symbolTableGet(id);
        id += 5;
    }
    else{
        type = symbolTableGet(id);
    }
    
    if(type->type == bt_custom){
        char customType[30];
        strcpy(customType,type->value);
        id = strcat(customType, ".");
    }
    else{
        id = strcat(id, ".");
    }
    self->child->name = strcat(id, self->child->name);


    //Data* name = visitId(NULL);
    Data* child = visitIdMutation(self->child);

    rval = tcIdMutationDot(self, child);
    //free(name);
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
 
    Data* child = visitIdMutationChild(self->child, NULL); //suptype
    Data*  args = visitExprs(self->args);

    rval = tcIdMutationCall(self, child, args);
    free(child);
    free(args);
    
    indent--;
    return rval;
}   

Data* visitIdMutationIndex(IdMutationIndex* self, Id id){
    if(PPRINTFLAG == 1)
    {
        prettyPrint("IdMutationIndex");
    }
    indent++;
    Data* rval;

    Data* type = symbolTableGet(id);
    Data* typeCopy = tcCopy(type);

    symbolTablePush(id, type->list);

    Data* expr = visitExpr(self->index);
    Data* child = visitIdMutationChild(self->child, id); // suptype

    rval = tcIdMutationIndex(self, expr, child, typeCopy);
    free(typeCopy);
    free(expr);
    free(child);

    indent--;
    return rval;
}

Data* visitIdMutationChild(IdMutations* self, Id id){
    if(self == NULL)    return tcAccept();
    Data* rval;
    switch (*self) {

    
    case im_dot:
        rval = visitIdMutationDot((IdMutationDot*)self, id);
        break;
    case im_call:
        rval = visitIdMutationCall((IdMutationCall*)self);
        break;
    case im_index:
        rval = visitIdMutationIndex((IdMutationIndex*)self, id);
        break;
    case im_none:
    case im_value:
        rval = tcAccept();
        break;
    default:
        rval = createError(ECoutOfRange);
    }

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

Data* visitTypeValue(TypeValue* self){
    if(PPRINTFLAG == 1)
    {
        prettyPrint("TypeValue");
    }
    indent++;
    Data* rval;

    switch (self->type) {

    case bt_NULL:
    case bt_number:
    case bt_text:
    case bt_logic:  
    case bt_unset:
        rval = createData(self->type);
        break;
    case bt_list:
        rval = createList(visitType(self->list));
        break;
    default:
        createError(ECoutOfRange);
    }


    indent--;
    return rval;
}

//--------------------------------------

//mangler
Data* visitId(Id self){ //<--- check imod reserved
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
Data* visitTypeDCL(Type* self){ //<-does nothing
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

Data* visitPreambleBoard(PreambleBoard* self){
     if(PPRINTFLAG == 1)
    {
        prettyPrint("Board");
    }
    indent++;
    if(self->width >= 0 && self->height >= 0){
        // symbolTablePush("Board", self);
    }
    else {
        prettyPrint("Board dimensions must be over 0");
        return createError(ECargumentExeption);
    }

    indent--;
    return tcAccept();
}

Data* visitPreambleTileItem(Type* self){//<--- not used
    return tcAccept();
}

Data* visitPreambleTile(PreambleTile* self){
     if(PPRINTFLAG == 1)
    {
        prettyPrint("Tile");
    }
    indent++;
    // char str[30];
    // strcat(str, "tile.");
    // strcat(str, ((Initialization*)self->children->item)->variable);
    // free(((Initialization*)self->children->item)->variable);
    // ((Initialization*)self->children->item)->variable = str;
    FOREACH(Initialization*, self,
        
        char* str = malloc(sizeof(char[30]));
        strcat(str, "tile.");
        strcat(str, foreach_value->variable);
        free(foreach_value->variable);
        foreach_value->variable = str;
        visitInitialization(foreach_value);
    )
    indent--;
    return tcAccept();
}

Data* visitPreamblePlayer(PreamblePlayers* self){
    //list of string with playernames.
     if(PPRINTFLAG == 1)
    {
        prettyPrint("Players");
    }
    indent++;
    // symbolTablePush("Players", self);
    FOREACH(Id, self,
        visitId(foreach_value);
    );
    indent--;
    return tcAccept();
}


