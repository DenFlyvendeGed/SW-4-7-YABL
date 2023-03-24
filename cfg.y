

{%
    char yylex();
    void yyerror();

    #include <stdlib.h>
    #include "cfg.h"
%}

%code requires
{
    
}

%union
{
    int number;
    char *string;
    Func *function;
    TypeValue *typeValue;
    Type *type;
    Repeatable *repeatable;
    Expr *expr;
    BinaryOperator *binaryOp;
    UnaryOperator *unaryOp;
    IdMutationIndex *idMutationIndex;
    IdMutation *idMutation;
    IfStmt *ifStmt;
    Repeat *repeat;
    TimesLoop *timesLoop;
    ForLoop *forLoop;
    WhileLoop *whileLoop;
    RepeatLoop *repeatLoop;
    Initialization *initialization;
    Assign *assign;
    Event *event;
    Variable *variable;
    Preamble *preamble;
    PreamblBoard *preamblBoard;
    PreambleTileItem *preambleTileItem;
    PreambleTile *preambleTile;
    PreamblePlayes *preamblePlayes;
}

%token returnskeyword funckeyword 
%token numberdcl logicdcl listdcl textdcl
%token setupevent turnevent closeevent
%token<string> text id
%token<number> number logic
%token scopebegin scopeend endofstatement 
%token setpreamble board boardsize player tile
%token forkeyword in repeat ifkeyword elsekeyword whilekeyword times onkeyword
%token addition subtraction multiplication division modulus not neq eq gt gteq lt lteq assignoperator and or negate returnkeyword
%type
%type<ifStmt> If 
%type<variable> Assign
%type<event> Event SetupEvent TurnEvent CloseEvent 
%type<function> Func
%type<repeatable>  Funcs Stmts Exprs Args Scope
%type AfterElse AfterIf
%type IdMutation IdMutation
%type Index
%type Call
%type<repeat> Repeat
%type<typeValue> Type ReturnsType
%type<expr> Id Factor Expr //expresion skal nok væk
%type List 
%type<string> P1 P2 //<-- might be string :(
%type<int> P3 P4 P5 P6 //bool


%%
Start : 
    Preamble Funcs
;

Preamble :
	Preambles tile PreableIds setpreamble board boardsize setpreamble player PreableIds 
;

Preambles :
	setpreamble id PreableIds Preambles
|   /* empty */
;

PreableIds :
	id PreableIds PreableBracets
|
;

PreableBracets:
	"[" PreableInsideBracets "]"
|   /* empty */
;

PreambleBracets:
	number
|   /* empty */
;

PreableInsideBracets:
	number
|   /* empty */
;




Funcs :
    Func Funcs
|   /* empty */
;

Func :
    funckeyword id "("Args")" ReturnsType Scope
|   Event { $$ = $1; }
;

Event :
	CloseEvent
|   SetupEvent
|   TurnEvent
;

SetupEvent : 
	setupevent Scope
    {
        struct event *s = malloc(sizeof *s);
        if (!s) YYNOMEM;

        *s =(struct event) {
            .type = setup,
            .scope = $2,  
        }
        $$ = s
    }
;

TurnEvent :
	turnevent Scope
        {
        struct event *s = malloc(sizeof *s);
        if (!s) YYNOMEM;

        *s =(struct event) {
            .type = turn,
            .scope = $2,  
        }
        $$ = s
    }
;

CloseEvent :
	closeevent Scope
        {
        struct event *s = malloc(sizeof *s);
        if (!s) YYNOMEM;

        *s =(struct event) {
            .type = close,
            .scope = $2,  
        }
        $$ = s
    }
;


Args :
    Exprs
;

ReturnsType : 
    returnskeyword Type
|   /* empty */   
;

Scope :
    scopebegin Stmts scopeend
    {
        struct stmtList *s = malloc(sizeof *s);
        if (!s) YYNOMEM;

        *s =(struct stmtList) {
            .firstChild = $2,
        }
        $$ = s

    }
;

Stmts :
    Stmt endofstatement Stmts 
    {
        struct stmtList *s = malloc(sizeof *s);
        if (!s) YYNOMEM;

        *s =(struct stmtList) {
            .firstChild = $1,
        }
        $1.nextSibling = $3; // <----
        $$ = s

    }
|   %empty
;

Stmt :
    Assign
|   If
|   repeat Repeat 
|   Initialization AssignInitialization
|   Scope
|   Expr
|   returnkeyword Expr
|   %empty   
;

If :
	ifkeyword Condition Scope AfterIf
    {
         struct ifStatement *s = malloc(sizeof *s);
        if (!s) YYNOMEM;

        *s =(struct ifStatement) {
            .condition = $2,
            .then = $3,
            .else = $4,
        };
        $$ = s
    }
;

AfterIf :
	elsekeyword AfterElse {$$ = $2}
|   %empty
;

AfterElse :
	If // <---
|   Scope
;

Condition : 
    Expr
;

Repeat :
    whilekeyword Condition Scope
|   Expr times Scope
|   forkeyword id in Id Scope
|   Scope
;

Exprs :
    Exprs ',' Expr { $$ = exprsAddExpr($1, $3); }
|   %empty { $$ = createExprs(); }
;

Expr : //<--
    P6
    {
        struct node *s = malloc(sizeof *s);
        if (!s) YYNOMEM;

        *s =(struct node) {
            .type = Expr,
            .child = $1,
            .sibling = NULL
        }
        $$ = s
    }
;

Factor : // <-----
    '('Expr')' {
        struct node *s = malloc(sizeof *s);
        if (!s) YYNOMEM;

        *s =(struct node) {
            .type = Factor,
            .child = $2,
            .sibling = NULL
        }
        $$ = s
        }
    |   negate id // <---
    |   number 
    |   logic 
    |   text
    |   List 
    |   Id 
    {
        struct node *s = malloc(sizeof *s);
        if (!s) YYNOMEM;

        *s =(struct node) {
            .type = Factor,
            .child = $1,
            .sibling = NULL
        }
        $$ = s
    }
;

P0 :
    P0 not Factor
|   Factor
;

P1 :
    P1 multiplication P0 {$$ = $1 * $3}
|   P1 division P0 {$$ = $1 / $3}
|   P1 modulus P0 {$$ = $1 % $3}
|   P0 {$$ = $1}
;

P2 :
    P2 addition P1 {$$ = $1 + $3}
|   P2 subtraction P1 {$$ = $1 - $3}
|   P1 {$$ = $1}
;

P3 :
    P3 gt P2 {$$ = $1 > $3}
|   P3 lt P2 {$$ = $1 < $3}
|   P3 gteq P2 {$$ = $1 >= $3}
|   P3 lteq P2 {$$ = $1 <= $3}
|   P2 {$$ = $1}
;

P4 :
    P4 eq P3 {$$ = $1 == $3}
|   P4 neq P3 {$$ = $1 != $3}
|   P3 {$$ = $1}
;

P5 :
    P5 and P4 {$$ = $1 &  $3}
|   P4 {$$ = $1}
;

P6 :
    P6 or P5 {$$ = $1 | $3}
|   P5 {$$ = $1}
;

Assign :
    Id assignoperator Expr
;

Initialization :
	Type Id
;

AssignInitialization :
    assignoperator Expr
|   %empty
;

Id :
   id IdMutation 
;

Dot :
	'.' Id
;

Call :
	'(' Args ')' 
;

Index :
	'[' Expr ']' //<---
;

IdMutation:
	Call IdMutation
    {
        ($1)->type = $2;
        $$ = $1;
    }
|   Index IdMutation
|   Dot
|   /* empty */
;

List :  //maybe?
    '['Exprs']' 
    {
        $$ = NULL //Create List 
    }
;


Type :
    numberdcl 
    {
        struct Typedcl *s = malloc(sizeof *s);
        if (!s) YYNOMEM;

        *s =(struct TypeValue) {
            .type = number,
            .child = NULL  
        }
        $$ = s
    }
|   logicdcl
    {
        struct TypeValue *s = malloc(sizeof *s);
        if (!s) YYNOMEM;

        *s =(struct TypeValue) {
            .type = logic,
            .child = NULL  
        }
        $$ = s
    }    
|   textdcl
    {
        struct TypeValue *s = malloc(sizeof *s);
        if (!s) YYNOMEM;

        *s =(struct TypeValue) {
            .type = text,
            .list = NULL  
        }
        $$ = s
    }
|   listdcl Type
    {
        struct TypeValue *s = malloc(sizeof *s);
        if (!s) YYNOMEM;

        *s =(struct TypeValue) {
            .type = list,
            .list = $2  
        }
        $$ = s
    }
;


%%



