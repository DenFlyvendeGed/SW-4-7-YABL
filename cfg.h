#ifndef YABL_CFG_H
#define YABL_CFG_H

#include "./data-structures/list.h"

char* copystringalloc(char*);

typedef enum {exprs, scope, expr, stmts, stmt, funcs, func, args, arg, event, ifstmt, repeatstmt, idMutation, variable, preamblePlayers, assign, initialization, unaryOperator, binaryOperator, listConstants, listConstant, preambles, type, constant} Nonterminals;


typedef enum {bt_number, bt_text, bt_logic, bt_list} BasicTypes;
struct Typedcl;
typedef union {
	struct Typedcl* list;
	BasicTypes type;
} TypeValue;

typedef struct Typedcl{
	Nonterminals nonterminal;	
	TypeValue* typeval;	
} Type;

// Constants
typedef int   Number;
typedef int   Logic;
typedef char* Text;

typedef enum {
	td_number,
	td_logic,
	td_text,
} TypeDcls;


typedef struct{
	Nonterminals nonterminal;
	TypeDcls typeDcl;
	char* value;
} Constant;



// Repeatables 
typedef struct {
	Nonterminals nonterminal;
	YablList children;
} Repeatable;

typedef Repeatable Exprs;

typedef Repeatable Stmts;

typedef Repeatable Scope;

typedef Repeatable Args;

typedef Repeatable Funcs;

typedef Repeatable Preambles;

typedef char* Id;


typedef struct {
	Nonterminals nonterminal;
	Exprs* exprs;
} ListConstant;

// Func
typedef struct {
	Nonterminals nonterminal;
	Id name;
	Args* args;
	Type* returntype;
	Scope* scope;
} Func;

// Expr

typedef enum {et_constant, et_id_mutation, et_unary_operator, et_binary_operator, et_expression} ExprType;
typedef struct Expr{
	Nonterminals nonterminal;
	ExprType exprType;
	void* child;
} Expr;

typedef enum {
	bo_plus, bo_minus, bo_division, bo_mul, bo_modulus, 
	bo_not, bo_eq, bo_neq, bo_gt, bo_gteq, bo_lt, bo_lteq,
	bo_and, bo_or 
} BinaryOperators;

typedef struct {
	Nonterminals nonterminal;
	BinaryOperators bo;
	Expr* childExpr1;
	Expr* childExpr2;
} BinaryOperator;

typedef enum {
	uo_negate, uo_not
} UnaryOperators;

typedef struct {
	Nonterminals nonterminal;
	UnaryOperators uo;
	Expr* childExpr;
} UnaryOperator;

// Id Mutation
typedef enum {im_none, im_value, im_dot, im_call, im_index} IdMutations;

typedef struct {
	Nonterminals nonterminal;
	Id name;
	void* child;
} IdMutation;

typedef struct{
	IdMutations type;
	IdMutation* child;
} IdMutationDot;

typedef struct{
	IdMutations type;
	Exprs* args;
	void* child;
} IdMutationCall;

typedef struct{
	IdMutations type;
	Expr* index;
	void* child;
} IdMutationIndex;

/// if
typedef struct{} Stmt;

typedef struct {
	Nonterminals nonterminal;
	Expr* condition;
	Scope* then;
	Scope* elsestmt;
} IfStmt;

/// repeat
typedef enum {lt_timesloop, lt_forloop, lt_whileloop, lt_repeatloop} LoopType;
typedef struct {
	Nonterminals nonterminal;
	void * loopType; //change name??
	Scope* scope;
} Repeat;

typedef struct {
	LoopType loopType;	
	Expr* goal;	
} TimesLoop;

typedef struct {
	LoopType loopType;	
	// Might need some list constant
	Id variable_name;
	IdMutation* input_name;
} ForLoop;

typedef struct {
	LoopType loopType;	
	Expr* condition;
} WhileLoop;

typedef struct {
	LoopType loopType;	
} RepeatLoop;


/// assing
typedef struct {
	Nonterminals nonterminal;
	IdMutation* variable;
	Expr* expression;
} Assign;

/// Initialization
typedef struct {
	Nonterminals nonterminal;
	Type* type;
	Expr* initialValue;
	Id variable;
} Initialization;

// Events
typedef enum {event_setup, event_turn, event_close} Events;
typedef struct {
	Nonterminals nonterminal;
	Events eventType;
	Scope* scope;
} Event;

// Variable
typedef struct {
	Nonterminals nonterminal;
	Id name;
	Type* type;
} Variable;


// Return
typedef struct {
	Nonterminals nonterminal;
	Expr* expr;
} ReturnStmt;

// Preamble
typedef struct {
	Nonterminals nonterminal;
	Id name;
	YablList* ids;
} Preamble;

typedef struct {
	Nonterminals nonterminal;
	struct {int x, y;} dimentions;
} PreambelBoard;

typedef struct {
	Id name;
	int is_list;
	int is_array;
} PreambleTileItem;

typedef struct {
	Nonterminals nonterminal;
	YablList* tile_items;
} PreambelTile;

typedef struct {
	Nonterminals nonterminal;
	YablList* ids;
} PreamblePlayers;

Exprs* createExprs();
void destroyExprs(Exprs* exprs);

Constant* createConstant(TypeDcls type, char* value);
void destroyConstant(Constant* p);

Func* createFunc(char* name, Args* funcArgs, Type* returnType, Scope* funcScope);
void destroyFunc(Func* p);

TypeValue* createTypeValue(BasicTypes varType, Type* listChild);
void destroyTypeValue(TypeValue* p);

Type* createType(TypeValue* value);
void destroyType(Type* p);

Repeatable* creatRepeatable(Nonterminals nonterminal, YablList* list);
void destroyRepeatable(Repeatable* p);

Expr* createExpr(ExprType ExprType, void* child);
Exprs* exprsAddExpr(Exprs* self, Expr* expr);
void destroyExpr(Expr* p);

BinaryOperator* createBinaryOperator(BinaryOperators bOp, Expr* childExpr1, Expr* childExpr2);
void destroyBinaryOperator(BinaryOperator* p);

UnaryOperator* createUnaryOperator(UnaryOperators uOp, Expr* childExpr);
void destroyUnaryOperator(UnaryOperator* p);

IdMutationDot* createIdMutationDot(IdMutation* child);
void destroyIdMutationDot(IdMutationDot* p);

IdMutationCall* createIdMutationCall(void* child, Exprs* mutationArgs);
void destroyIdMutationCall(IdMutationCall* p);

IdMutationIndex* createIdMutationIndex(void* child, Expr* index);
void destroyIdMutationIndex(IdMutationIndex* p);

IdMutation* createIdMutation(Id name, void* child);
void destroyIdMutation(IdMutation* p);

IfStmt* createIfStmt(Expr* cond, Scope* condTrue, Scope* condFalse);
void destroyIfStmt(IfStmt* p);

Repeat* createRepeat(void* loop, Scope* repeatScope);
void destroyRepeat(Repeat* self);

TimesLoop* createTimesLoop(Expr* goal);
void destroyTimesLoop(TimesLoop* p);

ForLoop* createForLoop(Id varName, IdMutation* inputName);
void destroyForLoop(ForLoop *p);

WhileLoop* createWhileLoop(Expr* cond);
void destroyWhileloop(WhileLoop* p);

RepeatLoop* createRepeatLoop();
void destroyRepeatLoop(RepeatLoop* p);

Initialization* createInitialization(Id var, Type* varType, Expr* initialValue);
void destroyInitialization(Initialization* p);

Assign* createAssign(IdMutation* var, Expr* exp);
void destroyAssign(Assign* p);

Event* createEvent(Scope* eventScope, Events eventType);
void destroyEvent(Event* p);

Variable createVariable(Id name, Type* type);
void destroyVariable(Variable* p);

Stmts* createStmts();
void destroyStmts(Stmts* p);

Scope* createScope(Stmts* stmts);
Scope* scopeAddStmt(Scope* self, Stmt* stmt);
void destroyScope(Scope* p);

Args* createArgs();
void destroyArgs(Args* p);

Funcs* createFuncs();
Funcs* funcsAddFunc(Funcs* self, Func* func);
void  destroyFuncs(Funcs* p);

ListConstant* createListConstant(Exprs* exprs);
void destroyListConstant(ListConstant* p);

ReturnStmt* createReturnStmt(Expr* rtn);
void destryReturnStmt(ReturnStmt* self);

Funcs createPreambles();
void  destroyPreambles(Funcs* p);

#endif // !YABL_CFG_H
