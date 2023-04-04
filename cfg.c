#include "cfg.h"
#include "data-structures/list.h"
#include <stdlib.h>
#include <string.h>

void destroyList();

char* copystringalloc(char* src){
	char* cpy = malloc(strlen(src) + 1)	;
	if( cpy == NULL ) return NULL;
	strcpy(src, cpy);
	return cpy;
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

		case preamblePlayers: break;
		case preambles: break;
		default:;
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
	destroyTypeVal((p->typeval));
	free(p);
}



// struct Expr *Expr;
Expr* createExpr(ExprType exprType, void* child)
{
	Expr* p = malloc(sizeof(Expr));
	p->nonterminal = expr;
	p->child = child;
	p->exprType = exprType;


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

void destroyIdMutatuinIndex(IdMutationIndex* p)
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
	p->variable_name = varName;
	p->input_name = inputName;

	return p;
}

void destroyForLoop(ForLoop *p)
{
	destroyIdMutation(p->input_name);
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

void  destroyExprs(Exprs* p)
{
	destroyList(p->children);
}

// typedef Repeatable Stmts;
Stmts* createStmts()
{
	Stmts* p = createRepeatable(stmts);
	return p;
}
void destroyStmts(Stmts* p)
{
	destroyRepeatable(p);
}

// typedef Repeatable Scope;
Scope* createScope(Stmts* p)
{
	return (Scope*)p;
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

// typedef Repeatable Preambles;
Funcs createPreambles()
{
	Funcs *p = malloc(sizeof(Funcs));
	p->nonterminal = preambles;

	return *p;
}

void  destroyPreambles(Funcs* p)
{
}

