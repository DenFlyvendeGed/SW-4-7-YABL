#include "./data-structures/list.h"

typedef enum {exprs, scope, expr, stmts, stmt, funcs, func, args, arg, event, ifstmt, repeatstmt, idMutation, variable, preamblePlayers, assign, initialization, unaryOperator, binaryOperator, listConstants, listConstant, preambles, type, repeat} Nonterminals;



typedef enum {number, text, logic, list} BasicTypes;
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

typedef union {
	Number number;	
	Logic logic;
	Text text;
} Constant;



// Repeatables 
typedef struct {
	Nonterminals nonterminal;
	YablList* children;
} Repeatable;

typedef Repeatable Exprs;
Exprs createExprs();
void  destroyExprs();

typedef Repeatable Stmts;
Stmts createStmts();
void destroyStmts();

typedef Repeatable Scope;
Scope createScope();
void destroyScope();

typedef Repeatable Args;
Args createArgs();
void destroyArgs();

typedef Repeatable Funcs;
Funcs createFuncs();
void  destroyFuncs();

typedef Repeatable ListConstant;
ListConstant createFuncs();
void destroyFuncs();

typedef Repeatable Preambles;
Funcs createPreambles();
void  destroyPreambles();

typedef char* Id;


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
	bo_plus, bo_minus, bo_divsion, bo_mul, bo_modulus, 
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
	uo_negate
} UnaryOperators;

typedef struct {
	Nonterminals nonterminal;
	UnaryOperators uo;
	Expr* childExpr;
} UnaryOperator;

// Id Mutation
typedef enum {im_none, im_dot, im_call, im_index} IdMutations;

typedef struct{
	Id name;
	IdMutations childType;
	void* child;
} IdMutationDot;

typedef struct{
	Args* args;
	IdMutations childType;
	void* child;
} IdMutationCall;

typedef struct{
	Expr* index;
	IdMutations childType;
	void* child;
} IdMutationIndex;

typedef struct {
	Nonterminals nonterminal;
	Id name;
	IdMutations idMutation;	
	void* child;
} IdMutation;

/// if
typedef struct {
	Nonterminals nonterminal;
	Exprs* condition;
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
	char* variable_name;
	Id input_name;
} ForLoop;

typedef struct {
	LoopType loopType;	
	Expr* condition;
} WhileLoop;

typedef struct {
	LoopType loopType;	
} RepeatLoop;


/// Initialization
typedef struct {
	Nonterminals nonterminal;
	Type* type;
	Id variable;
} Initialization;


/// assing
typedef struct {
	Nonterminals nonterminal;
	Id variable;
	Expr* expression;
} Assign;

// Events
typedef enum {event_setup, event_turn, event_close} Events;
typedef struct {
	Nonterminals nonterminal;
	Events* eventType;
	Scope* scope;
} Event;

// Vblariable
typedef struct {
	Nonterminals nonterminal;
	Id name;
	Type* type;
} Variable;

// Preamble
typedef struct {
	Nonterminals nonterminal;
	Id name;
	YablList* ids;
} Preamble;

typedef struct {
	Nonterminals nonterminal;
	struct {int x, y;} dimensions;
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


Constant createConstant(Number num, Logic intBool, char* Text);
void destroyConstant(Constant* p);
Func createFunc(Id name, Args* funcArgs, Type* returnType, Scope* funcScope);
void destroyFunc(Func* p);
TypeValue createTypeValue(BasicTypes varType, Type* listChild);
void destroyTypeValue(TypeValue* p);
Type createType(TypeValue* value);
void destroyType(Type* p);
Repeatable creatRepeatable(Nonterminals nonterminal, YablList* list);
void destroyRepeatable(Repeatable* p);
Expr createExpr(ExprType ExprType, void* child);
void destroyExpr(Expr* p);
BinaryOperator createBinaryOperator(BinaryOperators bOp, Expr* childExpr1, Expr* childExpr2);
void destroyBinaryOperator(BinaryOperator* p);
UnaryOperator createUnaryOperator(UnaryOperators uOp, Expr* childExpr);
void destroyUnaryOperator(UnaryOperator* p);
IdMutationDot createIdMutationDot(IdMutations childType, void* child, Id name);
void destroyIdMutationDot(IdMutationDot* p);
IdMutationCall createIdMutationCall(IdMutations childType, void* child, Args* mutationArgs);
void destroyIdMutationCall(IdMutationCall* p);
IdMutationIndex createIdMutationIndex(IdMutations childType, void* child, Expr* index);
void destroyIdMutatuinIndex(IdMutationIndex* p);
IdMutation createIdMutation(Id name, void* child, IdMutations mutationType);
void destroyIdMutation(IdMutation* p);
IfStmt createIfStmt(Exprs* cond, Scope* condTrue, Scope* condFalse);
void destroyIfStmt(IfStmt* p);
Repeat createRepeat(void* loop, Scope* repeatScope);

TimesLoop createTimesLoop(Expr* goal);
void destroyTimesLoop(TimesLoop* p);
ForLoop createForLoop(Id varName, Id inputName);
void destroyForLoop(ForLoop *p);
WhileLoop createWhileLoop(Expr* cond);
void destroyWhileloop(WhileLoop* p);
RepeatLoop createRepeatLoop();
void destroyRepeatLoop(RepeatLoop* p);
Initialization createInitialization(Id var, Type* varType);
void destroyInitialization(Initialization* p);
Assign createAssign(Id var, Expr* exp);
void destroyAssign(Assign* p);
Event createEvent(Scope* eventScope, Events* eventType);
void destroyEvent(Event* p);
Variable createVariable(Id name, Type* type);
void destroyVariable(Variable* p);

Exprs createExprs();
void  destroyExprs(Exprs* p);
Stmts createStmts();
void destroyStmts(Stmts* p);
Scope createScope();
void destroyScope(Scope* p);
Args createArgs();
void destroyArgs(Args* p);
Funcs createFuncs();
void  destroyFuncs(Funcs* p);
ListConstant createListConstant();
void destroyListConstant(ListConstant* p);
Funcs createPreambles();
void  destroyPreambles(Funcs* p);