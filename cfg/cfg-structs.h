#ifndef YABL_CFG_STRUCTS_H
#define YABL_CFG_STRUCTS_H

#include "../visitor/data.h"

#include "../data-structures/list.h"
#include "cfg-basic-types.h"

typedef enum {start, exprs, scope, expr, stmts, stmt, funcs, func, args, arg, event, ifstmt, repeatstmt, idMutation, variable, preambleTile, preamblePlayers, preambleBoard, assign, initialization, unaryOperator, binaryOperator, typecast, listConstants, listConstant, preambles, type, constant, returnstmt, breakstmt} Nonterminals;


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

typedef Repeatable PreambleTile;

typedef Repeatable PreamblePlayers;

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

typedef enum {et_constant, et_list, et_id_mutation, et_unary_operator, et_binary_operator, et_expression, et_typecast} ExprType;
typedef struct Expr{
	Nonterminals nonterminal;
	ExprType exprType;
	Data* extension;
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

typedef struct {
	Nonterminals nonterminal;
	Expr* cast;
	Type* type;
} TypeCast;

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
	void * loopType; //change name?? pointer til en af de nedenstående loop typer
	Scope* scope;
} Repeat;

typedef struct {
	LoopType loopType;	
	Expr* goal;	
} TimesLoop;

typedef struct {
	LoopType loopType;	
	// Might need some list constant
	Id variableName;
	IdMutation* inputName;
} ForLoop;

typedef struct {
	LoopType loopType;	
	Expr* condition;
} WhileLoop;

typedef struct {
	LoopType loopType;	
} RepeatLoop;

/// break
typedef struct {
	Nonterminals nonterminal;
} Break;


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
	int width, height;
} PreambleBoard;



#endif // !YABL_CFG_STRUCTS_H

