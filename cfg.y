
{%
    char yylex();
    void yyerror();

    #include <stdlib.h>
%}

%code requires
{
    enum node_type = {Id, Factor, Expr};
    typedef struct node{
        enum node_type type;
        union{
            char *str;
            int num;
        } value;
        
        union{
            struct node *node;
            int num;
            char *str;
        } *child, *sibling;
        
    } node;




    enum var_type = {number, logic, text, list};
    struct varType{
        enum var_type type;
        struct var_type *child; //handle list...
    };

    typedef struct listElement {
        char *id; //maybe delete??
        struct varType *type;
        union{
            char *str;
            int num;
        } value;
        struct listElement *prev, *next;  

    } listElement;

    struct var{
        char *id;
        int size; //?
        enum var_type type;
        union{
            char *str;
            int num;
        } value;
    };

    struct list //list type to handle smt args funcs etc?
    {
        char *id;
        int size;
        enum var_type type;
        struct listElement *firstChild; //lastChild?
        //something for initialization
    };

    struct method
    {
        char *name;
        union 
        {
            struct var *variable;
            struct list *list;
        } returnType;
        struct list *params;
        struct node *body;
    };

    struct stmtList //nodeList??
    {
        union {
            struct node *node;
            struct stmtList *list;
        } firstChild;
         
    };

    /* struct condition //expr??
    {
        int i;
        int increment;
        int goal;


    }; */

    struct ifStatement
    {
        struct node *condition;
        struct stmtList *then;
        struct stmeList *else;
    };

    enum loop_type = {while, for, foreach};
    struct loop
    {
        enum loop_type *type;
        union
        {
            struct whileLoop *while;
            struct forLoop *for;
            struct foreachLoop *foreach;
        } loop;
    };

    struct whileLoop
    {
        struct node *condition;
        struct stmtList *loop;
    };

    struct forLoop
    {
        int i;
        int increment;
        int goal;
        struct stmtList *loop;
    };

    struct foreachLoop
    {
        int i;
        int increment;
        int goal;
        struct List *inputList;
        struct stmtList *loop;
    };

    struct function
    {
        char *id;
        struct list *args;
        struct varType *returnType;
        struct stmtList *scope;
        struct function *nextSibling;
    };

    enum event_type = {setup, turn, close};
    struct event
    {
        enum event_type *type;
        struct stmtList *scope;
    };
}

%union
{
    int int;
    char *string;
    struct list *list;
    struct node *node;
    struct var *var;
    struct varType *varType;
    struct loop *loop;
    struct whileLoop *whileLoop;
    struct stmeList *stmtList;
    struct function *function;
    struct event *event;
    struct ifStatement *ifStatement;
}

%token returnskeyword funckeyword 
%token numberdcl logicdcl listdcl textdcl
%token setupevent turnevent closeevent
%token<string> text id
%token<int> number logic
%token scopebegin scopeend endofstatement 
%token setpreamble board boardsize player tile
%token forkeyword in repeat ifkeyword elsekeyword whilekeyword times onkeyword
%token addition subtraction multiplication division modulus not neq eq gt gteq lt lteq assignoperator and or negate returnkeyword
%type Start Setup Preamble Preambles PreableIds AssignInitialization
%type Condition Call Index IdMutation

%type<ifStatement> If 
%type<var> Assign
%type<event> Event SetupEvent TurnEvent CloseEvent 
%type<function> Func
%type<stmtList> Scope Funcs Stmts Exprs Args AfterElse AfterIf
%type<loop> Repeat
%type<varType> Type ReturnsType
%type<node> Id Factor Expr //expresion skal nok væk
%type<list> List 
%type<int> P1 P2 //<-- might be string :(
%type<int> P3 P4 P5 P6 //bool


%%
Start : 
    Preamble Setup
;

Preamble :
	Preambles tile PreableIds setpreamble board boardsize setpreamble player PreableIds 
;

Preambles :
	setpreamble id PreableIds Preambles
|   
;

PreableIds :
	id PreableIds
|
;

Setup :
	Funcs 
;


Funcs :
    Func Funcs
    { 
        struct stmtList *s = malloc(sizeof *s);
        if (!s) YYNOMEM;

        *s =(struct stmtList) {
            .firstChild = $1,
        }
        $1.nextSibling = $2;
        $$ = s

    }
|   %empty
;

Func :
    funckeyword id '('Args')' ReturnsType Scope {
        struct function *s = malloc(sizeof *s);
        if (!s) YYNOMEM;

        *s =(struct function) {
            .id = $2,
            .args = $4,
            .returnType = $6,
            .scope = $7,  
            .nextSibling = NULL
        }
        $$ = s
    }
|   onkeyword Event //<---
    {
        struct function *s = malloc(sizeof *s);
        if (!s) YYNOMEM;

        *s =(struct function) {
            .id = $2.type,
            .args = NULL,
            .returnType = NULL,
            .scope = $2.scope,  
        }
        $$ = s
    }
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
    {
        struct varType *s = malloc(sizeof *s);
        if (!s) YYNOMEM;

        *s =(struct varType) {
            .type = $2.type,
            .child = NULL  
        }
        $$ = s
    }
|   %empty   
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
|   Type id AssignInitialization
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
    {

        struct whileLoop *s1 = malloc(sizeof *s1);
        if (!s1) YYNOMEM;

        *s1 =(struct whileLoop) {
            .condition = $2,
            .body = $3,
        };


        struct loop *s = malloc(sizeof *s);
        if (!s) YYNOMEM;

        *s =(struct loop) {
            .type = while,
            .loop = s1,
        };
        $$ = s
    }
|   number times Scope
        {

        struct forLoop *s1 = malloc(sizeof *s1);
        if (!s1) YYNOMEM;

        *s1 =(struct forLoop) {
            .i = 1,
            .increment = 1,
            .goal = $1,
            .loop = $3,
        };


        struct loop *s = malloc(sizeof *s);
        if (!s) YYNOMEM;

        *s =(struct loop) {
            .type = for,
            .loop = s1,
        };
        $$ = s
    }
|   forkeyword id in id Scope
|   Scope
;

Exprs :
    Expr ',' Exprs
    { 
        struct stmtList *s = malloc(sizeof *s);
        if (!s) YYNOMEM;

        *s =(struct stmtList) {
            .firstChild = $1,
        }
        $1.nextSibling = $2;
        $$ = s

    }
|   %empty
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
    {
         struct var *s = malloc(sizeof *s);
        if (!s) YYNOMEM;

        *s =(struct var) {
            .id = $1.value;
            .type = typeof($3.child);
            .value = $3.child.value; 
        };


        $$ = s
    }
;

AssignInitialization :
    assignoperator Expr
|   %empty
;

Id :
   id IdMutation Dot
;

Dot :
	'.' Id
    {$$ = $2}
|   %empty
;

Call :
	'(' Args ')' 
;

Index :
	'[' Expr ']' //<---
;

IdMutation:
	Call IdMutation
|   Index IdMutation
|   %empty
;

List :  //maybe?
    '['Exprs']' 
    {
        struct list *s = malloc(sizeof *s);
        if (!s) YYNOMEM;

        *s =(struct list) {
            .id = NULL;
            .size = $2.count;
            .type = $2.type;
            .firstChild = $2; 
        };


        $$ = s
    }
;


Type :
    numberdcl 
    {
        struct varType *s = malloc(sizeof *s);
        if (!s) YYNOMEM;

        *s =(struct varType) {
            .type = number,
            .child = NULL  
        }
        $$ = s
    }
|   logicdcl
{
        struct varType *s = malloc(sizeof *s);
        if (!s) YYNOMEM;

        *s =(struct varType) {
            .type = logic,
            .child = NULL  
        }
        $$ = s
    }    
|   textdcl
    {
        struct varType *s = malloc(sizeof *s);
        if (!s) YYNOMEM;

        *s =(struct varType) {
            .type = text,
            .child = NULL  
        }
        $$ = s
    }
|   listdcl Type
    {
        struct varType *s = malloc(sizeof *s);
        if (!s) YYNOMEM;

        *s =(struct varType) {
            .type = list,
            .child = $2  
        }
        $$ = s
    }
;


%%


