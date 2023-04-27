%{
    void yyerror();

    #include <stdlib.h>
    #include "../cfg.h"
	#include "lex.yy.h"
	#define YYERROR_VERBOSE 1
	extern char* yytext;
	extern int yylineno;

	Repeatable* YABL_AST;
%}

%union
{
	char* text;

	Type* type;

	Exprs* exprs;
	Expr*  expr;
	Stmts* stmts;
	Stmt*  stmt;
	Scope* scope;
	Args*  args;
	Funcs* funcs;
	Func*  func;
	Event* event;
	Id*    id;

	Repeat*     repeat;

	ListConstant*    listConstant;
	IdMutation*      idMutation;
	IdMutationIndex* idMutationIndex;
	IdMutationCall*  idMutationCall;
	IdMutationDot*   idMutationDot;

	Initialization*  initialization;
	Assign*          assign;
	Variable*        variable;

	Preambles*       preambles;
	PreambleTile*    preambleTile;
	PreambleBoard*   preambleBoard;
	PreamblePlayers* preamblePlayers;
}

%token returnskeyword funckeyword 
%token numberdcl logicdcl listdcl textdcl
%token setupevent turnevent closeevent
%token<text> text id number logic boardsize
%token scopebegin scopeend endofstatement 
%token setpreamble board boardsize player tile
%token forkeyword in repeat ifkeyword elsekeyword whilekeyword times onkeyword
%token addition subtraction multiplication division modulus not neq eq gt gteq lt lteq assignoperator and or negate returnkeyword
%token lparen rparen lsparen rsparen lcparen rcparen dot comma
%token<stmt> breakkeyword

%type<exprs> Exprs List ExprsContinue
%type<expr>  Expr P0 P1 P2 P3 P4 P5 P6 Condition Factor AssignInitialization
%type<stmts> Stmts
%type<stmt>  Stmt If
%type<scope> Scope AfterIf AfterElse
%type<args>  Args
%type<funcs> Funcs
%type<func>  Func
%type<event> Event TurnEvent CloseEvent SetupEvent

%type<repeat> Repeat
%type<assign> Assign 
%type<idMutation> Id IdMutation
%type<idMutationDot> Dot
%type<idMutationIndex> Index 
%type<idMutationCall> Call 
%type<type> Type ReturnsType
%type<initialization> Initialization

%type<preambles> Preambles
%type<preambleTile> PreambleTile PreambleTileTypes
%type<preambleBoard> PreambleBoard
%type<preamblePlayers> PreamblePlayers PreamblePlayer

%%
Start : 
	Preambles Funcs{
		YABL_AST = createRepeatable(start);
		repeatablePushChild(YABL_AST, $1);
		repeatablePushChild(YABL_AST, $2);
	}
;

Preambles:
	PreambleBoard  Preambles  { $$ = preamblesPushPreamble($2, $1); }
|   PreamblePlayer Preambles  { $$ = preamblesPushPreamble($2, $1); }
|   PreambleTile   Preambles  { $$ = preamblesPushPreamble($2, $1); }
|   %empty					  { $$ = createPreambles(); }
;

PreambleBoard:
	board boardsize { $$ = createPreambleBoard($2); free($2); }
;

PreamblePlayer:
	player PreamblePlayers { $$ = $2; }
;

PreamblePlayers:
	text PreamblePlayers { $$ = preamblePlayersAddPlayer($2, $1); }
|   %empty { $$ = createPreamblePlayers(); }
;

PreambleTile:
	tile PreambleTileTypes
;

PreambleTileTypes:
	Initialization PreambleTileTypes { preambleTileAddInitialiation($2, $1); }
|   %empty { $$ = createPreambleTile(); }
;

Funcs :
    Func Funcs { $$ = funcsAddFunc($2, $1); }
|   %empty     { $$ = createFuncs(); }
;

Func :
    funckeyword id lparen Args rparen ReturnsType Scope { $$ = createFunc($2, $4, $6, $7 ); }
|   Event { $$ = (Func*)$1; }
;

Event :
	CloseEvent { $$ = $1; }
|   SetupEvent { $$ = $1; }
|   TurnEvent  { $$ = $1; }
;

SetupEvent : 
	setupevent Scope { $$ = createEvent($2, event_setup); }
;

TurnEvent :
	turnevent Scope { $$ = createEvent($2, event_turn); }
;

CloseEvent :
	closeevent Scope { $$ = createEvent($2, event_close); }
;

Args :
	 Initialization comma Args { $$ = argsAddInitialization($3, $1); }
|    %empty { $$ = createArgs(); }
;

ReturnsType : 
    returnskeyword Type { $$ = $2; }
|   %empty { $$ = (Type*)NULL; }
;

Scope :
    scopebegin Stmts scopeend { $$ = createScope($2); }
;

Stmts :
    Stmt Stmts { if( $1 != NULL) stmtsAddStmt($2, $1); $$ = $2; }
|   %empty { $$ = createStmts(); }
;

Stmt :
    Assign endofstatement { $$ = (Stmt*)$1; }
|   If { $$ = (Stmt*)$1; }
|   repeat Repeat { $$ = (Stmt*)$2; }
|   Initialization endofstatement { $$ = (Stmt*)$1; }
|   Scope { $$ = (Stmt*)$1; }
|   Expr endofstatement { $$ = (Stmt*)$1; }
|   returnkeyword Expr endofstatement { $$ = (Stmt*)createReturnStmt($2); }
|   breakkeyword endofstatement { $$ = (Stmt*)createBreak(); }
|   endofstatement { $$ = (Stmt*)NULL; }
;

If :
	ifkeyword Condition Scope AfterIf { $$ = (Stmt*)createIfStmt($2, $3, $4); }
;

AfterIf :
	elsekeyword AfterElse 
|   %empty { $$ = createScope(createStmts()); }
;

AfterElse :
	If    { $$ = (Scope*)createStmts(); scopeAddStmt($$, (Stmt*)$1); }
|   Scope { $$ = $1; }
;

Condition : 
    Expr { $$ = $1; }
;

Repeat :
    whilekeyword Condition Scope { $$ = createRepeat(createWhileLoop($2), $3); }
|   Expr times Scope { $$ = createRepeat(createTimesLoop($1), $3); }
|   forkeyword id in Id Scope { $$ = createRepeat(createForLoop($2, $4), $5); }
|   Scope { $$ = createRepeat(createRepeatLoop(), $1); }
;

Exprs :
    Expr ExprsContinue { if($1 != NULL) exprsAddExpr($2, $1); $$ = $2; }
|   %empty { $$ = createExprs(); }
;

ExprsContinue:
	comma Exprs { $$ = $2; }
|   %empty    { $$ = createExprs(); }

Expr :
    P6 { $$ = $1; }
;

Factor :
    lparen Expr rparen { $$ = $2; }
|   negate Factor{ $$ = createExpr(et_unary_operator, createUnaryOperator(uo_negate, $2)); }
|   number { $$ = createExpr(et_constant, createConstant(td_number, $1)); }
|   logic { $$ = createExpr(et_constant, createConstant(td_logic, $1)); }
|   text { $$ = createExpr(et_constant, createConstant(td_text, $1)); }
|   List { $$ = createExpr(et_constant, $1); }
|   Id { $$ = createExpr(et_id_mutation, $1); } 
;

P0 :
    not Factor { $$ = createExpr(et_unary_operator, createUnaryOperator(uo_not, $2)); }
|   Factor { $$ = $1; }
;

P1 :
    P1 multiplication P0 { $$ = createExpr(et_unary_operator, createBinaryOperator(bo_mul, $1, $3)); }
|   P1 division P0 { $$ = createExpr(et_binary_operator, createBinaryOperator(bo_division, $1, $3)); }
|   P1 modulus P0 { $$ = createExpr(et_binary_operator, createBinaryOperator(bo_modulus, $1, $3)); }
|   P0 { $$ = $1; }
;

P2 :
    P2 addition P1 { $$ = createExpr(et_binary_operator, createBinaryOperator(bo_plus, $1, $3)); }
|   P2 subtraction P1 { $$ = createExpr(et_binary_operator, createBinaryOperator(bo_minus, $1, $3)); }
|   P1 { $$ = $1; }
;

P3 :
    P3 gt P2 { $$ = createExpr(et_binary_operator, createBinaryOperator(bo_gt, $1, $3)); }
|   P3 lt P2 { $$ = createExpr(et_binary_operator, createBinaryOperator(bo_lt, $1, $3)); }
|   P3 gteq P2 { $$ = createExpr(et_binary_operator, createBinaryOperator(bo_gteq, $1, $3)); }
|   P3 lteq P2 { $$ = createExpr(et_binary_operator, createBinaryOperator(bo_lteq, $1, $3)); }
|   P2 { $$ = $1; }
;

P4 :
    P4 eq P3 { $$ = createExpr(et_binary_operator, createBinaryOperator(bo_eq, $1, $3)); }
|   P4 neq P3 { $$ = createExpr(et_binary_operator, createBinaryOperator(bo_neq, $1, $3)); }
|   P3 { $$ = $1; }
;

P5 :
    P5 and P4 { $$ = createExpr(et_binary_operator, createBinaryOperator(bo_and, $1, $3)); }
|   P4 { $$ = $1; }
;

P6 :
    P6 or P5 { $$ = createExpr(et_binary_operator, createBinaryOperator(bo_or, $1, $3)); }
|   P5 { $$ = $1; }
;

Assign :
    Id assignoperator Expr { $$ = createAssign($1, $3); }
;

Initialization :
	Type id AssignInitialization { $$ = (Stmt*)createInitialization($2, $1, $3); }
;

AssignInitialization :
    assignoperator Expr { $$ = $2; }
|   %empty { $$ = (Expr*)NULL; }
;

Id :
   id IdMutation { $$ = (IdMutation*)createIdMutation($1, $2); }
;

Dot :
	dot Id { $$ = createIdMutationDot($2); }
;

Call :
	lparen Exprs rparen IdMutation { $$ = createIdMutationCall($4, $2); }
;

Index :
	lsparen Expr rsparen IdMutation { $$ = createIdMutationIndex($4, $2); }
;

IdMutation:
	Call { $$ = (IdMutation*)$1; }
|   Index { $$ = (IdMutation*)$1; }
|   Dot { $$ = (IdMutation*)$1; }
|   %empty { $$ = (IdMutation*)NULL; }
;

List :
    lsparen Exprs rsparen  { $$ = createListConstant($2); }
;

Type :
    numberdcl { $$ = createType(createTypeValue(bt_number, NULL)); }
|   logicdcl { $$ = createType(createTypeValue(bt_logic, NULL)); }
|   textdcl { $$ = createType(createTypeValue(bt_text, NULL)); }
|   listdcl Type { $$ = createType(createTypeValue(bt_list, $2)); }
;

%%



