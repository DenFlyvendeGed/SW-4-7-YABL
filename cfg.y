%{

%}

%token returnskeyword funckeyword 
%token id numberdcl logicdcl listdcl textdcl
%token number text logic
%token scopebegin scopeend endofstatement
%token forkeyword in repeat ifkeyword elsekeyword whilekeyword times
%token addition subtraction multiplication division modulus not neq eq gt gteq lt lteq assignoperator and or negate returnkeyword

%%
Start : 
    Funcs
;

Funcs :
    Func Funcs
|   /* empty */
;

Func :
    funckeyword id "("Args")" ReturnsType Scope
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
;

Stmts :
    Stmt endofstatement Stmts 
|   /* empty */
;

Stmt :
    Assign
|   If
|   repeat Repeat 
|   Type id AssignInitialization
|   Scope
|   Expr
|   returnkeyword Expr
|   /* empty */   
;

If :
	ifkeyword Condition Scope AfterIf
;

AfterIf :
	elsekeyword AfterElse
|   /* empty */
;

AfterElse :
	If
|   Scope
;

Condition : 
    Expr
;

Repeat :
    whilekeyword Condition Scope
|   number times Scope
|   forkeyword id in id Scope
;

Exprs :
    Expr "," Exprs
|   /* empty */
;

Expr :
    P6
;

Factor :
    "("Expr")"
|   negate id
|   number 
|   logic
|   text
|   List
|   Id
;

P0 :
    P0 not Factor
|   Factor
;

P1 :
    P1 multiplication P0
|   P1 division P0
|   P1 modulus P0
|   P0
;

P2 :
    P2 addition P1
|   P2 subtraction P1
|   P1
;

P3 :
    P3 gt P2
|   P3 lt P2
|   P3 gteq P2
|   P3 lteq P2
|   P2
;

P4 :
    P4 eq P3
|   P4 neq P3
|   P3
;

P5 :
    P5 and P4
|   P4
;

P6 :
    P6 or P5
|   P5
;

Assign :
    id assignoperator Expr
;

AssignInitialization :
    assignoperator Expr
|   /* empty */
;

Id :
   id IdMutation Dot
;

Dot :
	"." Id
|   /* empty */
;

Call :
	"(" Args ")"
;

Index :
	"[" Expr "]"
;

IdMutation:
	Call IdMutation
|   Index IdMutation
|   /* empty */
;

List :
    "["Exprs"]"
;

Type :
    numberdcl    
|   logicdcl    
|   textdcl
|   listdcl Type
;

%%

