#include "cfg.h"
#include "../data-structures/list.h"
#include <stdlib.h>
#include <string.h>

char* copystringalloc(char* src){
	char* cpy = malloc(strlen(src) + 1)	;
	if( cpy == NULL ) return NULL;
	strcpy(cpy, src);
	return cpy;
}

Repeatable* repeatablePushChild(Repeatable* self, void* child){
	yablListInsert(&self->children, 0, child);
	return self;
}

Repeatable* createRepeatable(Nonterminals nonterminal){
	Repeatable* self = malloc(sizeof(Repeatable));
	self->nonterminal = nonterminal;
	self->children = yablListCreate();
	return self;
}


void universalDestroyFunction(void* nonterminal) {
	switch (*(Nonterminals*)nonterminal) {
		case type    : destroyType(nonterminal)    ; break;
		case constant: destroyConstant(nonterminal); break;

		case expr : destroyExpr(nonterminal) ; break;
		case exprs: destroyExprs(nonterminal); break;
		case stmts: destroyStmts(nonterminal); break;
		case scope: destroyScope(nonterminal); break;
		case stmt : break;
		case funcs: destroyFuncs(nonterminal); break;
		case func : destroyFunc(nonterminal) ; break;
		case args : destroyArgs(nonterminal) ; break;
		case arg  : break;

		case event : destroyEvent(nonterminal) ; break;
		case ifstmt: destroyIfStmt(nonterminal); break;
		case assign: destroyAssign(nonterminal); break;
					 
		case repeatstmt: destroyRepeat(nonterminal)    ; break;
		case idMutation: destroyIdMutation(nonterminal); break;
		case variable  : destroyVariable(nonterminal)  ; break;

		case initialization: destroyInitialization(nonterminal); break;
		case unaryOperator : destroyUnaryOperator(nonterminal) ; break;
		case binaryOperator: destroyBinaryOperator(nonterminal); break;
		case listConstant  : destroyListConstant(nonterminal)  ; break;
		case listConstants : break;

		case preamblePlayers: destroyPreamblePlayers(nonterminal); break;
		case preambles		: destroyPreambles(nonterminal)      ; break;
		case preambleBoard  : destroyPreambleBoard(nonterminal)  ; break;
		case preambleTile   : destroyPreambleTile(nonterminal)   ; break;
		case returnstmt: destroyReturnStmt(nonterminal); break;
		default: ;
	}
}

void destroyRepeatable(Repeatable* self){
	yablListDelete(self->children, &universalDestroyFunction);
	free(self);
}

// int int;
// char *string;

//constant
Constant* createConstant(TypeDcls type, char* value) //<------
{
	Constant* self = malloc(sizeof(Constant));
	self->nonterminal = constant;
	self->value = value;
	self->typeDcl = type;
	return self;
}

void destroyConstant(Constant* p)
{
	free(p->value);
	free(p);
}


// struct Func *Function;
Func* createFunc(Id name, Args* funcArgs, Type* returnType, Scope* funcScope)
{
	Func* p = malloc(sizeof(Func));
	p->nonterminal = func;
	p->name = name;
	p->args = funcArgs;
	p->returntype = returnType;
	p->scope = funcScope;

	return p;
}

void destroyFunc(Func* p)
{
	destroyArgs((p->args));
	destroyType((p->returntype));
	destroyScope((p->scope));
	free(p);
}

// struct TypeValue *TypeValue;
TypeValue* createTypeValue(BasicTypes varType, Type* listChild) //<--------
{
	TypeValue* p = malloc(sizeof(TypeValue));
	p->type = varType;
	p->list = listChild;
	return p;
}

void destroyTypeValue(TypeValue* p)
{
	destroyType(p->list);
	free(p);
}

// struct Type *Type;
Type* createType(TypeValue* value)
{
	Type* p = malloc(sizeof(Type));
	p->nonterminal = type;
	p->typeval = value;
	return p;
}

void destroyType(Type* p)
{
	destroyTypeValue((p->typeval));
	free(p);
}



// struct Expr *Expr;
Expr* createExpr(ExprType exprType, void* child)
{
	Expr* p = malloc(sizeof(Expr));
	p->nonterminal = expr;
	p->child = child;
	p->exprType = exprType;
	p->extension = NULL;


	return p;
}

void destroyExpr(Expr* p)
{
	switch(p->exprType)
	{
		case et_constant:
			destroyConstant((p->child));
			break;
		case et_id_mutation:
			destroyIdMutation((p->child));
			break;
		case et_unary_operator:
			destroyUnaryOperator((p->child));
			break;
		case et_binary_operator:
			destroyBinaryOperator((p->child));
			break;
		case et_expression:
			destroyExpr((p->child));
			break;
	}
	free(p);
}

// struct BinaryOperator *BinaryOp;
BinaryOperator* createBinaryOperator(BinaryOperators bOp, Expr* childExpr1, Expr* childExpr2)
{
	BinaryOperator* p = malloc(sizeof(BinaryOperator));
	p->nonterminal = binaryOperator;
	p->bo = bOp;
	p->childExpr1 = childExpr1;
	p->childExpr2 = childExpr2;

	return p;
}

void destroyBinaryOperator(BinaryOperator* p)
{
	destroyExpr((p->childExpr1));
	destroyExpr((p->childExpr2));
	free(p);
}


// struct UnaryOperator *UnaryOp;
UnaryOperator* createUnaryOperator(UnaryOperators uOp, Expr* childExpr)
{
	UnaryOperator *p = malloc(sizeof(UnaryOperator));
	p->nonterminal = unaryOperator;
	p->childExpr = childExpr;
	p->uo = uOp;

	return p;
}

void destroyUnaryOperator(UnaryOperator* p)
{
	destroyExpr((p->childExpr));
	free(p);
}

TypeCast* createTypeCast(Type* type, Expr* cast){
	TypeCast* self = malloc(sizeof(TypeCast));
	self->type = type;
	self->cast = cast;
	return self;
}

void destroyTypeCast(TypeCast* self){
	destroyType(self->type);
	destroyExpr(self->cast);
	free(self);
}

// struct IdMutationDot *IdMutationDot;
IdMutationDot* createIdMutationDot(IdMutation* child)
{
	IdMutationDot *p = malloc(sizeof(IdMutationDot));
	p->type = im_dot;
	p->child = child;
	return p;
}

void destroyIdMutationDot(IdMutationDot* p)
{
	destroyIdMutation(p->child);
	free(p);
}


// struct IdMutationCall *IdMutationCall;
IdMutationCall* createIdMutationCall(void* child, Exprs* mutationArgs)
{
	IdMutationCall *p = malloc(sizeof(IdMutationCall));
	p->type = im_call;
	p->child = child;
	p->args = mutationArgs;
	return p;
}

void destroyIdMutationCall(IdMutationCall* p)
{
	destroyExprs((p->args));
	free(p);
}


// struct IdMutationIndex *IdMutationIndex;
IdMutationIndex* createIdMutationIndex(void* child, Expr* index)
{
	IdMutationIndex *p = malloc(sizeof(IdMutationIndex));
	p->type = im_index;
	p->child = child;
	p->index = index;
	return p;
}

void destroyIdMutationIndex(IdMutationIndex* p)
{
	destroyExpr((p->index));
	free(p);
}



// struct IdMutation *IdMutation;
IdMutation* createIdMutation(Id name, void* child)
{
	IdMutation *p = malloc(sizeof(IdMutation));
	p->nonterminal = idMutation;
	p->name = name;
	p->child = child;
	return p;
}

void destroyIdMutation(IdMutation* p)
{
	switch (*(IdMutations*)p->child) {
		case im_index: destroyIdMutationIndex(p->child); break;
		case im_dot: destroyIdMutationDot(p->child); break;
		case im_call: destroyIdMutationCall(p->child); break;
		case im_value: ;
		case im_none: ;
	}

	free(p);
}


// struct IfStmt *IfStmt;
IfStmt* createIfStmt(Expr* cond, Scope* condTrue, Scope* condFalse)
{
	IfStmt *p = malloc(sizeof(IfStmt));
	p->nonterminal = ifstmt;
	p->condition = cond;
	p->then = condTrue;
	p->elsestmt = condFalse;

	return p;
}

void destroyIfStmt(IfStmt* p)
{
	destroyExpr((p->condition));
	destroyScope((p->then));
	destroyScope((p->elsestmt));
	free(p);
}


// struct Repeat *Repeat;
Repeat* createRepeat(void* loop, Scope* repeatScope)
{
	Repeat *p = malloc(sizeof(Repeat));
	p->nonterminal = repeatstmt;
	p->loopType = loop;
	p->scope = repeatScope;
	return p;
}

void destroyRepeat(Repeat* self){
	switch (*(LoopType*)self->loopType) {
		case lt_timesloop:  destroyTimesLoop(self->loopType); break;
		case lt_forloop:    destroyForLoop(self->loopType); break;
		case lt_whileloop:  destroyWhileloop(self->loopType); break;
		case lt_repeatloop: destroyRepeatLoop(self->loopType); break;
	}

	destroyScope(self->scope);
	free(self);
}

// struct TimesLoop *TimesLoop;
TimesLoop* createTimesLoop(Expr* goal)
{
	TimesLoop *p = malloc(sizeof(TimesLoop));
	p->loopType = lt_timesloop;
	p->goal = goal;
	return p;
}

void destroyTimesLoop(TimesLoop* p)
{
	destroyExpr(p->goal);
	free(p);
}

// struct ForLoop *ForLoop;
ForLoop* createForLoop(Id varName, IdMutation* inputName)
{
	ForLoop *p = malloc(sizeof(ForLoop));
	p->loopType = lt_forloop;
	p->variableName = varName;
	p->inputName = inputName;

	return p;
}

void destroyForLoop(ForLoop *p)
{
	destroyIdMutation(p->inputName);
	free(p);
}

// struct whileLoop *whileLoop;
WhileLoop* createWhileLoop(Expr* cond)
{
	WhileLoop *p = malloc(sizeof(WhileLoop));
	p->loopType = lt_whileloop;
	p->condition = cond;
	return p;
}

void destroyWhileloop(WhileLoop* p)
{
	destroyExpr(p->condition);
	free(p);
}

// struct RepeatLoop *RepeatLoop;
RepeatLoop* createRepeatLoop()
{
	RepeatLoop *p = malloc(sizeof(RepeatLoop));
	p->loopType = lt_repeatloop;

	return p;
}

void destroyRepeatLoop(RepeatLoop* p)
{
	free(p);
}

Break* createBreak(){
	Break* self = malloc(sizeof(Break));
	self->nonterminal = breakstmt;
	return self;
}

void destroyBreak(Break* self){
	free(self);
}

// struct Initialization *Initialization;
Initialization* createInitialization(Id var, Type* varType, Expr* initialValue)
{
	Initialization *p = malloc(sizeof(Initialization));
	p->nonterminal = initialization;
	p->type = varType;
	p->variable = var;
	p->initialValue = initialValue;

	return p;
}

void destroyInitialization(Initialization* p)
{
	destroyType((p->type));
	free(p);
}

// struct Assign *Assign;
Assign* createAssign(IdMutation* var, Expr* exp)
{
	Assign *p = malloc(sizeof(Assign));
	p->nonterminal = assign;
	p->variable = var;
	p->expression = exp;
	return p;
}
void destroyAssign(Assign* p)
{
	destroyExpr((p->expression));
	destroyIdMutation(p->variable);
	free(p);
}

// struct Event *Event;
Event* createEvent(Scope* eventScope, Events eventType)
{
	Event *p = malloc(sizeof(Event));
	p->nonterminal = event;
	p->scope = eventScope;
	p->eventType = eventType;
	return p;
}
void destroyEvent(Event* p)
{
	destroyScope((p->scope));
	free(p);
}

// struct Variable *Variable;
Variable createVariable(Id name, Type* type)
{
	Variable *p = malloc(sizeof(Variable));
	p->nonterminal = variable;
	p->name = name;
	p->type = type;
	return *p;
}

void destroyVariable(Variable* p)
{
	destroyType((p->type));
	free(p);
}


// typedef Repeatable Exprs;
Exprs* createExprs() 
{
	Exprs* p = createRepeatable(exprs);
	return p;
}

Exprs* exprsAddExpr(Exprs* self, Expr* expr){
	return repeatablePushChild(self, expr);
}

void  destroyExprs(Exprs* p)
{
	destroyRepeatable(p);
}

// typedef Repeatable Stmts;
Stmts* createStmts()
{
	Stmts* p = createRepeatable(stmts);
	return p;
}

Stmts* stmtsAddStmt(Stmts* self, Stmt* stmt){
	return repeatablePushChild(self, stmt);
}

void destroyStmts(Stmts* p)
{
	destroyRepeatable(p);
}

// typedef Repeatable Scope;
Scope* createScope(Stmts* p)
{
	p->nonterminal = scope;
	return (Scope*)p;
}

Scope* scopeAddStmt(Scope* self, Stmt* stmt){
	return repeatablePushChild(self, stmt);
}


void destroyScope(Scope* p)
{
	destroyRepeatable(p);
}

// typedef Repeatable Args;
Args* createArgs()
{
	Args* p = createRepeatable(args);
	return p;
}

Args* argsAddInitialization(Args* self, Initialization* initialization) {
	return repeatablePushChild(self, initialization);
}

void destroyArgs(Args* p)
{
	destroyRepeatable(p);
}

// typedef Repeatable Funcs;
Funcs* createFuncs()
{
	Funcs* p = createRepeatable(funcs);
	return p;
}

Funcs* funcsAddFunc(Funcs* self, Func* func){
	return repeatablePushChild(self, func);
}

void  destroyFuncs(Funcs* p)
{
	destroyRepeatable(p);
}

// typedef Repeatable ListConstant;
ListConstant* createListConstant(Exprs* exprs)
{
	ListConstant* self = malloc(sizeof(ListConstant));
	self->nonterminal = listConstant;
	self->exprs = exprs;
	return self;
}

void destroyListConstant(ListConstant* p)
{
	destroyExprs(p->exprs);
	free(p);
}

ReturnStmt* createReturnStmt(Expr* expr){
	ReturnStmt* self = malloc(sizeof(ReturnStmt));
	self->nonterminal = returnstmt;
	self->expr = expr;
	return self;
}

void destroyReturnStmt(ReturnStmt* self){
	destroyExpr(self->expr);
	free(self);
}


// typedef Repeatable Preambles;
Preambles* createPreambles(){
	return createRepeatable(preambles);
}

Preambles* preamblesPushPreamble(Preambles* self, void* child){
	return repeatablePushChild(self, child);
}

void  destroyPreambles(Preambles* self){
	destroyRepeatable(self);
}

PreambleTile* createPreambleTile(){
	return createRepeatable(preambleTile);
}

PreambleTile* preambleTileAddInitialiation(PreambleTile* self, Initialization* variable){
	return repeatablePushChild(self, variable);
}

void destroyPreambleTile(PreambleTile* self){
	destroyRepeatable(self);
}

PreamblePlayers* createPreamblePlayers(){
	return createRepeatable(preamblePlayers);
}

PreamblePlayers* preamblePlayersAddPlayer(PreamblePlayers* self, char* player){
	return repeatablePushChild(self, player);
}

void destroyPreamblePlayers(PreamblePlayers* self){
	yablListDelete(self->children, &free);
	free(self);
}

PreambleBoard* createPreambleBoard(char* size, char* len){
	PreambleBoard* self = malloc(sizeof(PreambleBoard));
	self->nonterminal = preambleBoard;
	self->width = atoi(strtok(size, "x"));
	self->height= atoi(strtok(NULL, "\0"));
	self->writeWidth = atoi(strtok(len, "x"));
	self->writerHeight = atoi(strtok(NULL, "\0"));
	return self;
}

void destroyPreambleBoard(PreambleBoard* self){
	free(self);
}

PreambleGlobals* createPreambleGlobals(){
	return createRepeatable(preambleGlobals);
}

PreambleTile* preambleGlobalsAddInitialiation(PreambleGlobals* self, Initialization* variable){
	return repeatablePushChild(self, variable);
}

void destroyPreambleGlobals(PreambleGlobals* self){
	destroyRepeatable(self);
}

#define IS_EQ(X) (strcmp(X, name) == 0)
int isNamePredefined(char* name){
	if(
		IS_EQ("print")
	||  IS_EQ("input")
	||  IS_EQ("currentPlayer")
	||  IS_EQ("gettoken")
	||  IS_EQ("endgame")
	||  IS_EQ("board")
	||  IS_EQ("printBoard")
	)    return 1;
	else return 0;
}

