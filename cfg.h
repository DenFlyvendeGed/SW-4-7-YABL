#include "./data-structures/list.h"

typedef enum {exprs, scope, expr, stmts, stmt, funcs, func, args, arg, event, ifstmt, repeatstmt, idMutation, variable, preamblePlayers, assign, initialization} Nonterminals;



typedef enum {number, text, logic, list} BasicTypes;
struct Typedcl;
typedef union {
	struct Typedcl* list;
	BasicTypes type;
} TypeValue;

typedef struct Typedcl{
	Nonterminals nonterminal;	
	TypeValue typeval;	
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
	YablList children;
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
	Args args;
	Type returntype;
	Scope scope;
} Func;

// Expr

typedef enum {et_constant, et_id_mutation, et_unary_operator, et_binary_operator, et_expressoin} ExprType;
typedef struct Expr{
	Nonterminals nonterminal;
	ExprType expr_type;
	void* child;
} Expr;

typedef enum {
	bo_plus, bo_minus, bo_divsion, bo_mul, bo_modulus, 
	bo_not, bo_eq, bo_neq, bo_gt, bo_gteq, bo_lt, bo_lteq,
	bo_and, bo_or 
} BinaryOperators;

typedef struct {
	BinaryOperators bo;
	Expr child_expr1;
	Expr child_expr2;
} BinaryOperator;

typedef enum {
	uo_negate
} UnaryOperators;

typedef struct {
	UnaryOperators uo;
	Expr child_expr;
} UnaryOperator;

// Id Mutation
typedef enum {im_none, im_dot, im_call, im_index} IdMutations;

typedef struct{
	Id name;
	IdMutations child_type;
	void* child;
} IdMutationDot;

typedef struct{
	Args args;
	IdMutations child_type;
	void* child;
} IdMutationCall;

typedef struct{
	struct Expr index;
	IdMutations child_type;
	void* child;
} IdMutationIndex;

typedef struct {
	Nonterminals nonterminal;
	Id name;
	IdMutations id_mutation;
	void* child;
} IdMutation;

/// if
typedef struct {
	Nonterminals nonterminal;
	Exprs condition;
	Scope then;
	Scope elsestmt;
} IfStmt;

/// repeat
typedef enum {lt_timesloop, lt_forloop, lt_whileloop, lt_repeatloop} LoopType;
typedef struct {
	Nonterminals nonterminal;
	void * loop_type;
	Scope scope;
} Repeat;

typedef struct {
	LoopType loop_type;	
	Expr goal;	
} TimesLoop;

typedef struct {
	LoopType loop_type;	
	// Might need some list constant
	char* variable_name;
	Id input_name;
} ForLoop;

typedef struct {
	LoopType loop_type;	
	Expr condition;
} WhileLoop;

typedef struct {
	LoopType loop_type;	
} RepeatLoop;


/// Initialization
typedef struct {
	Nonterminals nonterminal;
	Type type;
	Id variable;
} Initialization;

/// assing
typedef struct {
	Nonterminals nonterminal;
	Id variable;
	Expr expression;
} Assign;

// Events
typedef enum {event_setup, event_turn, event_close} Events;
typedef struct {
	Nonterminals nonterminal;
	Events event_type;
	Scope scope;
} Event;

// Variable
typedef struct {
	Nonterminals nonterminal;
	Id name;
	Type type;
} Variable;

// Preamble
typedef struct {
	Nonterminals nonterminal;
	Id name;
	YablList ids;
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
	YablList tile_items;
} PreambelTile;

typedef struct {
	Nonterminals nonterminal;
	YablList ids;
} PreamblePlayers;

Exprs exprsAddExpr(Exprs self, Expr expr);


